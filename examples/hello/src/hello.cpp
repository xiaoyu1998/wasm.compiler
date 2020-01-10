#include <hello.hpp>
#include "producer_schedule.hpp"
#include "crypto.hpp"

ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);
}

ACTION hello::check( name nm ) {
   print_f("Name : %\n", nm);
   wasm::check(nm == "hello"_n, "check name not equal to `hello`");

   print_f("producer : \n");
   std::vector<name> producers = get_active_producers();
   for(auto p: producers){
   	   print_f("producer : %\n", p);
   }
}

WASM_DISPATCH( hello, (hi)(check))