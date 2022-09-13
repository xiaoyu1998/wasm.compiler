#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_log.hpp>
#include <synthetix_database.hpp>


using namespace wasm;
using namespace synthetix;

void synthetix::init_synthetix_feepool(){
    require_auth( get_self() );

    set_uint_value(KEY_EXCHANGE_FEE_RATE,   INIT_EXCHANGE_FEE_RATE);
    set_uint_value(KEY_TARGET_THRESHOLD,    INIT_TARGET_THRESHOLD);
    set_uint_value(KEY_FEE_PERIOD_DURATION, INIT_RECENT_FEE_PERIODS);

    recent_fee_periods_t recent_fee_periods(KEY_RECENT_FEE_PERIODS);
    check(!wasm::db::get(recent_fee_periods), "Recent fee periods already exists! Can not init synthetix feepool");

    fee_period_t new_fee_period;
    new_fee_period.fee_period_id         = 1;
    new_fee_period.start_time            = current_block_time();
    // new_fee_period.fees_to_distribute    = 0;
    // new_fee_period.fees_claimed          = 0;
    // new_fee_period.rewards_to_distribute = 0;
    // new_fee_period.rewards_claimed       = 0;
    recent_fee_periods.fee_periods.push_back(new_fee_period);

    _set_recent_fee_periods(recent_fee_periods);

}

void synthetix::record_fee_paid( uint64_t amount){

    auto recent_fee_periods = _get_recent_fee_periods();
    recent_fee_periods.fee_periods[0].fees_to_distribute += amount;
    _set_recent_fee_periods(recent_fee_periods);
}

void synthetix::close_current_fee_period(){

    auto recent_fee_periods      = _get_recent_fee_periods();
    auto recent_fee_periods_size = recent_fee_periods.fee_periods.size();

    auto& first_fee_period       = recent_fee_periods.fee_periods[0];
    check( first_fee_period.start_time <= current_block_time() + _get_fee_period_duration(), "Too early to close fee period");

    fee_period_t new_fee_period;
    new_fee_period.fee_period_id         = first_fee_period.fee_period_id + 1;
    new_fee_period.start_time            = current_block_time(); 
    // new_fee_period.fees_to_distribute    = 0;
    // new_fee_period.fees_claimed          = 0;
    // new_fee_period.rewards_to_distribute = 0;
    // new_fee_period.rewards_claimed       = 0;
    recent_fee_periods.fee_periods.insert(recent_fee_periods.fee_periods.begin(), new_fee_period);  

    //merge state to the last second and remove the last period
    if(recent_fee_periods.fee_periods.size() > POOL_FEE_PERIOD_LENGTH){

        auto& second_last_fee_period = recent_fee_periods.fee_periods[POOL_FEE_PERIOD_LENGTH - 1];
        auto& last_fee_period        = recent_fee_periods.fee_periods[POOL_FEE_PERIOD_LENGTH];


        second_last_fee_period.fees_to_distribute    = last_fee_period.fees_to_distribute   -
                                                       last_fee_period.fees_claimed         +
                                                       second_last_fee_period.fees_to_distribute;
        second_last_fee_period.rewards_to_distribute = last_fee_period.rewards_to_distribute -
                                                       last_fee_period.rewards_claimed       +
                                                       second_last_fee_period.rewards_to_distribute;

        while(recent_fee_periods.fee_periods.size() > POOL_FEE_PERIOD_LENGTH)
            recent_fee_periods.fee_periods.pop_back();

    }

    _set_recent_fee_periods(recent_fee_periods);

}


void synthetix::feepool_append_account_issuance_record( regid account,
                                                      uint64_t debt_ratio,
                                                      uint64_t debt_entry_index){

    fee_period_t fee_period_object = _get_recent_fee_period(0);
    feepool_state_append_account_issuance_record(
        account.value,
        debt_ratio,
        debt_entry_index,
        fee_period_object.starting_debt_index);
}


void synthetix::_claim_fees(regid claiming_address){

    
    check( is_fees_claimable(claiming_address), "C-Ratio below penalty threshold");

    uint64_t available_fees, available_rewards;
    _fees_available(claiming_address, available_fees, available_rewards);

    _set_last_fee_withdrawal(claiming_address.value, _get_recent_fee_period(1).fee_period_id);

    if(available_fees > 0){
        uint64_t fees_paid = _record_fee_payment(available_fees);
        //_pay_fees(claiming_address.value, fees_paid);
    }

    // if(available_rewards > 0){
    //     uint64_t rewards_paid = _record_reward_payment(available_rewards);
    //     _pay_rewards(claiming_address, rewards_paid);
    // }
}

