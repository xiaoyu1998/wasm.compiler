#include <wasm.hpp>
#include <asset.hpp>
#include <db.hpp>

#include <string>
using namespace wasm;
using namespace wasm::db;


#define TABLE_IN_CONTRACT [[wasm::table, wasm::contract("index")]]

struct  TABLE_IN_CONTRACT account_t {
   uint64_t id;
   regid    owner;
   asset    balance;

   uint64_t scope() const { return id; }
   uint64_t primary_key()const { return id; }
   regid    get_owner()const{ return owner; }
   uint64_t get_symbol_code()const { return balance.symbol.code().raw(); }

   std::string to_string() const {
      return wasm::to_string("id:% owner:% balance:%", id, owner, balance);
   }
   void print() const {wasm::print(to_string());}

   typedef index_table< "accounts"_n, account_t, uint64_t,
                         indexed_by<"by_owner"_n, const_mem_fun<account_t, regid, &account_t::get_owner>>,
                         indexed_by<"by_code"_n, const_mem_fun<account_t, uint64_t, &account_t::get_symbol_code>>> table_t;
   WASM_DB_TABLE_TYPE_DEFINITION(table_t)


   WASMLIB_SERIALIZE( account_t, (id)(owner)(balance))

};

