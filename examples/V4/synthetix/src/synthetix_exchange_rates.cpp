#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_database.hpp>

using namespace wasm;
using namespace synthetix;

void synthetix::init_synthetix_exchange_rates(){
    _set_rate(XUSD, PRECISION_1, current_block_time());
}

uint64_t synthetix::get_last_round_id_before_elapsed_secs(
    uint64_t currency_key,
    uint64_t starting_round_id,     
    uint64_t starting_times_stamp,
    uint64_t time_diff){

    uint64_t round_id        = starting_round_id;

    while(true){
        uint64_t next_time_stamp = 0;
        uint64_t next_rate;
        _get_rate_and_timestamp_at_round(currency_key, round_id + 1, next_rate, next_time_stamp);

        if( next_time_stamp == 0  || next_time_stamp > starting_times_stamp + time_diff)
            return round_id;

        round_id ++;
    }
    return round_id;
}

uint64_t synthetix::get_current_round_id(uint64_t currency_key){

    round_for_rate_t round(currency_key);
    check(wasm::db::get(round), "Rate of currency_key does not exist");

    return round.round;

}

uint64_t synthetix::effective_value_at_round(
    uint64_t source_currency_key, 
    uint64_t source_amount, 
    uint64_t destination_currency_key,
    uint64_t round_id_for_src,
    uint64_t round_id_for_dest){

    _rate_not_stale(source_currency_key);
    _rate_not_stale(destination_currency_key);   

    if(source_currency_key == destination_currency_key) return source_amount;

    uint64_t src_rate, dest_rate;
    uint64_t src_time, dest_time;
    _get_rate_and_timestamp_at_round(source_currency_key,      round_id_for_src,  src_rate,  src_time);
    _get_rate_and_timestamp_at_round(destination_currency_key, round_id_for_dest, dest_rate, dest_time);

    //int128_t tmp = (int128_t)source_amount * (int128_t)src_rate / (int128_t)dest_rate;
    int128_t tmp = synthetix_divide_decimal(synthetix_multiply_decimal(source_amount, src_rate), dest_rate);
    return (uint64_t)tmp;
    
}

uint64_t synthetix::effective_value(
    uint64_t source_currency_key, 
    uint64_t source_amount, 
    uint64_t destination_currency_key){

    _rate_not_stale(source_currency_key);
    _rate_not_stale(destination_currency_key);      

    if(source_currency_key == destination_currency_key) return source_amount;

    //int128_t tmp = (int128_t)source_amount * (int128_t)_get_rate(source_currency_key) / (int128_t)_get_rate(destination_currency_key);
    int128_t tmp = synthetix_divide_decimal(synthetix_multiply_decimal(source_amount, get_rate(source_currency_key)), get_rate(destination_currency_key));
    return (uint64_t)tmp;    
}


uint64_t synthetix::get_rate(uint64_t currency_key){
    return _rate_for_currency(currency_key);
}

uint64_t synthetix::_rate_for_currency(uint64_t currency_key){

    auto round_id = get_current_round_id(currency_key);
    rate_and_updated_time_t rate(symbol_code(currency_key), round_id);
    check(wasm::db::get(rate), "Rate of currency_key does not exist");
    return rate.rate;

}

void synthetix::_rates_for_currencies(const vector<uint64_t>& currency_keys, map<uint64_t, uint64_t>& rates){
    for(auto currency_key: currency_keys){
        rates[currency_key] = get_rate(currency_key);
    }
}

void synthetix::_rates_and_stale_for_currencies(const vector<uint64_t>& currency_keys, map<uint64_t, uint64_t>& rates, bool& any_rate_stale){

    any_rate_stale  = false; 
    uint64_t period = _get_rate_stale_period();

    for(auto currency_key: currency_keys){

        auto round_id = get_current_round_id(currency_key);

        //rate_and_updated_time_t rate((int128_t)currency_key << 64 | round_id);
        rate_and_updated_time_t rate(symbol_code(currency_key), round_id);
        check(wasm::db::get(rate), "Rate of currency_key does not exist");

        if(!any_rate_stale) {
            any_rate_stale = currency_key != XUSD && rate.time + period < current_block_time();
        }
   
        rates[currency_key] = rate.rate;
    }

    any_rate_stale = any_rate_stale;
}

bool synthetix::_rate_is_stale(uint64_t currency_key){
    if(currency_key == XUSD) return false;
    uint64_t rate_stale_period  = _get_rate_stale_period();
    return _last_rate_update_times(currency_key) + rate_stale_period < current_block_time();
}

bool synthetix::_any_rate_is_stale(const vector<uint64_t>& currency_keys){
    uint64_t rate_stale_period  = _get_rate_stale_period();
 
    for(auto currency_key:currency_keys){
        if( currency_key != XUSD && _last_rate_update_times(currency_key) + rate_stale_period < current_block_time())
            return true;
    }
    return false;
}

void synthetix::_rate_not_stale(uint64_t currency_key){
    check(!_rate_is_stale(currency_key), "rate stale or nonexistant currency");
}

uint64_t synthetix::_get_rate_stale_period(){ 
    return get_uint_value(KEY_RATE_STALE_PERIOD);
}

void synthetix::_set_rate(uint64_t currency_key, uint64_t new_rate, uint64_t time_sent){

    check(existing_mpa(currency_key), "Mpa does not exists");

    round_for_rate_t round(currency_key);
    if(wasm::db::get(round)){
        round.round ++;
    } 
    else {
        round.round = 0; 
    }
    wasm::db::set(round);

    rate_and_updated_time_t rate(symbol_code(currency_key), round.round, new_rate, time_sent);
    wasm::db::set(rate);


}

