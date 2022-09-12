#pragma once
#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>
#include <synthetix_constants.hpp>


using namespace wasm;
using namespace wayki;


namespace wasm { namespace db{ 

#define TABLE_IN_CONTRACT [[wasm::table, wasm::contract("synthetix")]]

        //synthetix
        struct TABLE_IN_CONTRACT mpas_index_t {
            name                         key;
            map<uint64_t, symbol_code>   indexs; 
            uint64_t primary_key()const { return key.value; }
            uint64_t scope()const { return SYNTHETIX_SCOPE; }
            mpas_index_t(const name &k):key(k){}
            mpas_index_t(){}

            WASMLIB_SERIALIZE( mpas_index_t, (key)(indexs))
        };

        struct TABLE_IN_CONTRACT mpa_t {
            regid contract;
            regid issuer;
            asset total_supply;
            uint64_t primary_key()const { return total_supply.symbol.code().raw(); }
            uint64_t scope()const { return SYNTHETIX_SCOPE; }
            mpa_t(const asset &supply):total_supply(supply){}
            mpa_t(uint64_t code):total_supply(0,symbol(symbol_code(code), PRECISION)){}
            mpa_t(uint64_t c, uint64_t i, uint64_t code):contract(regid(c)),issuer(regid(i)),total_supply(0,symbol(symbol_code(code), PRECISION)){}
            mpa_t(){}

            WASMLIB_SERIALIZE( mpa_t, (contract)(issuer)(total_supply))
        };

        typedef wasm::table< "mpasindex"_n, mpas_index_t, uint64_t > mpas_index;  
        typedef wasm::table< "mpas"_n, mpa_t, uint64_t > mpas;  

        //exchange rates
        struct TABLE_IN_CONTRACT rate_and_updated_time_t{
            symbol_code currency_key;
            uint64_t    round;
            uint64_t    rate;
            uint64_t    time;

            uint128_t  primary_key()const { return (uint128_t)currency_key.raw() << 64 | (uint128_t)round; }
            uint64_t   scope()const { return SYNTHETIX_SCOPE; }
            // rate_and_updated_time_t(uint128_t key):currency_key(symbol_code(key >> 64)), round(key){}
            rate_and_updated_time_t(symbol_code key, uint64_t r):currency_key(key), round(r){}
            rate_and_updated_time_t(symbol_code key, uint64_t r, uint64_t ra, uint64_t t):currency_key(key), round(r), rate(ra), time(t){}

            rate_and_updated_time_t(){}

            WASMLIB_SERIALIZE( rate_and_updated_time_t, (currency_key)(round)(rate)(time))
        };

        struct TABLE_IN_CONTRACT round_for_rate_t{
            symbol_code currency_key;
            uint64_t    round;

            uint64_t primary_key()const { return currency_key.raw() ; }
            uint64_t scope()const { return SYNTHETIX_SCOPE; }
            round_for_rate_t(symbol_code key):currency_key(key){}
            round_for_rate_t(uint64_t key):currency_key(symbol_code(key)){}
            round_for_rate_t(){}

            WASMLIB_SERIALIZE( round_for_rate_t, (currency_key)(round))
        };
        typedef wasm::table< "ratetimes"_n, rate_and_updated_time_t, uint128_t > rate_and_updated_times;  
        typedef wasm::table< "roundrates"_n, round_for_rate_t, uint64_t > round_for_rates; 

// struct inverse_pricing_t {
//    uint64_t     currency_key;
//    uint64_t entry_point;
//    uint64_t upper_limit;
//    uint64_t lower_limit;
//    bool     frozen;
//    uint64_t primary_key()const { return currency_key.raw(); }
// }
// //inverse
// typedef wasm::table< "inverses"_n, inverse_pricing_t, uint64_t > inverses; 

        //exchange state
         struct TABLE_IN_CONTRACT exchange_entry_t {
            symbol_code src;
            asset       amount; 
            symbol_code dest;
            asset       amount_received;
            uint64_t    exchange_fee_rate;
            uint64_t    time_stamp;
            uint64_t    round_id_for_src;
            uint64_t    round_id_for_dest;

            WASMLIB_SERIALIZE( exchange_entry_t, (src)(amount)(dest)(amount_received)(exchange_fee_rate)
                                                 (time_stamp)(round_id_for_src)(round_id_for_dest))
        };   

        struct TABLE_IN_CONTRACT exchange_t {
            regid                    owner;
            symbol_code              currency_key;
            vector<exchange_entry_t> entries;
            uint128_t primary_key()const { return (uint128_t)owner.value << 64 | (uint128_t)currency_key.raw(); }
            uint64_t  scope()const { return SYNTHETIX_SCOPE; }
            exchange_t(regid o, symbol_code k):owner(o), currency_key(k){}
            exchange_t(uint64_t o, symbol_code k):owner(regid(o)), currency_key(k){}
            exchange_t(){}

            WASMLIB_SERIALIZE( exchange_t, (owner)(currency_key)(entries))

        };  
        typedef wasm::table< "exchanges"_n, exchange_t, uint128_t > exchanges;  


