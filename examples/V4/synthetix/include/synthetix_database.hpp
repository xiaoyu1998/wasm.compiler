#pragma once
#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_entities.hpp>


using namespace wasm;
using namespace synthetix;
using namespace wasm::db;


namespace wasm { namespace db { 

    static constexpr uint64_t  reserved  = 0;   
    enum return_t{
        NONE    = 0,
        MODIFIED,
        APPENDED,
    };

    template<typename Type>
    struct get_table_type{
    };

    template<>
    struct get_table_type<mpas_index_t>{
        using type = mpas_index;
    };

    template<>
    struct get_table_type<mpa_t>{
        using type = mpas;
    };

    template<>
    struct get_table_type<rate_and_updated_time_t>{
        using type = rate_and_updated_times;
    };

    template<>
    struct get_table_type<round_for_rate_t>{
        using type = round_for_rates;
    };

    template<>
    struct get_table_type<exchange_t>{
        using type = exchanges;
    };

    template<>
    struct get_table_type<debt_ledger_t>{
        using type = debt_ledger;
    };

    template<>
    struct get_table_type<issuance_data_t>{
        using type = issuance_data;
    };

    template<>
    struct get_table_type<account>{
        using type = accounts;
    };

    template<>
    struct get_table_type<currency_stats>{
        using type = stats;
    };

    template<>
    struct get_table_type<recent_fee_periods_t>{
        using type = fee_periods;
    };

    template<>
    struct get_table_type<account_issuance_ledger_t>{
        using type = account_issuance_ledgers;
    };

    template<>
    struct get_table_type<uint_storage_t>{
        using type = uint_storages;
    };

    template<>
    struct get_table_type<uint128_storage_t>{
        using type = u128_storages;
    };

    //with object type
    template<typename ObjectType>
    bool get(ObjectType& object) {
        typename get_table_type<ObjectType>::type objects(regid(wasm::db::reserved), object.scope());
        if( !objects.get( object, object.primary_key())) return false;//should be return error
        return true;
    }

    template<typename ObjectType>
    return_t set(const ObjectType& object) {
        ObjectType                                obj;
        typename get_table_type<ObjectType>::type objects(regid(wasm::db::reserved), object.scope());

        if(objects.get( obj, object.primary_key() )){
            objects.modify( obj, wasm::no_payer, [&]( auto& s ) {
               s = object;
            });
            return return_t::MODIFIED;        
        } else {
            objects.emplace( wasm::no_payer, [&]( auto& s ) {
               s = object;
            });
            return return_t::APPENDED;
        }
    }

    template<typename ObjectType>
    void del(const ObjectType& object) {
        typename get_table_type<ObjectType>::type objects(regid(wasm::db::reserved), object.scope());
        objects.erase(object.primary_key(), wasm::no_payer);
    }

}}//db//wasm


// #define synthetix_entities_declaration()                                                                               \
//         TABLE uint64_t_index_t {                                                                                    \
//             name                         key;                                                                       \
//             map<uint64_t, symbol_code>   indexs;                                                                    \
//             uint64_t primary_key()const { return key.value; }                                                       \
//             uint64_t_index_t& set(uint64_t k) {                                                                     \
//                 key = name(k);                                                                                      \
//                 return &*this;                                                                                      \
//             }                                                                                                       \
//         };                                                                                                          \
//         typedef wasm::table< "uintindexs"_n, uint64_t_index_t, uint64_t > uint64_indexs;                            \
//                                                                                                                     \
//         TABLE mpa_t {                                                                                               \
//             regid contract;                                                                                         \
//             regid issuer;                                                                                           \
//             asset total_supply;                                                                                     \
//             uint64_t primary_key()const { return total_supply.symbol.code().raw(); }                                \
//         };                                                                                                          \
//         typedef wasm::table< "mpas"_n, mpa_t, uint64_t > mpas;                           

// #define synthetix_exchange_rates_entities_declaration()                                                                \
//         TABLE rate_and_updated_time_t{                                                                              \
//             symbol_code currency_key;                                                                               \
//             uint64_t    index;                                                                                      \
//             uint64_t    rate;                                                                                       \
//             uint64_t    time;                                                                                       \
//                                                                                                                     \
//             uint128_t  primary_key()const { return (uint128_t)currency_key.raw() << 64 | (uint128_t)index; }        \
//         };                                                                                                          \
//         typedef wasm::table< "ratetimes"_n, rate_and_updated_time_t, uint128_t > rate_and_updated_times;            \
//                                                                                                                     \
//         TABLE round_for_rate_t{                                                                                     \
//             symbol_code currency_key;                                                                               \
//             uint64_t    round;                                                                                      \
//                                                                                                                     \
//             uint64_t primary_key()const { return currency_key.raw() ; }                                             \
//         };                                                                                                          \
//         typedef wasm::table< "roundrates"_n, round_for_rate_t, uint64_t > round_for_rates; 

