#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>
#include <inline_transaction.hpp>

#include <string>
using namespace wasm;

namespace wasm {

   using std::string;

   CONTRACT mpas : public contract {
      public:
         using contract::contract;

         ACTION set_owner(regid owner);
         ACTION mint(regid to, asset quantity, string memo );
         ACTION burn(regid from, asset quantity, string memo );
         ACTION transfer(regid from, regid to, asset quantity, string memo );
         ACTION balance_of( regid owner, const symbol& symbol );

         static asset get_max_supply( regid token_contract_account, symbol_code sym_code )
         {
            assetstat statstable( token_contract_account, sym_code.raw() );

            asset_stats st;
            statstable.get( st, sym_code.raw() );
            return st.total_supply;
         }

         static asset get_balance( regid token_contract_account, regid owner, symbol_code sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );

            account ac;
            accountstable.get( ac, sym_code.raw() );
            return ac.balance;
         }

      private:
         //storages
         TABLE uint_storage_t {
             name     key;
             uint64_t value;

             uint64_t primary_key()const { return key.value; }
         };
         typedef wasm::table< "uintstore"_n, uint_storage_t, uint64_t > tbl_uint_store;

         TABLE  account {
            regid    owner;
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };
         typedef wasm::table< "accounts"_n, account, uint64_t > accounts;

         TABLE  asset_stats {
            asset    total_supply;
            regid    issuer;

            uint64_t primary_key()const { return total_supply.symbol.code().raw(); }
         };
         typedef wasm::table< "assetstat"_n, asset_stats, uint64_t > assetstat;

         void sub_balance( regid owner, asset value );
         void add_balance( regid owner, asset value, regid ram_payer );

         uint64_t _get_uint_value(const name& key);
         void     _set_uint_value(const name& key, uint64_t value);
         void     _del_uint_value(const name& key);

         regid    _get_owner();

   };
} /// namespace wasm
