#include <wasm.hpp>
#include <asset.hpp>
#include <inline_transaction.hpp>
#include <uniswap_entities.hpp>
#include <uniswap_utils.hpp>

using namespace wasm;
using namespace std;

namespace wasm {

   using std::string;

   CONTRACT uniswap : public contract {
       public:
           using contract::contract;

           [[wasm::action]]
           void create(regid token0, regid token1, symbol symbol0, symbol symbol1, regid liquidity_token);

           // [[wasm::action]] 
           // void burn(std::string market_id, regid to);  

           [[wasm::action]]
           void skim(std::string market_id, regid to, asset balance0, asset balance1);  

           [[wasm::action]]
           void sync(std::string market_id, asset balance0, asset balance1); 

           [[wasm::action]]
           void close(std::string market_id, bool closed);

           [[wasm::action]]
           void get_market(std::string market_id);

           [[wasm::action]]
           void get_action(regid to, uint64_t nonce);

       public:
           [[wasm::on_notify("*::transfer")]] 
           void on_transfer(regid from, regid to, asset quant, std::string memo);

       private:
           void _mint(const uint128_t& market_id, uint64_t step, regid to, asset quant, uint64_t nonce);
           void _burn(const uint128_t& market_id, asset quant, regid to);  
           void _swap(const uint128_t& market_id, asset quant, asset amount_out, regid to);
           void _mint_liquidity(regid liquidity_token, regid to, asset quant);
           void _burn_liquidity(regid liquidity_token, regid from, asset quant);
   };
} /// namespace wasm