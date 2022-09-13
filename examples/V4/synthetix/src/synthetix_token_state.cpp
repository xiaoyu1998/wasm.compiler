#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>

using namespace wasm;
using namespace synthetix;



void synthetix::init_synthetix_token_state(){
    require_auth( get_self() );


    asset  maximum_supply(10000000.0000000000, symbol(symbol_code(XT), PRECISION));
    auto   sym = maximum_supply.symbol;

    //create xt
    currency_stats st;
    stats statstable( _self, sym.code().raw() );
    check( !statstable.get( st, sym.code().raw() ), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = get_self();
    });

}



ACTION synthetix::transfer( regid   from,
                         regid   to,
                         asset   quantity,
                         string  memo )
{
    // from mintedable token
    // if(to == _get_self()){



    // }
    _transfer_normal(from, to ,quantity, memo);
}

ACTION synthetix::create( regid  issuer,
                       asset  maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply" );
    check( maximum_supply.amount > 0, "max-supply must be positive" );

    currency_stats st;
    stats statstable( _self, sym.code().raw() );
    check( !statstable.get( st, sym.code().raw() ), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


ACTION synthetix::issue( regid to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    currency_stats st;
    stats statstable(_self, sym.code().raw()) ;
    check( statstable.get( st, sym.code().raw() ), "token with symbol does not exist, create token before issue" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, wasm::no_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    _add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
      wasm::transaction inline_trx(get_self(), name("transfer"), std::vector<permission>{{st.issuer, name("wasmio.owner")}}, std::tuple(st.issuer, to, quantity, memo));
      inline_trx.send();

    }
}

void synthetix::_transfer_normal( regid   from,
                               regid   to,
                               asset   quantity,
                               string  memo )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();

    currency_stats st;
    stats statstable( _self, sym.raw() );
    statstable.get( st, sym.raw() );

    notify_recipient( from );
    notify_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    _sub_balance( from, quantity );
    _add_balance( to, quantity, payer );    
}

void synthetix::_sub_balance( regid owner, asset value ) {
    accounts from_acnts( _self, owner.value );

    account from;
    check( from_acnts.get( from, value.symbol.code().raw()), "no balance object found" );
    check( from.balance.amount >= value.amount, "overdrawn balance" );

    from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
    }

void synthetix::_add_balance( regid owner, asset value, regid payer )
{
    accounts to_acnts( _self, owner.value );

    account to;
    if( !to_acnts.get( to, value.symbol.code().raw() ) ) {
        to_acnts.emplace( payer, [&]( auto& a ){
            a.owner   = owner;
            a.balance = value;
        });
    } else {
        to_acnts.modify( to, wasm::no_payer, [&]( auto& a ) {
            a.balance += value;
        });
    }
}

uint64_t synthetix::_token_state_balance_of( regid owner, uint64_t symbol_code){

    accounts owner_acnts( _self, owner.value );
    account owner_object;
    if(!owner_acnts.get( owner_object, symbol_code)) return 0;

    return owner_object.balance.amount;
}

//fixme:should move to another module
void synthetix::issue_mpa( regid to, asset quantity){

    mpa_t object;
    mpas  mpa_table(_self, SYNTHETIX_SCOPE);

    uint64_t currency_key = quantity.symbol.code().raw();
    check(mpa_table.get(object, currency_key), "Get synth error"); 

     mpa_table.modify(object, wasm::no_payer, [&](auto &s) {
        s.total_supply += quantity;
    });

     notify_bank_to_issue_mpa(to, quantity);

}

void synthetix::notify_bank_to_issue_mpa( regid to, asset quantity){

    //notify bank to mint xusd
    regid issuer_bank = get_mpa_issuer_bank(XUSD);
    wasm::transaction inline_trx(issuer_bank, name("issue"), std::vector<permission>{{get_self(), name("wasmio.code")}}, std::tuple<regid,asset,string>(to, quantity, "issue"));
    inline_trx.send(); 

}

void synthetix::burn_mpa( regid from, asset quantity){

    mpa_t object;
    mpas  mpa_table(_self, SYNTHETIX_SCOPE);

    uint64_t currency_key = quantity.symbol.code().raw();
    check(mpa_table.get(object, currency_key), "Get synth error"); 

     mpa_table.modify(object, wasm::no_payer, [&](auto &s) {
        s.total_supply -= quantity;
    });

    notify_bank_to_burn_mpa(from , quantity);

}

void synthetix::notify_bank_to_burn_mpa( regid from, asset quantity){

    //notify bank to burn
    regid issuer_bank = get_mpa_issuer_bank(XUSD);
    wasm::transaction inline_trx(issuer_bank, name("burn"), std::vector<permission>{{get_self(), name("wasmio.code")}}, std::tuple<regid,asset,string>(from, quantity, "burn"));
    inline_trx.send();

}



