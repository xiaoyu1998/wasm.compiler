#include <wasm.hpp>
#include <asset.hpp>

#include <string>
using namespace wasm;

namespace wasm {

   using std::string;

   CONTRACT ampl : public contract {
       public:
           using contract::contract;

           ACTION init(regid owner, symbol_code code, uint64_t base_cpi, regid cpi_oracle, regid market_oracle);
           ACTION rebase();   
           ACTION transfer(regid from, regid to, asset quantity, string memo );  
           ACTION total_supply();     
           ACTION balance_of(regid owner);  
           // ACTION get_ampl(); 
           ACTION set_oracle(regid cpi, regid market);
           ACTION set_lag(uint64_t rebase_lag);


       private:
       	   void _sub_balance( regid owner, asset value );
           void _add_balance( regid owner, asset value, regid payer );
           uint64_t  _rebase(uint64_t epoch, int64_t supply_delta);
           uint64_t  _get_oracle_data(regid oracle);
           int64_t   _compute_supply_delta(uint64_t exchange_rate, int64_t cpi_rate);
   };
} /// namespace wasm
