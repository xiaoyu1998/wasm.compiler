#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_database.hpp>

using namespace wasm;
using namespace wasm::db;

void synthetix::init_synthetix_exchanger(){
    set_uint_value(KEY_WAITING_PERIOD_SECS, INIT_WAITING_PERIOD_SECS);
    set_uint_value(KEY_EXCHANGE_ENABLED, INIT_EXCHANGE_ENABLED);
}

uint64_t synthetix::_max_secs_left_in_waiting_period(regid account, uint64_t currency_key){
    return _secs_left_in_waiting_period_for_exchange(get_max_time_stamp(account, currency_key));
}

uint64_t synthetix::_fee_rate_for_exchange(uint64_t source_currency_key, uint64_t destination_currency_key){
    uint64_t exchange_fee_rate = get_exchange_fee_rate();
    uint64_t multiplier = 1;

    //fixme:do some case 
    // // Is this a swing trade? I.e. long to short or vice versa, excluding when going into or out of sUSD.
    // // Note: this assumes shorts begin with 'i' and longs with 's'.
    // if (
    //     (sourceCurrencyKey[0] == 0x73 && sourceCurrencyKey != sUSD && destinationCurrencyKey[0] == 0x69) ||
    //     (sourceCurrencyKey[0] == 0x69 && destinationCurrencyKey != sUSD && destinationCurrencyKey[0] == 0x73)
    // ) {
    //     // If so then double the exchange fee multipler
    //     multiplier = 2;
    // }

    return exchange_fee_rate * multiplier;

}

void synthetix::settlement_owing(uint64_t account, uint64_t currency_key, uint64_t& reclaim_amount, uint64_t& rebat_amount){

    exchange_t exchange(account, symbol_code(currency_key));
    //exchange_t exchange(regid(account), symbol_code(currency_key));
    wasm::db::get(exchange);

    for(auto e: exchange.entries){

        auto source_currency_key      = e.src.raw();
        auto amount                   = e.amount.amount;
        auto destination_currency_key = e.dest.raw();
        auto amount_received          = e.amount_received.amount;

        uint64_t src_round_id_at_period_end, dest_round_id_at_period_end;
        get_round_ids_at_period_end(e, src_round_id_at_period_end, dest_round_id_at_period_end);

        uint64_t destination_amount = effective_value_at_round(
            source_currency_key,
            amount,
            destination_currency_key,
            src_round_id_at_period_end,
            dest_round_id_at_period_end
        );

        uint64_t amount_should_have_received, fee;
        calculate_exchange_amount_minus_fees(source_currency_key, destination_currency_key, destination_amount, amount_should_have_received, fee);

        if(amount_received > amount_should_have_received){
            reclaim_amount = reclaim_amount + amount_received - amount_should_have_received;
        } else if(amount_should_have_received > amount_received ){
            rebat_amount = rebat_amount + amount_should_have_received - amount_received;
        }
    }
    
}

void synthetix::calculate_amount_after_settlement(
    regid     from,
    uint64_t  currency_key,
    uint64_t  amount,
    uint64_t  refunded,
    uint64_t& amount_after_settlement){

    amount_after_settlement = amount;

    //fixme::burn would be reject if their balance is not sufficient
    // uint64_t balance_of_source_after_settlement = bank.balance_of(from, currency_key);

    // if(amount_after_settlement > balance_of_source_after_settlement)
    //     amount_after_settlement = balance_of_source_after_settlement;


    if(refunded > 0)
        amount_after_settlement += refunded;

}

void synthetix::calculate_exchange_amount_minus_fees(
    uint64_t  source_currency_key,
    uint64_t  destination_currency_key,
    uint64_t  destination_amount,
    uint64_t& amount_received,
    uint64_t& fee){

    amount_received = destination_amount;

    uint64_t exchange_fee_rate = _fee_rate_for_exchange(source_currency_key, destination_currency_key);

    amount_received = synthetix_multiply_decimal(destination_amount, PRECISION_1 - exchange_fee_rate);
    fee             = destination_amount - amount_received; 

}

void synthetix::_append_exchange(
    regid       destination_address,
    symbol_code source_currency_key,
    asset       amount,
    symbol_code destination_currency_key,
    asset       amount_received){

    check(amount.symbol.precision()          == PRECISION, "precision is not match");
    check(amount_received.symbol.precision() == PRECISION, "precision is not match");

    uint64_t exchange_fee_rate  = get_exchange_fee_rate();
    uint64_t round_id_for_src   = get_current_round_id(source_currency_key.raw());
    uint64_t round_id_for_dest  = get_current_round_id(destination_currency_key.raw());

    append_exchange_entry(
        destination_address.value,
        source_currency_key.raw(),
        amount.amount,
        destination_currency_key.raw(),
        amount_received.amount,
        exchange_fee_rate,
        current_block_time(),
        round_id_for_src,
        round_id_for_dest
    );


}