uint64_t synthetix::_fees_by_period(regid account, vector<uint64_t> &user_fees, vector<uint64_t> &user_rewards){

    uint64_t user_owner_ship_percentage, debt_entry_index;
    get_accounts_debt_entry(account.value, 0, user_owner_ship_percentage, debt_entry_index);

    // If they don't have any debt ownership and they never minted, they don't have any fees.
    // User ownership can reduce to 0 if user burns all synths,
    // however they could have fees applicable for periods they had minted in before so we check debtEntryIndex.
    if(debt_entry_index == 0 && user_owner_ship_percentage == 0) return 0;

    uint64_t fees_from_period, rewards_from_period;
    _fee_and_rewards_from_period(0, user_owner_ship_percentage, debt_entry_index, fees_from_period, rewards_from_period);

    // user_fees[0]    = fees_from_period;
    // user_rewards[0] = rewards_from_period;  
    
    user_fees.push_back(fees_from_period);
    user_rewards.push_back(rewards_from_period);

    uint64_t last_fee_withdrawal     = _get_last_fee_withdrawal(account.value);
    auto     recent_fee_periods_size = _get_recent_fee_periods().fee_periods.size();
    for(uint64_t i = recent_fee_periods_size - 1; i > 0 ; i-- ){

        uint64_t next = i - 1;
        uint64_t next_period_starting_debt_index = _get_recent_fee_period(next).starting_debt_index;

        // We can skip the period, as no debt minted during period (next period's startingDebtIndex is still 0)
        if(next_period_starting_debt_index > 0 && last_fee_withdrawal < _get_recent_fee_period(i).fee_period_id) {

            uint64_t closing_debt_index = next_period_starting_debt_index - 1;

            applicable_issuance_data(account.value, closing_debt_index, user_owner_ship_percentage, debt_entry_index);

            _fee_and_rewards_from_period(i, user_owner_ship_percentage, debt_entry_index, fees_from_period, rewards_from_period);

        }
        user_fees[i]    = fees_from_period;
        user_rewards[i] = rewards_from_period;
    }

    return user_fees.size();

}

void synthetix::_fee_and_rewards_from_period(
    uint64_t  period, 
    uint64_t  owner_ship_percentage, 
    uint64_t  debt_entry_index, 
    uint64_t& fees_from_period, 
    uint64_t& rewards_from_period){

    if(owner_ship_percentage == 0) {
        fees_from_period    = 0;
        rewards_from_period = 0;
        return;
    }

    uint64_t debt_owner_ship_for_period = owner_ship_percentage;

    if(period > 0){
        uint64_t closing_debt_index = _get_recent_fee_period(period - 1).starting_debt_index - 1;
        debt_owner_ship_for_period  = _effective_debt_ratio_for_period(closing_debt_index, owner_ship_percentage, debt_entry_index);
    }

    fees_from_period    = synthetix_divide_decimal(_get_recent_fee_period(period).fees_to_distribute, debt_owner_ship_for_period);
    rewards_from_period = synthetix_divide_decimal(_get_recent_fee_period(period).rewards_to_distribute, debt_owner_ship_for_period);
}

uint64_t synthetix::_effective_debt_ratio_for_period(uint64_t closing_debt_index, uint64_t owner_ship_percentage, uint64_t debt_entry_index){

    uint64_t fee_period_debt_owner_ship = synthetix_multiply_decimal(
                                              synthetix_divide_decimal(_debt_ledger(closing_debt_index), _debt_ledger(debt_entry_index)), 
                                              owner_ship_percentage);
    return fee_period_debt_owner_ship; 

}


void synthetix::_fees_available(regid account, uint64_t& available_fees, uint64_t& available_rewards){

    vector<uint64_t> user_fees, user_rewards;
    auto fees_and_rewards_size = _fees_by_period(account, user_fees, user_rewards);

    uint64_t total_fees    = 0;
    uint64_t total_rewards = 0;

    for(uint64_t i = 1; i < fees_and_rewards_size; i ++){
        total_fees    += user_fees[i];
        total_rewards += user_rewards[i];
    }

    available_fees    = total_fees;
    available_rewards = total_rewards;

}

uint64_t synthetix::_record_fee_payment(uint64_t xUSD_amount){

    uint64_t remaining_to_allocate = xUSD_amount;
    uint64_t fees_paid             = 0;

    auto recent_fee_periods        = _get_recent_fee_periods();
    auto recent_fee_periods_size   = _get_recent_fee_periods().fee_periods.size();

    //fixme:cannot claim while i == 0
    for(uint64_t i = recent_fee_periods_size - 1; i >= 0; i--){ // fixme:
        fee_period_t& fee_period = recent_fee_periods.fee_periods[i];//_get_recent_fee_periods_storage(i);

        uint64_t fees_already_claimed = fee_period.fees_claimed;
        uint64_t delta                = fee_period.rewards_to_distribute - fees_already_claimed;

        if(delta > 0){

            uint64_t amount_in_period = delta < remaining_to_allocate ? delta :remaining_to_allocate;
            fee_period.fees_claimed += amount_in_period;
            //_set_recent_fee_periods_storage(fee_period);

            remaining_to_allocate          -= amount_in_period;
            fees_paid                      += amount_in_period;

            if(remaining_to_allocate == 0) return fees_paid;

            //fixme:
            if( i == 0 && remaining_to_allocate > 0){
                remaining_to_allocate = 0;
            }
        }

    }

    _set_recent_fee_periods(recent_fee_periods);

    return fees_paid;

}

