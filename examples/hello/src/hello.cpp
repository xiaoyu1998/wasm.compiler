#include <hello.hpp>
#include "producer_schedule.hpp"

ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);
   uint64_t i = 0;
   while(true){
       print_f("Name : %", nm);
       print(i);
       i++;
   }
}

ACTION hello::check( name nm ) {
   // print_f("Name : %\n", nm);
   // wasm::check(nm == "hello"_n, "check name not equal to `hello`");

   // name producer[11];
   // get_active_producers(producer, sizeof(name) * 11);

   auto producers = get_active_producers();
   

   print(producers[0]);
   print("\n");
   print(producers[1]);

}


ACTION hello::transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo )
{

  if (to == get_self()) {
      print("hello.transfer");
      wasm::transaction inline_trx(name("wasmio.bank"), name("transfer"), std::vector<permission>{{to, name("wasmio.owner")}}, std::tuple(to, from, quantity, memo));
      inline_trx.send();
  }

}


WASM_DISPATCH( hello, (hi)(check)(transfer))