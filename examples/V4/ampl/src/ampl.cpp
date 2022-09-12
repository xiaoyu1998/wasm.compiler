#include <ampl.hpp>
#include <ampl_utils.hpp>
#include <inline_transaction.hpp>
#include <ampl_database.hpp>
#include <return.hpp>
#include <make_log.hpp>
#include <exception.hpp>

using namespace wasm;

std::optional<global_t> g_ampl;

uint64_t ampl::_get_oracle_data(regid oracle)
{
     //return GET_ORACLE_DATA(oracle);
  return 100000000;
}

uint64_t ampl::_rebase(uint64_t epoch, int64_t supply_delta)
{

    g_ampl->total_supply.amount += supply_delta;
    g_ampl->gons_per_fragment     = div_u128(TOTAL_GONS, g_ampl->total_supply.amount);

    return g_ampl->total_supply.amount;
}

int64_t ampl::_compute_supply_delta(uint64_t exchange_rate, int64_t cpi_rate)
{
    //supply_delta = total_supply * (exchange_rate - cpi_rate) / cpi_rate;
    return div_i128(mul_i128(g_ampl->total_supply.amount, exchange_rate - cpi_rate ), cpi_rate);
}


ACTION ampl::init(regid owner, symbol_code code, uint64_t base_cpi, regid cpi_oracle, regid market_oracle)
{
    require_auth( get_maintainer(get_self()));

    check(!g_ampl.has_value(), "ampl already initialize");

    global_t storage(get_self().value);
    storage.total_supply        = TO_ASSET(INITIAL_FRAGMENTS_SUPPLY, code);
    storage.gons_per_fragment   = div_u128(TOTAL_GONS, storage.total_supply.amount);

    storage.min_rebase_time_interval_sec = ONE_DAY;
    storage.rebase_window_offset_sec     = TWO_HOURS;
    storage.rebase_window_length_sec     = FIFTEEN_MINUTES;
    storage.last_rebase_time_stamp_sec   = 0;
    storage.epoch                        = 0;
    storage.base_cpi                     = base_cpi;
    storage.rebase_lag                   = 30;

    storage.cpi_oracle                   = cpi_oracle;
    storage.market_oracle                = market_oracle;

    WASM_LOG_FPRINT(AMPL_DEBUG, "storage:%", storage)
    g_ampl = storage;

    account_t _owner(owner.value);
    check(!wasm::db::get(_owner), contract_failed{}, "% already exist", owner);

    //_owner.balance = TO_ASSET(TOTAL_GONS, code);
    _owner.balance = TOTAL_GONS;
    wasm::db::set(_owner);
}

ACTION ampl::rebase()
{
    check(g_ampl.has_value(), "ampl was not initialized");
    require_auth( get_maintainer(get_self()));
    
    uint64_t now = current_block_time();
  
    check(g_ampl->last_rebase_time_stamp_sec + g_ampl->min_rebase_time_interval_sec < now, "to quick to rebase");

    g_ampl->last_rebase_time_stamp_sec = now - now % g_ampl->min_rebase_time_interval_sec + g_ampl->rebase_window_offset_sec;

    uint64_t cpi           = _get_oracle_data(g_ampl->cpi_oracle);
    uint64_t cpi_rate      = mul_u128(cpi, g_ampl->base_cpi);
    uint64_t exchange_rate = _get_oracle_data(g_ampl->market_oracle);
    int64_t  supply_delta  = _compute_supply_delta(exchange_rate, cpi_rate) / g_ampl->rebase_lag;

    _rebase(++g_ampl->epoch, supply_delta);

}


ACTION ampl::transfer( regid   from,
                       regid   to,
                       asset   quantity,
                       string  memo )
{
    check(g_ampl.has_value(), "ampl was not initialized");

    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");

    notify_recipient( from );
    notify_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == g_ampl->total_supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    _sub_balance( from, quantity );
    _add_balance( to, quantity, payer );
}

void ampl::_sub_balance( regid owner, asset value ) {
   account_t _owner( owner.value );
   check( wasm::db::get(_owner), contract_failed{}, "no account object % found", owner );
  
   uint64_t gon_value = mul_u128(value.amount, g_ampl->gons_per_fragment);

   check( _owner.balance >= gon_value, contract_failed{}, "overdrawn balance from.balance:% and gon_value:%", _owner.balance, gon_value );
   _owner.balance -= gon_value;
   wasm::db::set(_owner);
}

void ampl::_add_balance( regid owner, asset value, regid payer )
{
   account_t _owner( owner.value );
   wasm::db::get(_owner);
   
   uint64_t gon_value = mul_u128(value.amount, g_ampl->gons_per_fragment);

   _owner.balance += gon_value;
   wasm::db::set(_owner);
}

ACTION ampl::total_supply()
{
    check(g_ampl.has_value(), "ampl was not initialized");

    WASM_LOG_FPRINT(AMPL_DEBUG, "total_supply:%", g_ampl->total_supply);
    set_return(wasm::pack(g_ampl->total_supply));
}

ACTION ampl::balance_of(regid owner)
{
    check(g_ampl.has_value(), "ampl was not initialized");
    account_t _owner(owner.value);
    wasm::db::get(_owner);//account does not exist will return 0

    uint64_t balance = div_u128(_owner.balance, g_ampl->gons_per_fragment);
    WASM_LOG_FPRINT(AMPL_DEBUG, "_owner:%", balance);
    set_return(wasm::pack(balance));
}

// ACTION ampl::get_ampl(){

//     check(g_ampl.has_value(), "global does not exist"); 

//     WASM_LOG_FPRINT(true, "global:%", g_ampl.value());
//     set_return(wasm::pack<global_t>(g_ampl.value()));
// }

ACTION ampl::set_oracle(regid cpi, regid market){
    check(g_ampl.has_value(), "ampl was not initialized");

    g_ampl->cpi_oracle    = cpi;
    g_ampl->market_oracle = market;   
}


ACTION ampl::set_lag(uint64_t rebase_lag){
    check(g_ampl.has_value(), "ampl was not initialized");

    g_ampl->rebase_lag    = rebase_lag;
}

extern "C" bool pre_dispatch(regid self, regid original_receiver, name action) {
   global_t storage(self.value);
   if(wasm::db::get(storage)) g_ampl = storage;

   return true;
}

extern "C" void post_dispatch(regid self, regid original_receiver, name action) {
  if(g_ampl.has_value()) wasm::db::set(g_ampl.value());

}


