#include <wasm.hpp>
#include <asset.hpp>
#include <table.hpp>
#include <inline_transaction.hpp>

#include <string>
using namespace wasm;

namespace wasm {

using std::string;

static constexpr uint64_t RPC_SCOPE = 100;

#define TABLE_IN_CONTRACT [[wasm::table, wasm::contract("rpc")]]

CONTRACT rpc : public contract {
public:
    using contract::contract;

    ACTION transfer(regid from, regid to, asset quantity, string memo);

    // static asset get_balance(regid rpc_contract_account, regid owner, symbol_code sym_code) {
    //     accounts accountstable(rpc_contract_account, owner.value);

    //     account ac;
    //     accountstable.get(ac, sym_code.raw());
    //     return ac.balance;
    // }

    // RPC
    ACTION get_balance(regid id, symbol_code sym);
    ACTION get_account(regid id, symbol_code sym);
private:
    void add_balance(const regid &id, const asset &value);
private:
    struct TABLE_IN_CONTRACT account {
        regid id;
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };

    //                  name          value       scope=account.id
    typedef wasm::table<"accounts"_n, account, uint64_t> accounts;
};

} // namespace wasm
