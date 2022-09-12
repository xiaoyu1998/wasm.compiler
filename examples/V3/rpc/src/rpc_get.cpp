#include "rpc_get.hpp"
#include <rpc.hpp>

using namespace wasm;
// using std::chrono::system_clock;

const regid BANK_DEFAULT = regid(string_view("0-800"));

ACTION rpc::transfer(regid from, regid to, asset quantity, string memo) {

//    check( get_first_receiver() == BANK_DEFAULT, "must transfer by bank=" + BANK_DEFAULT.to_string() );
    check(from != to, "cannot transfer to self");
    check(to == _self, "can transfer to contract self only for recharge");
    require_auth(from);
    check(is_account(to), "to account does not exist");
    auto sym = quantity.symbol.code();

    //  currency_stats st;
    //  stats statstable( _self, sym.raw() );
    //  statstable.get( st, sym.raw() );

    notify_recipient(from);
    notify_recipient(to);

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    //  check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    //  auto payer = has_auth( to ) ? to : from;
    print("recharge from=%s", from.to_string());

    // recharge
    add_balance(from, quantity);
}

ACTION rpc::get_balance(regid id, symbol_code sym) {

    accounts accts(_self, id.value);
    account acct;
    uint64_t balance = 0;
    if (accts.get(acct, sym.raw())) {
        balance = acct.balance.amount;
    }
    auto value = wasm::pack<uint64_t>(balance);
    emit_result_value("balance", "uint64", value);
}


ACTION rpc::get_account(regid id, symbol_code sym){

    accounts accts(_self, id.value);
    account acct;
    uint64_t balance = 0;
    check_fmt(accts.get(acct, sym.raw()), "the account not exist! id=%s, sym=%s",
          id.to_string().c_str(), sym.to_string().c_str());

    emit_result("account", "account", acct);
}

// void rpc::sub_balance( regid owner, asset value ) {
//    accounts from_acnts( _self, owner.value );

//    account from;
//    check( from_acnts.get( from, value.symbol.code().raw()), "no balance object found" );
//    check( from.balance.amount >= value.amount, "overdrawn balance" );

//    from_acnts.modify( from, owner, [&]( auto& a ) {
//          a.balance -= value;
//       });
// }

void rpc::add_balance(const regid &id, const asset &value) {
    accounts accts(_self, id.value);

    account acct;
    if (!accts.get(acct, value.symbol.code().raw())) {
        accts.emplace(id, [&](auto &a) {
            a.id      = id;
            a.balance = value;
        });
        print("add balance of ", value.to_string(), " to new account=", id.to_string());
    } else {
        accts.modify(acct, wasm::no_payer, [&](auto &a) { a.balance += value; });
        print("add balance of ", value.to_string(), " to existed account=", id.to_string());
    }
}

WASM_DISPATCH(rpc, (transfer)(get_balance)(get_account))
