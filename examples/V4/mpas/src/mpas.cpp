#include <mpas.hpp>
#include <wasm_log.hpp>
#include <make_log.hpp>
#include <return.hpp>

using namespace wasm;

static constexpr bool     MPAS_DEBUG    = true;
static constexpr uint64_t TOKEN_VERSION = 100;
static constexpr uint64_t TOKEN_SCOPE   = 100;
static constexpr name     OWNER         = "owner"_n;



/* Usage:
 *    1. set contract owner who can conduct mint/burn/sub/add_balance ops
 *    2. in this case, waykix contract is the owner
 */
ACTION mpas::set_owner( regid owner ) {

    require_auth( get_maintainer(get_self()) );

    _set_uint_value(OWNER, owner.value);
}

ACTION mpas::mint( regid to, asset quantity, string memo )
{
    auto symb = quantity.symbol;
    check( symb.is_valid(),     "invalid symbol name" );
    check( memo.size() <= 256,  "memo has more than 256 bytes" );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    regid owner = _get_owner();
    require_auth( owner );

    asset_stats st;
    assetstat statstable(_self, symb.code().raw()) ;
    if (statstable.get( st, symb.code().raw() )) {
        check( quantity.symbol == st.total_supply.symbol, "symbol precision mismatch" );

        statstable.modify( st, wasm::no_payer, [&]( auto& s ) {
           s.total_supply += quantity;
        });
    } else {    //first-time issue
        statstable.emplace( wasm::no_payer, [&]( auto& s ) {
           s.total_supply = quantity;
           s.issuer       = owner;
        });
    }

    add_balance( to, quantity, st.issuer );
}



ACTION mpas::burn( regid from, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    auto owner = _get_owner();
    //check( from == owner, "only asset owner can burn one's own assets!" );
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    require_auth(owner);

    asset_stats st;
    assetstat statstable(_self, sym.code().raw()) ;
    check( statstable.get( st, sym.code().raw() ), "mpas with symbol does not exist, create mpas before issue" );
    check( quantity.symbol == st.total_supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.total_supply.amount, "quantity exceeds available supply");
    statstable.modify( st, wasm::no_payer, [&]( auto& s ) {
       s.total_supply -= quantity;
    });

    sub_balance( from, quantity );

}


ACTION mpas::transfer(  regid   from,
                        regid   to,
                        asset   quantity,
                        string  memo )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();

    asset_stats st;
    assetstat statstable( _self, sym.raw() );
    statstable.get( st, sym.raw() );

    notify_recipient( from );
    notify_recipient( to );
    //notify_recipient( _get_owner() );//owner will handle settle before transfer

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.total_supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );

}

void mpas::sub_balance( regid owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   //print(" owner:", owner, " quantity:", value);

   account from;
   check( from_acnts.get( from, value.symbol.code().raw()), "no balance object found" );

   // string msg = " overdrawn balance owner:";
   //        msg += owner.to_string();
   //        msg += " quantity:";
   //        msg += value.to_string();
   // check( from.balance.amount >= value.amount, msg );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void mpas::add_balance( regid owner, asset value, regid payer )
{
   accounts to_acnts( _self, owner.value );

   account to;
   if ( !to_acnts.get( to, value.symbol.code().raw() ) ) {
      to_acnts.emplace( payer, [&]( auto& a ) {
        a.owner   = owner;
        a.balance = value;
      }); 
   } else {
      to_acnts.modify( to, wasm::no_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

uint64_t mpas::_get_uint_value(const name& key) {

    uint_storage_t object;
    tbl_uint_store uint_table(_self, TOKEN_SCOPE);
    check( uint_table.get( object, key.value ), "key does not exist" );

    return object.value;

}

void mpas::_set_uint_value(const name& key, uint64_t value) {

    uint_storage_t object;
    tbl_uint_store uint_table(_self, TOKEN_SCOPE);

    if (uint_table.get( object, key.value )) {
        uint_table.modify( object, wasm::no_payer, [&]( auto& s ) {
           s.value = value;
        });
    } else {
        uint_table.emplace( wasm::no_payer, [&]( auto& s ) {
           s.key   = key;
           s.value = value;
        });
    }

}

void mpas::_del_uint_value(const name& key) {

    tbl_uint_store uint_table(_self, TOKEN_SCOPE);
    uint_table.erase(key.value, wasm::no_payer);

}

regid mpas::_get_owner() {
    return regid(_get_uint_value(OWNER));
}

ACTION mpas::balance_of( regid owner, const symbol& symbol )
{
    auto sym = symbol.code(); 

    asset_stats st;
    assetstat statstable( _self, sym.raw() );
    statstable.get( st, sym.raw() );

    WASM_LOG_PRINT(true,
      "symbol:", symbol,
      " st.supply.symbol:", st.total_supply.symbol)

    check( symbol== st.total_supply.symbol, "symbol precision mismatch" );

    accounts from_acnts( _self, owner.value );

    account from;
    check( from_acnts.get( from, symbol.code().raw()), "no balance object found" );

    make_log(owner, "account", wasm::pack(from));

    set_return(wasm::pack(from.balance));

}


WASM_DISPATCH( mpas, (set_owner)(mint)(burn)(transfer)(balance_of))
