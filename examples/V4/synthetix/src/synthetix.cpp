#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_log.hpp>

#include <synthetix_database.hpp>

using namespace wasm;
using namespace synthetix;


uint64_t synthetix::_debt_balance_of(
    regid issuer, 
    uint64_t currency_key){

    uint64_t initial_debt_owner_ship, debt_entry_index;
    issuance_data(issuer, initial_debt_owner_ship, debt_entry_index);

    if (initial_debt_owner_ship == 0) return 0;

    uint64_t debt_balance, total_system_debt;
    debt_balance_of_and_total_debt(issuer, currency_key, debt_balance, total_system_debt);
    return debt_balance;
}

void synthetix::debt_balance_of_and_total_debt(
    regid issuer, 
    uint64_t currency_key, 
    uint64_t& debt_balance, 
    uint64_t& total_system_debt){

    uint64_t initial_debt_owner_ship, debt_entry_index;
    issuance_data(issuer, initial_debt_owner_ship, debt_entry_index); 

    //fixme
    //print(symbol_code(currency_key));
    asset total_system_asset(0, symbol(symbol_code(currency_key), PRECISION));
    _totoal_issue_mpas(true, total_system_asset);
    total_system_debt = total_system_asset.amount;

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " initial_debt_owner_ship:", initial_debt_owner_ship, 
                     " debt_entry_index:", debt_entry_index, 
                     " total_system_debt:", total_system_debt, "\n") 

    if (initial_debt_owner_ship == 0){
        debt_balance = 0;
        return;
    }

    uint128_t current_debt_owner_ship = synthetix_multiply_decimal(synthetix_divide_decimal(_last_debt_ledger_entry(), _debt_ledger(debt_entry_index)), initial_debt_owner_ship);
    uint128_t high_precision_balance  = synthetix_multiply_decimal(total_system_debt, current_debt_owner_ship);

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " current_debt_owner_ship:", current_debt_owner_ship, 
                     " current_debt_balance:", high_precision_balance, "\n")     

    debt_balance = (uint64_t)high_precision_balance;
}

uint64_t synthetix::_collateral(regid account){
    return _token_state_balance_of(account, XT);
}


uint64_t synthetix::collateralisation_ratio(regid issuer){
    uint64_t total_owned_xt = _collateral(issuer);
    if(total_owned_xt == 0) return 0;

    uint64_t debt_balance = _debt_balance_of(issuer, XT);
    return synthetix_divide_decimal(debt_balance, total_owned_xt);
}

uint64_t synthetix::max_issuable_mpas(regid issuer){

    uint64_t destination_value = effective_value(XT, _collateral(issuer), XUSD);

    int128_t tmp = synthetix_divide_decimal(destination_value, issuance_ratio());

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " max_issuable   :", tmp,
                     " collateral:", destination_value,
                     " issuance_ratio:", issuance_ratio(), "\n")  

    return (uint64_t)tmp;
}

void synthetix::remaining_issuable_mpas(
    regid issuer,
    uint64_t& max_issuable, 
    uint64_t& already_issued, 
    uint64_t& total_system_debt){

    debt_balance_of_and_total_debt(issuer, XUSD, already_issued, total_system_debt);
    max_issuable = max_issuable_mpas(issuer);

    if(already_issued >= max_issuable) 
        max_issuable  = 0;
    else
        max_issuable -= already_issued; 

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     " existing_debt  :", already_issued, 
                     " total_system_debt:", total_system_debt,
                     " max_issuable:", max_issuable, "\n")  
}

bool synthetix::_available_currency_keys(map<uint64_t,symbol_code>& currency_keys) 
{
    mpas_index_t mpas_index(KEY_SYNTHETIX_MPAS);
    if(!wasm::db::get(mpas_index)) return false;

    currency_keys = mpas_index.indexs;
    return true;
}

uint64_t synthetix::_available_mpa_count() 
{
    mpas_index_t mpas_index(KEY_SYNTHETIX_MPAS);
    if(!wasm::db::get(mpas_index)) return 0;    
    
    return mpas_index.indexs.size();

}

