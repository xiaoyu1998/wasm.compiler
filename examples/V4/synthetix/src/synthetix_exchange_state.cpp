#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>

#include <synthetix_database.hpp>

using namespace wasm;
using namespace wasm::db;

void synthetix::init_synthetix_exchange_state(){
    set_uint_value(KEY_MAX_ENTRIES_IN_QUEUE, INIT_MAX_ENTRIES_IN_QUEUE);
}

ACTION synthetix::setmaxqueue(uint64_t max_entries_in_queue){

    require_auth( get_self() );
    set_uint_value(KEY_MAX_ENTRIES_IN_QUEUE, max_entries_in_queue);
}


void synthetix::append_exchange_entry(
    uint64_t owner,
    uint64_t source_currency_key,     
    uint64_t source_amount,
    uint64_t destination_currency_key,
    uint64_t amount_received,
    uint64_t exchange_fee_rate,
    uint64_t time_tamp,
    uint64_t round_id_for_src,
    uint64_t round_id_for_dest){

    exchange_t exchange(owner, symbol_code(source_currency_key));
    wasm::db::get(exchange);

    check(exchange.entries.size() < get_max_entries_in_queue(), "Max queue length reached");
    exchange.entries.push_back(exchange_entry_t{
        symbol_code(source_currency_key),
        TO_ASSET(source_amount, source_currency_key),
        symbol_code(amount_received),
        TO_ASSET(amount_received, amount_received),
        exchange_fee_rate,
        time_tamp,
        round_id_for_src,
        round_id_for_dest
    });

    wasm::db::set(exchange);
}

void synthetix::remove_entries(uint64_t from, uint64_t currency_key){

    exchange_t exchange(from, symbol_code(currency_key));
    wasm::db::del(exchange);

}

uint64_t synthetix::get_max_time_stamp(regid account, uint64_t currency_key){

    // exchange_t exchange;
    exchange_t exchange(account, symbol_code(currency_key));
    wasm::db::get(exchange);

    uint64_t time_stamp = 0;
    for(int i = 0; i < exchange.entries.size(); i ++){
        if(exchange.entries[i].time_stamp > time_stamp){
            time_stamp = exchange.entries[i].time_stamp;
        }
    }

    return time_stamp;

}

uint64_t synthetix::get_max_entries_in_queue(){
    return get_uint_value(KEY_MAX_ENTRIES_IN_QUEUE);
}


