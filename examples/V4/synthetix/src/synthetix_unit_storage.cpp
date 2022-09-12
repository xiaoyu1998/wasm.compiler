#include <synthetix.hpp>
#include <system.hpp>
#include <strings.hpp>
#include <synthetix_constants.hpp>
#include <synthetix_database.hpp>

using namespace wasm;
using namespace wayki;

uint64_t synthetix::get_uint_value(const name& key) {
    uint_storage_t object(key);
    if( !wasm::db::get(object)) return 0;
    return object.value;
}

void synthetix::set_uint_value(const name& key, uint64_t value) {
    uint_storage_t object = {key, value};
    wasm::db::set(object);
}

void synthetix::delete_uint_value(const name& key) {
    wasm::db::del(uint_storage_t(key));
}


uint64_t synthetix::get_u128_value(const uint128_t& key) {
    uint128_storage_t object(key);
    if( !wasm::db::get(object)) return 0;
    return object.value;
}

void synthetix::set_u128_value(const uint128_t& key, uint64_t value) {
    uint128_storage_t object = {key, value};
    wasm::db::set(object);
}

void synthetix::delete_u128_value(const uint128_t& key) {
    wasm::db::del(uint128_storage_t(key));
}





