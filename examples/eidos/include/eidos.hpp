#include <asset.hpp>
#include <wasm.hpp>
#include <table.hpp>
#include <inline_transaction.hpp>
#include "../capi/types.h"
#include "../core/time.hpp"

#include <string>
using namespace wasm;

//namespace wasm {

   using std::string;

   CONTRACT eidos : public contract {
      public:
         using contract::contract;
 
         ACTION create( name   issuer,
                      asset  maximum_supply);

         ACTION issue( name to, asset quantity, string memo );

         ACTION retire( asset quantity, string memo );

         ACTION transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo );

         ACTION open( name owner, const symbol& symbol, name payer );

         ACTION close( name owner, const symbol& symbol );

         static asset get_supply( name token_contract_account, symbol_code sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            
            currency_stats st;
            statstable.get( st, sym_code.raw() );
            return st.supply;
         }

         static asset get_balance( name token_contract_account, name owner, symbol_code sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );

            account ac;
            accountstable.get( ac, sym_code.raw() );
            return ac.balance;
         }
         // using create_action = wasm::transaction_wrapper<"create"_n, &token::create>;
         // using issue_action = wasm::transaction_wrapper<"issue"_n, &token::issue>;
         // using retire_action = wasm::transaction_wrapper<"retire"_n, &token::retire>;
         // using transfer_action = wasm::transaction_wrapper<"transfer"_n, &token::transfer>;
         // using open_action = wasm::transaction_wrapper<"open"_n, &token::open>;
         // using close_action = wasm::transaction_wrapper<"close"_n, &token::close>;

      private:
         TABLE  account {
            name     owner;
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         TABLE  currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef wasm::table< "accounts"_n, account, uint64_t > accounts;
         typedef wasm::table< "stat"_n, currency_stats, uint64_t > stats;

         void sub_balance( name owner, asset value );
         void add_balance( name owner, asset value, name ram_payer );

   };

//} /// namespace wasm
