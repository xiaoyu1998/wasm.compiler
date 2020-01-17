#include <hello.hpp>

ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);
}

WASM_DISPATCH( hello, (hi))