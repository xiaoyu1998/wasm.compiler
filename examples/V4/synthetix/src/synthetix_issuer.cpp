#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_log.hpp>


using namespace wasm;
using namespace synthetix;

void synthetix::init_synthetix_issuer()  {
    set_uint_value(KEY_MINIMUM_STAKE_TIME, INIT_MINIMUM_STAKING_TIME);
}

uint64_t synthetix::_last_issue_event(regid account){
    return get_u128_value((uint128_t)KEY_LAST_ISSUE_EVENT.value << 64 | account.value);
}

void synthetix::_set_last_issue_event(regid account){
    set_u128_value((uint128_t)KEY_LAST_ISSUE_EVENT.value << 64 | account.value, current_block_time());
}

bool synthetix::_can_burn_mpas(regid account){
     return current_block_time() >= _last_issue_event(account) + get_uint_value(KEY_MINIMUM_STAKE_TIME);
}


void synthetix::_internal_Issue_mpas(
    regid from,
    uint64_t amount,
    uint64_t existing_debt,
    uint64_t total_system_debt){

    _add_to_debt_register(from , amount, existing_debt, total_system_debt);

    // issue xUSD
    _set_last_issue_event(from);
    asset quantity(amount, symbol(symbol_code(XUSD), PRECISION));

    issue_mpa(from, quantity);

    // //notify bank to mint xusd
    // regid issuer_bank = _get_mpa_issuer_bank(XUSD);
    // wasm::transaction inline_trx(issuer_bank, name("issue"), std::vector<permission>{{get_self(), name("wasmio.code")}}, std::tuple<regid,asset,string>(from, quantity, "issue"));
    // inline_trx.send(); 

    // WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
    //                  " issue trx:", inline_trx, "\n")   

    _append_account_issuance_record(from);

}

void synthetix::_burnmpas(regid from, uint64_t amount) {

    check(_can_burn_mpas(from), "Minimum stake time not reached");

    // uint64_t reclaimed, refunded;
    // exchanger_settle(from, XUSD, reclaimed, refunded);

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " from:", from, 
                     " amount_to_burn:", amount, "\n")  


    uint64_t existing_debt, total_system_debt;
    debt_balance_of_and_total_debt(from, XUSD, existing_debt, total_system_debt);   
    check(existing_debt > 0, "No debt to forgive");

    //uint64_t debt_to_remove_after_settlement = exchanger_calculate_amount_after_settlement(from, XUSD, amount ,refound);

    //_internal_burn_mpa(from, debt_to_remove_after_settlement, existing_debt, total_system_debt);
    _internal_burn_mpas(from, amount, existing_debt, total_system_debt);

}

// Burns your XUSD to the target c-ratio so you can claim fees
void synthetix::_burn_mpa_to_target(regid from) {

    uint64_t existing_debt, total_system_debt;
    debt_balance_of_and_total_debt(from, XUSD, existing_debt, total_system_debt);  
    check(existing_debt > 0, "No debt to forgive");
    
    uint64_t max_issuable             = max_issuable_mpas(from); 
    uint64_t amount_to_burn_to_target = existing_debt > max_issuable ? existing_debt - max_issuable :0;
       
    _internal_burn_mpas(from, amount_to_burn_to_target, existing_debt, total_system_debt);
}

void synthetix::_internal_burn_mpas(
    regid from,
    uint64_t amount,
    uint64_t existing_debt,
    uint64_t total_system_debt){

    uint64_t amount_to_remove = existing_debt < amount ? existing_debt : amount;
    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " amount_to_burn:", amount, 
                     " existing_debt:", existing_debt,   
                     " amount_to_remove:", amount_to_remove, 
                     " total_system_debt:", total_system_debt,"\n")   

    _remove_from_debt_register(from, amount_to_remove, existing_debt, total_system_debt);
    uint64_t amount_to_burn = amount_to_remove;

    asset quantity(amount_to_burn, symbol(symbol_code(XUSD), PRECISION));
    burn_mpa(from, quantity);

    //notify bank to burn
    // regid issuer_bank = _get_mpa_issuer_bank(XUSD);
    // wasm::transaction inline_trx(issuer_bank, name("burn"), std::vector<permission>{{get_self(), name("wasmio.code")}}, std::tuple<regid,asset,string>(from, quantity, "burn"));
    // inline_trx.send(); 

    // WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
    //                  " burn trx:", inline_trx, "\n") 

    _append_account_issuance_record(from);
}

void synthetix::_append_account_issuance_record(regid from){

    uint64_t initial_debt_owner_ship, debt_entry_index;
    issuance_data(from, initial_debt_owner_ship, debt_entry_index);

    feepool_append_account_issuance_record(from ,initial_debt_owner_ship, debt_entry_index);

}