        //state
        struct TABLE_IN_CONTRACT debt_ledger_t{
            uint64_t index;
            uint64_t value;
            uint64_t primary_key()const { return index; }
            uint64_t scope()const { return SYNTHETIX_SCOPE; }
            debt_ledger_t(uint64_t i, uint64_t v):index(i),value(v){}
            debt_ledger_t(uint64_t i):index(i){}
            debt_ledger_t(){}

            WASMLIB_SERIALIZE( debt_ledger_t, (index)(value))
        };

        struct TABLE_IN_CONTRACT issuance_data_t{
            regid    owner;
            uint64_t initial_debt_owner_ship;
            uint64_t debt_entry_index;
            uint64_t primary_key()const { return owner.value; }
            uint64_t scope()const { return SYNTHETIX_SCOPE; }
            issuance_data_t(regid account, uint64_t init, uint64_t index):owner(account),initial_debt_owner_ship(init),debt_entry_index(index){}
            issuance_data_t(regid account):owner(account){}
            issuance_data_t(){}

            WASMLIB_SERIALIZE( issuance_data_t, (owner)(initial_debt_owner_ship)(debt_entry_index))
        };

        typedef wasm::table< "debtledger"_n, debt_ledger_t, uint64_t >   debt_ledger;  
        typedef wasm::table< "issuancedata"_n, issuance_data_t, uint64_t > issuance_data;  

        //token state
         struct TABLE_IN_CONTRACT  account {
            regid    owner;
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
            uint64_t scope()const { return owner.value; }
         };

         struct TABLE_IN_CONTRACT  currency_stats {
            asset    supply;
            asset    max_supply;
            regid    issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
            uint64_t scope()const { return issuer.value; }
         };

         typedef wasm::table< "accounts"_n, account, uint64_t > accounts;
         typedef wasm::table< "stat"_n, currency_stats, uint64_t > stats;

         //fee pool
         struct TABLE_IN_CONTRACT fee_period_t {
             uint64_t fee_period_id;
             uint64_t starting_debt_index;
             uint64_t start_time;
             uint64_t fees_to_distribute;
             uint64_t fees_claimed;
             uint64_t rewards_to_distribute;
             uint64_t rewards_claimed;

             WASMLIB_SERIALIZE( fee_period_t, (fee_period_id)(starting_debt_index)(start_time)
                                              (fees_to_distribute)(fees_claimed)(rewards_to_distribute)(rewards_claimed))

         };

         struct TABLE_IN_CONTRACT recent_fee_periods_t {
             name                 key;
             vector<fee_period_t> fee_periods;

             uint64_t primary_key()const { return key.value; }
             uint64_t scope()const { return SYNTHETIX_SCOPE; }
             recent_fee_periods_t(name k):key(k){}
             recent_fee_periods_t(){}

             WASMLIB_SERIALIZE( recent_fee_periods_t, (key)(fee_periods))

         };

         typedef wasm::table< "feeperiods"_n, recent_fee_periods_t, uint64_t > fee_periods;

         //fee pool state
         struct TABLE_IN_CONTRACT feepool_issuance_data_t {
             uint64_t debt_percentage;
             uint64_t debt_entry_index;

             WASMLIB_SERIALIZE( feepool_issuance_data_t, (debt_percentage)(debt_entry_index))
         };
         struct TABLE_IN_CONTRACT  account_issuance_ledger_t {
            regid                           owner;
            vector<feepool_issuance_data_t> issuance_data;

            uint64_t primary_key()const { return owner.value; }
            uint64_t scope()const { return SYNTHETIX_SCOPE; }
            account_issuance_ledger_t(regid account):owner(account){}
            account_issuance_ledger_t(uint64_t account):owner(regid(account)){}
            account_issuance_ledger_t(){}

            WASMLIB_SERIALIZE( account_issuance_ledger_t, (owner)(issuance_data))
         };

         typedef wasm::table< "accissledger"_n, account_issuance_ledger_t, uint64_t > account_issuance_ledgers;

         //storages
         struct TABLE_IN_CONTRACT uint_storage_t {
             name     key;
             uint64_t value;

             uint64_t primary_key()const { return key.value; }
             uint64_t scope()const { return SYNTHETIX_SCOPE; }
             uint_storage_t(name k):key(k){}
             uint_storage_t(name k, uint64_t v):key(k),value(v){}
             uint_storage_t(){}

             WASMLIB_SERIALIZE( uint_storage_t, (key)(value))
         }; 

         struct TABLE_IN_CONTRACT uint128_storage_t {
             uint128_t key;
             uint64_t  value;

             uint128_t primary_key()const { return key; }
             uint64_t scope()const { return SYNTHETIX_SCOPE; }
             uint128_storage_t(const uint128_t &k):key(k){}
             uint128_storage_t(const uint128_t &k, uint64_t v):key(k),value(v){}
             uint128_storage_t(){}

             WASMLIB_SERIALIZE( uint128_storage_t, (key)(value))
         };

         typedef wasm::table< "uintstorages"_n, uint_storage_t, uint64_t > uint_storages;   
         typedef wasm::table< "ustorages"_n, uint128_storage_t, uint64_t > u128_storages; 
}}//wasm//db  

