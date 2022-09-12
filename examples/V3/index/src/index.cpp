#include <index.hpp>
#include <wasm_log.hpp>
#include <wasm.hpp>

using namespace wasm;

#define TO_ASSET( amount, code ) \
    asset(amount, symbol(code, 8))


ACTION index::add(uint64_t id, regid owner, asset balance)
{
    account_t account{id, owner, balance};
    wasm::db::set(account);
    WASM_LOG_FPRINT(true, "account:%", account)
}

ACTION index::get( uint64_t id, regid owner, symbol_code code)
{
    account_t account1;
    account1.id = id;
    wasm::db::get(account1);
    WASM_LOG_FPRINT(true, "account1:%", account1)

    account_t account2;
    account2.owner = owner;
    wasm::db::get(N(by_owner), account2);
    WASM_LOG_FPRINT(true, "account2:%", account2)

    account_t account3;
    account3.balance = TO_ASSET(0, code);
    wasm::db::get(N(by_code), account3);
    WASM_LOG_FPRINT(true, "account3:%", account3)
}

//WASM_DISPATCH( index, (add)(get))
