#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_math.hpp>
#include <synthetix_database.hpp>

using namespace wasm;
using namespace wayki;

void synthetix::init_synthetix_state(){
    return set_uint_value(KEY_ISSUANCE_RATIO, INIT_ISSUANCE_RATIO);
}

void synthetix::issuance_data(regid issuer, uint64_t& initial_debt_owner_ship, uint64_t& debt_entry_index){

	// issuance_data_t object;
 //    issuance_data   issuance_data_table(_self, SYNTHETIX_SCOPE);

 //    // check( issuance_data_table.get( object, issuer.value ), "issue does not exists" );
 //    if(!issuance_data_table.get( object, issuer.value )){
 //    	initial_debt_owner_ship = 0;
 //        debt_entry_index        = 0;
 //    	return;
 //    }

 //    initial_debt_owner_ship = object.initial_debt_owner_ship;
 //    debt_entry_index        = object.debt_entry_index;

    issuance_data_t issuance(issuer);
    if(!wasm::db::get(issuance)){
        initial_debt_owner_ship = 0;
        debt_entry_index        = 0;
        return;
    }

    initial_debt_owner_ship = issuance.initial_debt_owner_ship;
    debt_entry_index        = issuance.debt_entry_index;


}

void synthetix::_set_current_issuance_data(regid issuer, uint64_t initial_debt_owner_ship){

	// uint64_t  debt_ledger_length = get_uint_value(KEY_DEBT_LEDGER_LENGTH);

	// issuance_data_t object;
 //    issuance_data   issuance_data_table(_self, SYNTHETIX_SCOPE);
 //    if(issuance_data_table.get( object,  issuer.value)){
 //    	issuance_data_table.modify( object, wasm::no_payer, [&]( auto& s ) {
 //           s.initial_debt_owner_ship = initial_debt_owner_ship;
 //           s.debt_entry_index        = debt_ledger_length;
 //        }); 

 //    }else{
 //        issuance_data_table.emplace( wasm::no_payer, [&]( auto& s ) {
 //           s.owner                   = issuer;
 //           s.initial_debt_owner_ship = initial_debt_owner_ship;
 //           s.debt_entry_index        = debt_ledger_length;
 //        });
 //    }
    uint64_t  debt_ledger_length = get_uint_value(KEY_DEBT_LEDGER_LENGTH);
    issuance_data_t issuance(issuer, initial_debt_owner_ship, debt_ledger_length);
    wasm::db::set(issuance);

}

void synthetix::_append_debt_ledger_value(uint64_t value){
	uint64_t  debt_ledger_length = get_uint_value(KEY_DEBT_LEDGER_LENGTH);
    set_uint_value(KEY_DEBT_LEDGER_LENGTH, debt_ledger_length + 1);

	// debt_ledger_t object;
 //    debt_ledger   debt_ledger_table(_self, SYNTHETIX_SCOPE);
 //    debt_ledger_table.emplace( wasm::no_payer, [&]( auto& s ) {
 //    	s.index = debt_ledger_length;
 //        s.value = value;
 //    });
    debt_ledger_t ledger(debt_ledger_length, value);
    wasm::db::set(ledger);

}


uint64_t synthetix::_last_debt_ledger_entry(){

	uint64_t  debt_ledger_length = get_uint_value(KEY_DEBT_LEDGER_LENGTH);
	return _debt_ledger(debt_ledger_length - 1);
}

uint64_t synthetix::_debt_ledger(uint64_t index){

	// debt_ledger_t object;
 //    debt_ledger   debt_ledger_table(_self, SYNTHETIX_SCOPE);
 //    check( debt_ledger_table.get( object, index), "index does not exists" );
    debt_ledger_t ledger(index);
    check(wasm::db::get(ledger), "Index does not exists" );

    return ledger.value;
}

uint64_t synthetix::debt_ledger_length(){
    return get_uint_value(KEY_DEBT_LEDGER_LENGTH);
}


uint64_t synthetix::issuance_ratio(){
    return get_uint_value(KEY_ISSUANCE_RATIO);
}

ACTION synthetix::setissuratio(uint64_t issuance_ratio) {
     require_auth( get_self() );
     check( issuance_ratio <= MAX_ISSUANCE_RATIO, "New issuance ratio cannot exceed MAX_ISSUANCE_RATIO");
     set_uint_value(KEY_ISSUANCE_RATIO, issuance_ratio);
}