bool synthetix::_totoal_issue_mpas(bool exclude_ether_collateral, asset& total_debt)
{
    uint64_t currency_key  = total_debt.symbol.code().raw();
    uint64_t currency_rate = get_rate(currency_key);

    map<uint64_t, symbol_code> currency_keys;
    check(_available_currency_keys(currency_keys), "Get available_currency_keys error");

    uint128_t total = 0;

    for(auto key : currency_keys){

        //fixme::should check xt
        if(currency_key == XT) continue;

        mpa_t mpa(key.first);
        //check(mpa_table.get(mpa),"Get mpa error");  
        check(wasm::db::get(mpa), "Get mpa error"); 

        if(exclude_ether_collateral &&  currency_key == XETH ) {
            total -= synthetix_multiply_decimal(mpa.total_supply.amount , get_rate(key.first));
        }

        total += synthetix_multiply_decimal(mpa.total_supply.amount , get_rate(key.first));
    }

    total_debt.amount = synthetix_divide_decimal(total, currency_rate) ;

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     "mpas_size:", currency_keys.size(), "\n" ) 

    //fixme:why?
    if(total_debt.amount < PRECISION_1) total_debt.amount = 0;
    return true;
}

bool synthetix::existing_mpa(uint64_t currency_key){

    mpas_index_t mpas_index(KEY_SYNTHETIX_MPAS);
    check(wasm::db::get(mpas_index), "Get synthetix.mpas error");

    auto iter   = mpas_index.indexs.find(currency_key);
    if(iter != mpas_index.indexs.end()) return true;
    return false;

}

regid synthetix::get_mpa_issuer_bank(uint64_t currency_key){

    mpa_t mpa(currency_key);
    check(wasm::db::get(mpa), "Mpa does not exist");
    return mpa.issuer; 
}

ACTION synthetix::totalmpas() {
    asset total_debt = asset::from_string("1.00000000 xUSD");
    _totoal_issue_mpas(false, total_debt);

    WASM_LOG_PRINT(  SYNTHETIX_DEBUG,
                     "total_debtx:", total_debt )  
}

ACTION synthetix::addmpa(mpa_t mpa) 
{
    require_auth( get_self() );

    symbol_code      code         = mpa.total_supply.symbol.code();
    uint64_t         currency_key = code.raw();

    mpas_index_t mpas_index(KEY_SYNTHETIX_MPAS);
    if(wasm::db::get(mpas_index)){
        check(mpas_index.indexs.size() <= MAX_MPAS_SIZE, "The number of mpa is out of limited");
        auto iter   = mpas_index.indexs.find(currency_key);
        check(iter == mpas_index.indexs.end(), "Mpa already exists");     
    } 
    mpas_index.indexs[currency_key] = code;    
    wasm::db::set(mpas_index);

    mpa_t mpa_tmp(currency_key);
    check(!wasm::db::get(mpa_tmp), "Mpa already exists");
    wasm::db::set(mpa);

}

ACTION synthetix::delmpa(symbol_code code) 
{
    require_auth( get_self() );

    uint64_t currency_key = code.raw();

    mpas_index_t mpas_index(KEY_SYNTHETIX_MPAS);
    check(wasm::db::get(mpas_index), "Get synthetix.mpas error");
    auto iter   = mpas_index.indexs.find(currency_key);
    check(iter != mpas_index.indexs.end(), "Mpa does not exist");
    mpas_index.indexs.erase(currency_key);
    wasm::db::set(mpas_index);

    mpa_t mpa(currency_key);
    check( wasm::db::get(mpa), "Mpa does not exist");   
    wasm::db::del(mpa);
}


ACTION synthetix::initsynthetix() {

    require_auth( get_self() );

    //add xusd
    mpas_index_t mpas_index(KEY_SYNTHETIX_MPAS);
    mpas_index.indexs[XUSD] = symbol_code(XUSD);    
    wasm::db::set(mpas_index);
    mpa_t mpa(get_self().value, get_self().value, XUSD);
    wasm::db::set(mpa);

    init_synthetix_state();
    init_synthetix_exchanger();
    init_synthetix_exchange_rates();
    init_synthetix_exchange_state();
    init_synthetix_feepool();
    init_synthetix_token_state();
    init_synthetix_issuer();

}

WASM_DISPATCH( synthetix, (initsynthetix)(addmpa)(delmpa)(totalmpas)
                       (setexenabled)(setwaitsecs)(exchange)(settle)
                       (setmaxqueue)
                       (setfeerate)(setfeeperiod)(setthreshold)(claimfees)(appendreward)
                       (setoracle)(setperiod)(updaterates)
                       (setstaketime)(issuempas)(issuemaxmpas)(burnmpas)(burnmpas2t)
                       (setissuratio)
                       (create)(issue)(transfer))