// #define synthetix_exchange_state_entities_declaration()                                                                \
//          TABLE exchange_entry_t {                                                                                   \
//             name  src;                                                                                              \
//             asset amount;                                                                                           \
//             name  dest;                                                                                             \
//             asset amount_recived;                                                                                   \
//             uint64_t exchange_fee_rate;                                                                             \
//             uint64_t time_stamp;                                                                                    \
//             uint64_t round_id_for_src;                                                                              \
//             uint64_t round_id_for_dest;                                                                             \
//         };                                                                                                          \
//                                                                                                                     \
//         TABLE exchange_t {                                                                                          \
//             regid                    owner;                                                                         \
//             name                     currency_key;                                                                  \
//             vector<exchange_entry_t> entries;                                                                       \
//             uint128_t primary_key()const { (uint128_t)owner.value << 64 | (uint128_t)currency_key.raw(); }          \
//         };                                                                                                          \
//         typedef wasm::table< "exchanges"_n, exchange_t, uint128_t > exchanges;  

// #define synthetix_state_entities_declaration()                                                                         \
//         TABLE debt_ledger_t{                                                                                        \
//             uint64_t index;                                                                                         \
//             uint64_t value;                                                                                         \
//             uint64_t primary_key()const { return index; }                                                           \
//         };                                                                                                          \
//                                                                                                                     \
//         TABLE issuance_data_t{                                                                                      \
//             regid owner;                                                                                            \
//             uint64_t initial_debt_owner_ship;                                                                       \
//             uint64_t debt_entry_index;                                                                              \
//             uint64_t primary_key()const { return owner.value; }                                                     \
//         };                                                                                                          \
//                                                                                                                     \
//         typedef wasm::table< "debtledger"_n, debt_ledger_t, uint64_t >   debt_ledger;                               \
//         typedef wasm::table< "issuancedata"_n, issuance_data_t, uint64_t > issuance_data;                                                                 

// #define synthetix_token_state_entities_declaration()                                                                   \
//          TABLE  account {                                                                                           \
//             regid    owner;                                                                                         \
//             asset    balance;                                                                                       \
//                                                                                                                     \
//             uint64_t primary_key()const { return balance.symbol.code().raw(); }                                     \
//          };                                                                                                         \
//                                                                                                                     \
//          TABLE  currency_stats {                                                                                    \
//             asset    supply;                                                                                        \
//             asset    max_supply;                                                                                    \
//             regid    issuer;                                                                                        \
//                                                                                                                     \
//             uint64_t primary_key()const { return supply.symbol.code().raw(); }                                      \
//          };                                                                                                         \
//                                                                                                                     \
//          typedef wasm::table< "accounts"_n, account, uint64_t > accounts;                                           \
//          typedef wasm::table< "stat"_n, currency_stats, uint64_t > stats;

// #define synthetix_feepool_entities_declaration()                                                                       \
//          TABLE fee_period_t {                                                                                       \
//              uint64_t fee_period_id;                                                                                \
//              uint64_t starting_debt_index;                                                                          \
//              uint64_t start_time;                                                                                   \
//              uint64_t fees_to_distribute;                                                                           \
//              uint64_t fees_claimed;                                                                                 \
//              uint64_t rewards_to_distribute;                                                                        \
//              uint64_t rewards_claimed;                                                                              \
//          }                                                                                                          \
//                                                                                                                     \
//          TABLE fee_period_storage_t {                                                                               \
//              name                                 key;                                                              \
//              fee_period_t[POOL_FEE_PERIOD_LENGTH] fee_periods                                                       \
//                                                                                                                     \
//              uint64_t primary_key()const { return key.value; }                                                      \
//          }                                                                                                          \
//                                                                                                                     \
//          typedef wasm::table< "feeperiods"_n, fee_period_storage_t, uint64_t > fee_periods;                                                                              

// #define synthetix_feepool_state_entities_declaration()                                                                 \
//          TABLE feepool_issuance_data_t {                                                                            \
//              uint64_t debt_percentage;                                                                              \
//              uint64_t debt_entry_index;                                                                             \
//          }                                                                                                          \
//          TABLE  account_issuance_ledger_t {                                                                         \
//             regid                                              owner;                                               \
//             feepool_issuance_data_t[ACCOUNT_FEE_PERIOD_LENGTH] issuance_data;                                       \
//                                                                                                                     \
//             uint64_t primary_key()const { return owner.value; }                                                     \
//          };                                                                                                         \
//                                                                                                                     \
//          typedef wasm::table< "accissledger"_n, account_issuance_ledger_t, uint64_t > account_issuance_ledgers;     


//  #define synthetix_key_storage_entities_declaration                                                                    \
//         TABLE uint_storage_t {                                                                                      \
//             name     key;                                                                                           \
//             uint64_t value;                                                                                         \
//                                                                                                                     \
//             uint64_t primary_key()const { return key.value; }                                                       \
//         };                                                                                                          \
//                                                                                                                     \
//         TABLE uint128_storage_t {                                                                                   \
//             uint128_t key;                                                                                          \
//             uint64_t value;                                                                                         \
//                                                                                                                     \
//             uint128_t primary_key()const { return key; }                                                            \
//         };                                                                                                          \
//         typedef wasm::table< "uintstorages"_n, uint_storage_t, uint64_t > uint_storages;                            \
//         typedef wasm::table< "ustorages"_n, uint128_storage_t, uint64_t > u128_storages;   
