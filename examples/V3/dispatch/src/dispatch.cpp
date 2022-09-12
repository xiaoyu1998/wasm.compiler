#include <dispatch.hpp>

using namespace wasm;

[[wasm::action]] 
void  dispatch::hi( name nm ) {
   print_f("Name : %\n", nm);
}


[[wasm::action]] 
void dispatch::prints( uint32_t b1, uint32_t b2)
{
    balance1 = b1;
    balance2 = b2;
    print(balance1, balance2);
}

[[wasm::on_notify("57194-2::transfer")]] 
void dispatch::on_transfer(regid from, regid to, asset quant, std::string memo) {
   print_f("on_transfer: % % % %\n", from, to, quant, memo);
}

[[wasm::on_notify("1-800::transfer1")]] 
void dispatch::on_transfer1(regid from, regid to, asset quant, std::string memo) {
   print_f("on_transfer1: % % % %\n", from, to, quant, memo);
}

[[wasm::on_notify("1-800::transfer2")]] 
void dispatch::on_transfer2(regid from, regid to, asset quant, std::string memo) {
   print_f("on_transfer2: % % % %\n", from, to, quant, memo);

}

[[wasm::on_notify("1-800::transfer3")]] 
void dispatch::on_transfer3(regid from, regid to, asset quant, std::string memo) {
   print_f("on_transfer3: % % % %\n", from, to, quant, memo);
}

[[wasm::on_notify("1-900::transfer4")]] 
void dispatch::on_transfer4(regid from, regid to, asset quant, std::string memo) {
   print_f("on_transfer4: % % % %\n", from, to, quant, memo);
}

[[wasm::on_notify("*::transfer")]] 
void dispatch::on_transfer5(regid from, regid to, asset quant, std::string memo) {
   check(get_first_receiver() == regid("0-800"), "the bank should be 0-800");
   print_f("on_transfer5: % % % %\n", from, to, quant, memo);
}

extern "C" bool pre_dispatch(regid self, regid original_receiver, name action) {
   print_f("pre_dispatch : % % %\n", self, original_receiver, action);
   return true;
}

extern "C" void post_dispatch(regid self, regid original_receiver, name action) {
   print_f("post_dispatch : % % %\n", self, original_receiver, action);
}


