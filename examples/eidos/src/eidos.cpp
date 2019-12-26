#include <eidos.hpp>
//#include <sstream>
#include <chrono>
#include <wasm.hpp>

using namespace wasm;
using std::chrono::system_clock;

ACTION eidos::create( name   issuer,
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


ACTION eidos::issue( name to, asset quantity, string memo )
{

    //if(wasm::name( "wasmio.bank" ) == get_first_receiver()) return;

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

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
      wasm::transaction inline_trx(get_self(), name("transfer"), std::vector<permission>{{st.issuer, name("wasmio.owner")}}, std::tuple(st.issuer, to, quantity, memo));
      //wasm::transaction inline_trx(name("wasmio.bank"), name("transfer"), std::vector<permission>{{st.issuer, name("wasmio.owner")}}, std::tuple(st.issuer, to, quantity, memo));
      inline_trx.send();

    }
}

ACTION eidos::retire( asset quantity, string memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    currency_stats st;
    stats statstable(_self, sym.code().raw()) ;
    check( statstable.get( st, sym.code().raw() ), "token with symbol does not exist" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, wasm::no_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}

ACTION eidos::transfer( name    from,
                      name    to,
                      asset   quantity,
                      string  memo )
{

    // eidos
    if( to == get_self() &&  wasm::name( "wasmio.bank" ) == get_first_receiver() ){
       print("1.inline wasmio.bank transfer\n");
       wasm::transaction inline_trx(name("wasmio.bank"), name("transfer"), std::vector<permission>{{to, name("wasmio.owner")}}, std::tuple(to, from, quantity, memo));
       inline_trx.send();

       print("2.token.transfer check_balance\n");
       accounts to_acnts( _self, to.value );
       account contract_self;
       check(to_acnts.get( contract_self, quantity.symbol.code().raw()),"no balance object found" );
       if (contract_self.balance.amount < quantity.amount ){

            asset quantity_issue = quantity;
            quantity_issue.amount += 2500000000;
            print("3.issue new token:",quantity_issue.to_string());
            wasm::transaction inline_trx_issue(get_self(), name("issue"), std::vector<permission>{{to, name("wasmio.owner")}}, std::tuple(to, quantity_issue, string("issue new 25")));
            inline_trx_issue.send();
       }

       print("3.inline token.transfer:",quantity.to_string());
       wasm::transaction inline_trx2(get_self(), name("transfer"), std::vector<permission>{{to, name("wasmio.owner")}}, std::tuple(to, from, quantity, memo));
       inline_trx2.send();
    }

    if(wasm::name( "wasmio.bank" ) == get_first_receiver()) return;

    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();

    currency_stats st;
    stats statstable( _self, sym.raw() );
    statstable.get( st, sym.raw() );

    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );
    
}

void eidos::sub_balance( name owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   account from;
   check( from_acnts.get( from, value.symbol.code().raw()), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void eidos::add_balance( name owner, asset value, name payer )
{
   accounts to_acnts( _self, owner.value );

   account to;
   if( !to_acnts.get( to, value.symbol.code().raw() ) ) {
      to_acnts.emplace( payer, [&]( auto& a ){
        a.owner = owner;
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, wasm::no_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

ACTION eidos::open( name owner, const symbol& symbol, name payer )
{
   require_auth( payer );

   auto sym_code_raw = symbol.code().raw();
   stats statstable( _self, sym_code_raw );

   currency_stats st;
   check( statstable.get( st, sym_code_raw), "symbol does not exist" );
   check( st.supply.symbol == symbol, "symbol precision mismatch" );

   accounts acnts( _self, owner.value );
   account account;

   if( ! acnts.get( account, sym_code_raw ) ) {
      acnts.emplace( payer, [&]( auto& a ){
        a.owner = owner;
        a.balance = asset{0, symbol};
      });
   }
}

ACTION eidos::close( name owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( _self, owner.value );

   account account;
   check( acnts.get(account, symbol.code().raw() ), "Balance row already deleted or never existed. Action won't have any effect." );
   check( account.balance.amount == 0, "Cannot close because the balance is not zero." );
   acnts.erase( account, wasm::no_payer );
}

//eidos
extern "C" {
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
       if(code == receiver || code == wasm::name( "wasmio.bank" ).value ){
           switch( action ) { 
             case wasm::name( "create" ).value: 
                 wasm::execute_action( wasm::name(receiver), wasm::name(code), &eidos::create ); 
                 break;
             case wasm::name( "issue" ).value: 
                 wasm::execute_action( wasm::name(receiver), wasm::name(code), &eidos::issue ); 
                 break;
             case wasm::name( "retire" ).value: 
                 wasm::execute_action( wasm::name(receiver), wasm::name(code), &eidos::retire ); 
                 break;
             case wasm::name( "transfer" ).value: 
                 wasm::execute_action( wasm::name(receiver), wasm::name(code), &eidos::transfer ); 
                 break;
             case wasm::name( "open" ).value: 
                 wasm::execute_action( wasm::name(receiver), wasm::name(code), &eidos::open ); 
                 break;
             case wasm::name( "close" ).value: 
                 wasm::execute_action( wasm::name(receiver), wasm::name(code), &eidos::close ); 
                 break;
             default:
                 check(false, "action did not exist!");
                 break;
           }
       }
   }
}



