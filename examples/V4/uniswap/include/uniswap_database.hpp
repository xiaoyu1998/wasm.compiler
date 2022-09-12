#include <wasm.hpp>
#include <asset.hpp>
#include <db.hpp>

#include <string>
using namespace wasm;
using namespace wasm::db;

namespace wasm {

#define TABLE_IN_CONTRACT [[wasm::table, wasm::contract("uniswap")]]

struct TABLE_IN_CONTRACT market_t {
   regid id;
   regid token0;
   regid token1;

   regid liquidity_token;
   asset liquidity_total_supply;

   asset reserve0;
   asset reserve1;

   time_point_sec block_timestamp_last;
   bool           closed = false;

   int128_t price0_cumulative_last = 0;
   int128_t price1_cumulative_last = 0;

   market_t(){}
   market_t(uint64_t r):id(regid(r)){}
   uint64_t scope() const { return id.value; }
   uint64_t primary_key() const { return id.value; }

   void   print() const {wasm::print(to_string());}
   string to_string() const {
        return wasm::to_string("id:% token0:% token1:% liquidity_token:% liquidity_total_supply:% reserve0:% reserve1:% closed:%", 
                                id, token0, token0, liquidity_token, liquidity_total_supply, reserve0, reserve0, closed);
   }   

   typedef index_table<"market"_n, market_t, uint64_t> table_t;
   WASM_DB_TABLE_TYPE_DEFINITION(table_t)

   WASMLIB_SERIALIZE( market_t, (id)(token0)(token1)(liquidity_token)(liquidity_total_supply)
                                (reserve0)(reserve1)(block_timestamp_last)(closed)
                                (price0_cumulative_last)(price1_cumulative_last))

};

}