bool synthetix::is_fees_claimable(regid account){
    uint64_t ratio        = collateralisation_ratio(account);
    uint64_t target_ratio = issuance_ratio();

    if(ratio < target_ratio) return true;

    uint64_t ratio_threshold = synthetix_multiply_decimal(target_ratio, PRECISION_1 + _get_target_threshold());
    if(ratio > ratio_threshold) return false;

    return true;
}

fee_period_t synthetix::_get_recent_fee_period(uint64_t period){

    recent_fee_periods_t recent_fee_periods(KEY_RECENT_FEE_PERIODS);
    check(wasm::db::get(recent_fee_periods), "recent fee periods were missing");
    check(period < recent_fee_periods.fee_periods.size(), "Period is out of range");

    return recent_fee_periods.fee_periods[period];

}

void synthetix::_set_recent_fee_period(uint64_t period, const fee_period_t& fee_period){

    recent_fee_periods_t recent_fee_periods(KEY_RECENT_FEE_PERIODS);
    check(wasm::db::get(recent_fee_periods), "recent fee periods were missing");

    recent_fee_periods.fee_periods[period] = fee_period;
    wasm::db::set(recent_fee_periods);

}

recent_fee_periods_t synthetix::_get_recent_fee_periods(){

    recent_fee_periods_t recent_fee_periods(KEY_RECENT_FEE_PERIODS);
    check(wasm::db::get(recent_fee_periods), "recent fee periods were missing");
    return recent_fee_periods;

}

void synthetix::_set_recent_fee_periods(const recent_fee_periods_t& fee_periods){

    wasm::db::set(fee_periods);

}


void synthetix::_set_last_fee_withdrawal(uint64_t claiming_address, uint64_t fee_period_id){

    set_u128_value((uint128_t)KEY_LAST_FEE_WITHDRAWAL.value << 64 | (uint128_t)claiming_address, fee_period_id);

}

uint64_t synthetix::_get_last_fee_withdrawal(uint64_t claiming_address){

    //fixme:should be reject before set
    return get_u128_value((uint128_t)KEY_LAST_FEE_WITHDRAWAL.value << 64 | (uint128_t)claiming_address);
    
}

uint64_t synthetix::_get_fee_period_duration(){
    //fixme:should be reject before set;
    return get_uint_value(KEY_FEE_PERIOD_DURATION);
}

uint64_t synthetix::get_exchange_fee_rate(){
    //fixme:should be reject before set;
    return get_uint_value(KEY_EXCHANGE_FEE_RATE);
}

uint64_t synthetix::_get_target_threshold(){
    //fixme:should be reject before set;
    return get_uint_value(KEY_TARGET_THRESHOLD);
}

ACTION synthetix::claimfees(regid claiming_address){

    require_auth(claiming_address);
    _claim_fees(claiming_address);

}

ACTION synthetix::setfeerate(uint64_t exchange_fee_rate)  {

    require_auth( get_self() );

    check( exchange_fee_rate >= MAX_EXCHANGE_FEE_RATE, "rate < MAX_EXCHANGE_FEE_RATE");
    set_uint_value(KEY_EXCHANGE_FEE_RATE, exchange_fee_rate);

}

ACTION synthetix::setfeeperiod(uint64_t fee_period_duration)  {

    require_auth( get_self() );

    check( fee_period_duration >= MIN_FEE_PERIOD_DURATION, "value < MIN_FEE_PERIOD_DURATION");
    check( fee_period_duration <= MAX_FEE_PERIOD_DURATION, "value > MAX_FEE_PERIOD_DURATION");
    set_uint_value(KEY_FEE_PERIOD_DURATION, fee_period_duration);

}

ACTION synthetix::setthreshold(uint64_t target_threshold_percent)  {

    require_auth( get_self() );

    check( target_threshold_percent >= 0, "Threshold should be positive");
    check( target_threshold_percent <= 50, "Threshold too high");
    set_uint_value(KEY_TARGET_THRESHOLD, synthetix_divide_decimal(target_threshold_percent, 100));

}

ACTION synthetix::appendreward( uint64_t amount){

    //fixme:should be from reward escrow contract 
    require_auth( get_self() );

    fee_period_t fee_period_object           = _get_recent_fee_period(0);
    fee_period_object.rewards_to_distribute += amount;

    _set_recent_fee_period(0, fee_period_object);
}