bool synthetix::_get_rate_and_timestamp_at_round(uint64_t currency_key, uint64_t round_id, uint64_t&rate, uint64_t& time){
    rate_and_updated_time_t r(symbol_code(currency_key), round_id);
    if(!wasm::db::get(r)) return false;

    rate = r.rate;
    time = r.time;

    return true;
}

bool synthetix::_get_rate_and_updated_time(uint64_t currency_key, rate_and_updated_time_t& rate_and_updated_time){
    round_for_rate_t round(currency_key);
    if(!wasm::db::get(round)) return false;

    rate_and_updated_time_t rate(symbol_code(currency_key), round.round);
    if(!wasm::db::get(rate)) return false;

    rate_and_updated_time = rate;

    return true;

}

uint64_t synthetix::_last_rate_update_times(uint64_t currency_key){

    rate_and_updated_time_t rate_object;
    if(!_get_rate_and_updated_time(currency_key, rate_object)) return 0;

    return rate_object.time;
}

void synthetix::_internal_update_rates(const std::vector<symbol_code>& currency_keys, const std::vector<uint64_t>& new_rates, uint64_t time_sent){
    int i = 0;
    for(auto symbole_code_key: currency_keys){
        uint64_t currency_key = symbole_code_key.raw();
        check(new_rates[i] != 0, "Zero is not a valid rate, please call delete rate instead.");//fixme::add delete rate
        check(currency_key != XUSD, "Rate of xUSD cannot be updated, it's always UNIT.");

        if(time_sent < _last_rate_update_times(currency_key)){
            continue;
        }
        //new_rates[i] = rate_or_inverted(currency_key, new_rates[i])
        _set_rate(currency_key, new_rates[i], time_sent);

        i++;
    }
}

void synthetix::_last_rate_update_times_for_currencies(const std::vector<symbol_code>& currency_keys, std::vector<uint64_t> last_update_times){
    for(auto currency_key:currency_keys){
        last_update_times.push_back(_last_rate_update_times(currency_key.raw()));
    }
}

//oracle
regid synthetix::_get_oracle(){
    return regid(get_uint_value(KEY_ORACLE));
}

ACTION synthetix::setoracle(regid oracle){
    require_auth( get_self() );
    set_uint_value(KEY_ORACLE, oracle.value);
}

//set_rate_stale_period
ACTION synthetix::setperiod(uint64_t period){
    require_auth( get_self() );
    set_uint_value(KEY_RATE_STALE_PERIOD, period);
}

ACTION synthetix::updaterates(const std::vector<symbol_code>& currency_keys, const std::vector<uint64_t>& new_rates){
    require_auth( _get_oracle() );
    _internal_update_rates(currency_keys, new_rates, current_block_time());
}


//inverse 
//setInversePricing
// void synthetix::setinverse(
//     const name& currency_key,
//     uint64_t    entry_point,
//     uint64_t    upper_limit,
//     uint64_t    lower_limit,
//     bool        freeze,
//     bool        freeze_at_upper_limit)
// {

//     check(entryPoint > 0, "entryPoint must be above 0");
//     check(lowerLimit > 0, "lowerLimit must be above 0");
//     check(upperLimit > entryPoint,     "upperLimit must be above the entryPoint");
//     check(upperLimit < entryPoint * 2, "upperLimit must be less than double entryPoint");
//     check(lowerLimit < entryPoint,     "lowerLimit must be below the entryPoint");

//     require_auth(_get_oracle());

//     inverse_pricing_t inverse_pricing_object;
//     if(inverses.get(currency_key, inverse_pricing_object)){
//         inverses.modify(inverse_pricing_object,  wasm::no_payer, [&](auto &s) {
//             inverse_pricing_object.entry_point = entry_point;
//             inverse_pricing_object.upper_limit = upper_limit;  
//             inverse_pricing_object.lower_limit = lower_limit;
//             inverse_pricing_object.frozen      = freeze;  
//         });

//     } else {
//         inverses.emplace(inverse_pricing_object,  wasm::no_payer, [&](auto &s) {
//             inverse_pricing_object.currency_key = currency_key;
//             inverse_pricing_object.entry_point = entry_point;
//             inverse_pricing_object.upper_limit = upper_limit;  
//             inverse_pricing_object.lower_limit = lower_limit;
//             inverse_pricing_object.frozen      = freeze;  
//         });

//         table_index_t table_index_object;
//         if(table_indexs.get(KEY_INVERSE_KEYS, table_index_object)){
//             table_indexs.modify(table_index_object,  wasm::no_payer, [&](auto &s) {
//                 table_index_object.indexs[currency_key] = currency_key;
//             });
//          } else {
//              table_indexs.emplace(table_index_object,  wasm::no_payer, [&](auto &s) {
//                 table_index_object.indexs[currency_key] = currency_key;
//             });           
//          }
//     }

//     if(freeze){
//         _set_rate(currency_key, freeze_at_upper_limit?upper_limit:lower_limit, current_block_time())
//     }

// }

// void synthetix::delinverse(const name& currency_key)
// {
//     require_auth(_get_oracle());

//     table_index_t table_index_object;
//     check(table_indexs.get(KEY_INVERSE_KEYS, table_index_object), "Get table_index_object error");
//     check(table_index_object.indexs[curreny_key],"Inverse does not exist");


//     inverse_pricing_t inverse_pricing_object;
//     check(inverses.get(currency_key, inverse_pricing_object), "No inverted price exists");


//     table_indexs.modify(table_index_object,  wasm::no_payer, [&](auto &s) {
//         table_index_object.indexs.erase(curreny_key);
//     }); 

//     inverses.erase(inverse_pricing_object, wasm::no_payer);
// }





