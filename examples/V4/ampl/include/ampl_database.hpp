#include <wasm.hpp>
#include <asset.hpp>
#include <db.hpp>

#include <string>
using namespace wasm;
using namespace wasm::db;

namespace wasm {

#define TABLE_IN_CONTRACT [[wasm::table, wasm::contract("ampl")]]

struct TABLE_IN_CONTRACT global_t {

    regid id;
    asset total_supply;
    uint64_t gons_per_fragment; 

    uint64_t last_rebase_time_stamp_sec;
    uint64_t min_rebase_time_interval_sec;
    uint64_t rebase_window_offset_sec;
    uint64_t rebase_window_length_sec;
    uint64_t epoch;
    uint64_t base_cpi;
    uint64_t rebase_lag;

    regid    cpi_oracle;
    regid    market_oracle;

    global_t(){}
    global_t(uint64_t r):id(regid(r)){}
    uint64_t scope() const { return id.value; }
    uint64_t primary_key() const { return id.value; }

    void   print() const {wasm::print(to_string());}
    string to_string() const {
        return wasm::to_string("id:% total_supply:% gons_per_fragment:%", 
                                id, total_supply, gons_per_fragment);
    }   

    typedef index_table<"global"_n, global_t, uint64_t> table_t;
    WASM_DB_TABLE_TYPE_DEFINITION(table_t)

    WASMLIB_SERIALIZE( global_t, (id)(total_supply)(gons_per_fragment)
                                 (last_rebase_time_stamp_sec)(min_rebase_time_interval_sec)(rebase_window_offset_sec)
                                 (rebase_window_length_sec)(epoch)(base_cpi)(rebase_lag)(cpi_oracle)(market_oracle))

};

struct TABLE_IN_CONTRACT account_t {
    regid    id;
    uint64_t balance = 0;

    account_t(){}
    account_t(uint64_t r):id(regid(r)){}
    uint64_t scope() const { return id.value; }
    uint64_t primary_key() const { return id.value; }

    void   print() const {wasm::print(to_string());}
    string to_string() const {
          return wasm::to_string("id:% balance:%",id, balance);
    } 

    typedef index_table<"accounts"_n, account_t, uint64_t> table_t;
    WASM_DB_TABLE_TYPE_DEFINITION(table_t)

    WASMLIB_SERIALIZE( account_t, (id)(balance))    

};

}