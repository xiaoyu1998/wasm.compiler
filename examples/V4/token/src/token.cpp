#include <token.hpp>
#include <wasm_log.hpp>
#include <make_log.hpp>
#include <return.hpp>

using namespace wasm;


ACTION token::create( regid  issuer,
                       asset  maximum_supply )
{
    require_auth( issuer );
    auto maintainer = get_maintainer(_self);
    check( issuer == maintainer, "issuer not contract maintainer");

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


ACTION token::issue( regid to, asset quantity, string memo )
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

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
      wasm::transaction inline_trx(get_self(), name("transfer"), std::vector<permission>{{st.issuer, name("wasmio_owner")}}, std::tuple(st.issuer, to, quantity, memo));
      inline_trx.send();

    }
}

ACTION token::retire( asset quantity, string memo )
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

ACTION token::transfer( regid    from,
                        regid    to,
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

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );
}

void token::sub_balance( regid owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   account from;
   check( from_acnts.get( from, value.symbol.code().raw()), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance from.balance:" + from.balance.to_string() + "value:" + value.to_string() );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void token::add_balance( regid owner, asset value, regid payer )
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

ACTION token::open( regid owner, const symbol& symbol, regid payer )
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
        a.owner   = owner;
        a.balance = asset{0, symbol};
      });
   }
}

ACTION token::close( regid owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( _self, owner.value );

   account account;
   check( acnts.get(account, symbol.code().raw() ), "Balance row already deleted or never existed. Action won't have any effect." );
   check( account.balance.amount == 0, "Cannot close because the balance is not zero." );
   acnts.erase( account, wasm::no_payer );
}

ACTION2 token::balance_of( regid owner, const symbol& symbol )
{
    auto sym = symbol.code(); 

    currency_stats st;
    stats statstable( _self, sym.raw() );
    statstable.get( st, sym.raw() );

    check( symbol== st.supply.symbol, "symbol precision mismatch" );

    accounts from_acnts( _self, owner.value );

    account from;
    check( from_acnts.get( from, symbol.code().raw()), "no balance object found" );

    WASM_LOG_PRINT(true,
      " owner:", owner,
      " symbol:", symbol,
      " from.balance:", from.balance,
      "\n")

    make_log(owner, "account", wasm::pack(from));

    set_return(wasm::pack(from.balance));
    
    return (int64_t)from.balance.amount;

}


extern "C" {
   int64_t apply( uint64_t receiver, uint64_t code, uint64_t action ) {
       //if(code == receiver || code == wasmio_bank.value ){
           switch( action ) { 
             case wasm::name( "create" ).value: 
                 wasm::execute_action( wasm::regid(receiver), wasm::regid(code), &token::create ); 
                 break;
             case wasm::name( "issue" ).value: 
                 wasm::execute_action( wasm::regid(receiver), wasm::regid(code), &token::issue ); 
                 break;
             case wasm::name( "retire" ).value: 
                 wasm::execute_action( wasm::regid(receiver), wasm::regid(code), &token::retire ); 
                 break;
             case wasm::name( "transfer" ).value: 
                 wasm::execute_action( wasm::regid(receiver), wasm::regid(code), &token::transfer ); 
                 break;
             case wasm::name( "open" ).value: 
                 wasm::execute_action( wasm::regid(receiver), wasm::regid(code), &token::open ); 
                 break;
             case wasm::name( "close" ).value: 
                 wasm::execute_action( wasm::regid(receiver), wasm::regid(code), &token::close ); 
                 break;
             case wasm::name( "balance_of" ).value: 
                 return wasm::execute_action_with_return( wasm::regid(receiver), wasm::regid(code), &token::balance_of ); 
                 break;
             default:
                 check(false, "action did not exist!");
                 break;
           }
       //}

       return 0;
   }
}

