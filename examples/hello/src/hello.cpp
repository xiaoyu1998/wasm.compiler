#include <hello.hpp>
#include "producer_schedule.hpp"
#include "crypto.hpp"

ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);

   string h("hello");

   wasm::checksum160 hash160 = wasm::sha1((const char*)h.data(), h.size());
   printhex((const void*)&hash160, 20);
   print("\n");

   wasm::checksum256 hash256 = wasm::sha256((const char*)h.data(), h.size());
   printhex((const void*)&hash256, 32);
   print("\n");

   wasm::checksum512 hash512 = wasm::sha512((const char*)h.data(), h.size());
   printhex((const void*)&hash512, 64);
   print("\n");

   wasm::checksum160 ripemd160 = wasm::ripemd160((const char*)h.data(), h.size());
   printhex((const void*)&ripemd160, 20);

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