void synthetix::_internal_settle(regid from, uint64_t currency_key, uint64_t reclaimed, uint64_t refunded){

    check(_max_secs_left_in_waiting_period(from, currency_key) == 0, "Cannot settle during waiting period");

    uint64_t reclaim_amount, rebate_amount;
    settlement_owing(from.value, currency_key, reclaim_amount, rebate_amount);

    if(reclaim_amount > rebate_amount){
        reclaimed = reclaim_amount - rebate_amount;
        reclaim(from, currency_key, reclaimed);
    }else if(rebate_amount > reclaim_amount){
        refunded = rebate_amount - reclaim_amount;
        refund(from, currency_key, refunded);
    }

    remove_entries(from.value, currency_key);
}

//fixme::should check
void synthetix::reclaim(regid from, uint64_t currency_key, uint64_t amount){
    asset quantity(amount, symbol(symbol_code(currency_key), PRECISION));
    burn_mpa(from , quantity);
}

void synthetix::refund(regid from, uint64_t currency_key, uint64_t amount){
    asset quantity(amount, symbol(symbol_code(currency_key), PRECISION));
    issue_mpa(from , quantity);
}

void synthetix::get_round_ids_at_period_end(
    const exchange_entry_t &e, 
    uint64_t& src_round_id_at_period_end, 
    uint64_t& dest_round_id_at_period_end){

    auto source_currency_key      = e.src;
    auto destination_currency_key = e.dest;
    auto time_stamp               = e.time_stamp;
    auto round_id_for_src         = e.round_id_for_src;
    auto round_id_for_dest        = e.round_id_for_dest;

    src_round_id_at_period_end  = get_last_round_id_before_elapsed_secs(source_currency_key.raw(), round_id_for_src, time_stamp, _get_waiting_period_secs());
    dest_round_id_at_period_end = get_last_round_id_before_elapsed_secs(destination_currency_key.raw(), round_id_for_dest, time_stamp, _get_waiting_period_secs());

}

uint64_t synthetix::_secs_left_in_waiting_period_for_exchange(uint64_t time_stamp){

    uint64_t waiting_period_secs = _get_waiting_period_secs();
    if(time_stamp == 0 || current_block_time() > time_stamp + waiting_period_secs ){
        return 0;
    }
    return time_stamp + waiting_period_secs - current_block_time();
}

void synthetix::_remit_fee(uint64_t fee, uint64_t currency_key){

    uint64_t usd_fee_amount = effective_value(currency_key, fee, XUSD);

    // regid feepool = get_self();
    // notify_bank_to_issue_mpa(feepool, usd_fee_amount);

    record_fee_paid(usd_fee_amount);
}

uint64_t synthetix::_get_waiting_period_secs(){
    return get_uint_value(KEY_WAITING_PERIOD_SECS);
}

bool synthetix::_get_exchange_enabled(){
    return get_uint_value(KEY_EXCHANGE_ENABLED);
}

ACTION synthetix::setexenabled(bool exchange_enabled){
    require_auth( get_self() );
    set_uint_value(KEY_EXCHANGE_ENABLED, exchange_enabled);
}

ACTION synthetix::setwaitsecs(uint64_t waiting_period_secs){
    require_auth( get_self() );
    set_uint_value(KEY_WAITING_PERIOD_SECS, waiting_period_secs);
}

ACTION synthetix::exchange(
    regid       from,
    symbol_code source_currency_key,     
    uint64_t    source_amount,
    symbol_code destination_currency_key,
    regid       destination_address){


    check(source_currency_key != destination_currency_key, "Can't be same mpa");
    check(source_amount > 0, "Zero amount");
    check(_get_exchange_enabled(), "Exchanging is disabled");

    require_auth( from );

    uint64_t reclaimed, refunded;
    _internal_settle(from, source_currency_key.raw(), reclaimed, refunded);

    uint64_t source_amount_after_settlement;
    calculate_amount_after_settlement(from, source_currency_key.raw(), source_amount, refunded, source_amount_after_settlement);

    // Note: We don't need to check their balance as the burn() below will do a safe subtraction which requires
    // the subtraction to not overflow, which would happen if their balance is not sufficient.
    //burn_mpa(from, source_amount_after_settlement);

    uint64_t destination_amount = effective_value(
        source_currency_key.raw(),
        source_amount_after_settlement,
        destination_currency_key.raw()
    );

    uint64_t amount_received, fee;
    calculate_exchange_amount_minus_fees(
        source_currency_key.raw(),
        destination_currency_key.raw(),
        destination_amount,
        amount_received,
        fee
    );

    //issue_mpa(destination_address, amount_received);

    // if( fee > 0)
    //     _remit_fee(fee, destination_currency_key);

    notify_recipient( from );
    notify_recipient( destination_address );

    // persist the exchange information for the dest key
    _append_exchange(
        destination_address,
        source_currency_key,
        //asset(source_amount_after_settlement, symbol(destination_currency_key,PRECISION)),
        TO_ASSET(source_amount_after_settlement, destination_currency_key),
        destination_currency_key,
        TO_ASSET(amount_received, destination_currency_key)
        //asset(amount_received, symbol(destination_currency_key, PRECISION))
    );
}

ACTION synthetix::settle(regid from, symbol_code currency_key){
    uint64_t reclaimed, refunded;    
    return _internal_settle(from, currency_key.raw(), reclaimed, refunded);
}