void synthetix::_add_to_debt_register(
    regid    from, 
    uint64_t amount, 
    uint64_t existing_debt, 
    uint64_t total_debt_issued){

    check(amount > 0, "Can not add amount <= 0 to debt ledger, the account should have enough XT");

    uint64_t new_total_debt_issued = amount + total_debt_issued;
    uint64_t debt_percentage       = synthetix_divide_decimal(amount, new_total_debt_issued);
    uint64_t delta                 = PRECISION_1 - debt_percentage;//why add 1

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " amount_to_issue:", amount,
                     " existing_debt:", existing_debt,  
                     " new_total_debt_issued:", new_total_debt_issued, 
                     " debt_percentage  :", debt_percentage,
                     " pre_total_debt_issued:", total_debt_issued,"\n")  

    if(existing_debt > 0){
        debt_percentage = synthetix_divide_decimal(amount + existing_debt, new_total_debt_issued);

        WASM_LOG_PRINT(  SYNTHETIX_DEBUG, 
                         " amount_to_issue:", amount, 
                         " existing_debt:", existing_debt, 
                         " new_total_debt_issued:", new_total_debt_issued,  
                         " debt_percentage_e:", debt_percentage,"\n")  
    }

    _set_current_issuance_data(from, debt_percentage);

    uint64_t  debt_ledger_length = get_uint_value(KEY_DEBT_LEDGER_LENGTH);
    //fixme:
    //if(debt_ledger_length > 0 && total_debt_issued > 0){
    if(debt_ledger_length > 0 && total_debt_issued > PRECISION_1){
        WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                         " delta:", delta, 
                         " pre_last_debt_ledger_entry:", _last_debt_ledger_entry(), 
                         " new_last_debt_ledger_entry:", synthetix_multiply_decimal(_last_debt_ledger_entry(), delta)) 

        _append_debt_ledger_value(synthetix_multiply_decimal(_last_debt_ledger_entry(), delta));
    }else{
        WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                         " delta:", delta, 
                         " new_last_debt_ledger_entry:", PRECISION_1) 
        _append_debt_ledger_value(PRECISION_1);
    } 

}

void synthetix::_remove_from_debt_register(
    regid    from, 
    uint64_t amount, 
    uint64_t existing_debt, 
    uint64_t total_debt_issued){

    check(amount > 0, "Can not add amount <= 0 to debt ledger");

    uint64_t debt_to_remove        = amount;
    uint64_t new_total_debt_issued = total_debt_issued - debt_to_remove;

    uint64_t delta = 0;

    //fixme:to clear debt new_total_debt_issued should be 0
    //if( new_total_debt_issued > 0){
    if( new_total_debt_issued > PRECISION_1){
        uint64_t debt_percentage = synthetix_divide_decimal(debt_to_remove, new_total_debt_issued);
        delta                    = PRECISION_1 + debt_percentage;

        WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                         " debt_to_remove:", debt_to_remove, 
                         " pre_total_debt_issued:", total_debt_issued,
                         " new_total_debt_issued:", new_total_debt_issued, 
                         " debt_percentage:", debt_percentage, "\n") 
    }

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " debt_to_remove:", debt_to_remove, 
                     " pre_total_debt_issued:", total_debt_issued,
                     " new_total_debt_issued:", new_total_debt_issued, 
                     " existing_debt:", existing_debt,  "\n")

    if(debt_to_remove == existing_debt){
        _set_current_issuance_data(from, 0);
        //_decrement_total_issuer_count();
    }else{
        uint64_t new_debt = existing_debt - debt_to_remove;
        uint64_t new_debt_percentage = synthetix_divide_decimal(new_debt, new_total_debt_issued);

        _set_current_issuance_data(from, new_debt_percentage);
    }

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " delta:", delta, 
                     " pre_last_debt_ledger_entry:", _last_debt_ledger_entry(), 
                     " new_last_debt_ledger_entry:", synthetix_multiply_decimal(_last_debt_ledger_entry(), delta)) 

  
    _append_debt_ledger_value(synthetix_multiply_decimal(_last_debt_ledger_entry(), delta));

}

ACTION synthetix::setstaketime(uint64_t seconds)  {

    require_auth( get_self() );

    check( seconds <= MAX_MINIMUM_STAKING_TIME, "Stake time exceed maximum 1 week");
    set_uint_value(KEY_MINIMUM_STAKE_TIME, seconds);

}

ACTION synthetix::issuempas(regid from, uint64_t amount) {

    require_auth( from );

    uint64_t max_issuable, existing_debt, total_system_debt;
    remaining_issuable_mpas(from, max_issuable, existing_debt, total_system_debt);
    check(amount <= max_issuable, "Amount too large");
    _internal_Issue_mpas(from, amount, existing_debt, total_system_debt);  
}

ACTION synthetix::issuemaxmpas(regid from) {

    require_auth( from );

    WASM_LOG_PRINT( SYNTHETIX_DEBUG,
                    " from:", from,  "\n")  

    uint64_t max_issuable, existing_debt, total_system_debt;
    remaining_issuable_mpas(from, max_issuable, existing_debt, total_system_debt);
    _internal_Issue_mpas(from, max_issuable, existing_debt, total_system_debt); 
}

ACTION synthetix::burnmpas(regid from, asset quantity){

    require_auth( from );

    //_burn_mpa_to_target(from);
    check(quantity.symbol.code().raw() == XUSD, "must burn xUSD");
    check(quantity.symbol.precision()  == PRECISION, "precision must 8");

    _burnmpas(from, quantity.amount);

}


ACTION synthetix::burnmpas2t(regid from){

    require_auth( from );

    _burn_mpa_to_target(from);

}

