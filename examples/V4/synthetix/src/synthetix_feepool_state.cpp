#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_log.hpp>
#include <synthetix_database.hpp>


using namespace wasm;
using namespace wayki;



void synthetix::get_accounts_debt_entry(uint64_t account, uint64_t index, uint64_t& debt_percentage, uint64_t& debt_entry_index){

    check(index < ACCOUNT_FEE_PERIOD_LENGTH, "index exceeds the FEE_PERIOD_LENGTH");

    //account_issuance_ledger_t account_issuance_ledger(regid(account));
    account_issuance_ledger_t account_issuance_ledger(account);
    if( !wasm::db::get(account_issuance_ledger)) return;

    check(index < account_issuance_ledger.issuance_data.size(), "index exceeds the ledger size"); 

    debt_percentage  = account_issuance_ledger.issuance_data[index].debt_percentage;
    debt_entry_index = account_issuance_ledger.issuance_data[index].debt_entry_index;

}

void synthetix::applicable_issuance_data( uint64_t  account, 
                                       uint64_t  closing_debt_index, 
                                       uint64_t& owner_ship_percentage, 
                                       uint64_t& debt_entry_index){

    account_issuance_ledger_t account_issuance_ledger(account);
    if( !wasm::db::get(account_issuance_ledger)) return;

    for(auto issuance: account_issuance_ledger.issuance_data){

        if(closing_debt_index >= issuance.debt_entry_index){
            owner_ship_percentage = issuance.debt_percentage;
            debt_entry_index      = issuance.debt_entry_index;             
        }
    }

    return;
}

void synthetix::feepool_state_append_account_issuance_record( uint64_t account,
                                                           uint64_t debt_ratio,
                                                           uint64_t debt_entry_index,
                                                           uint64_t current_period_start_debt_index) {

    account_issuance_ledger_t account_issuance_ledger(account);
    auto empty = !wasm::db::get(account_issuance_ledger);
    if(empty) {
        account_issuance_ledger.owner = regid(account);
    }


    account_issuance_ledger.issuance_data.insert(account_issuance_ledger.issuance_data.begin(), {debt_ratio, debt_entry_index});
    while(account_issuance_ledger.issuance_data.size() > ACCOUNT_FEE_PERIOD_LENGTH)
        account_issuance_ledger.issuance_data.pop_back();

    wasm::db::set(account_issuance_ledger);

}
                                            