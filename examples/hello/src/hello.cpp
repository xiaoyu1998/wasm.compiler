#include <hello.hpp>
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
   print_f("Name : %\n", nm);
   wasm::check(nm == "hello"_n, "check name not equal to `hello`");
}

// WASM_DISPATCH( hello, (hi)(check) )
ACTION hello::transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo )
{

  if (to == get_self()) {
    print("get ",quantity.to_string()," from ", from.to_string() );
    print(",you won the game!");
  }

	 // require_recipient( from );
  //  require_recipient( to );
}

WASM_DISPATCH( hello, (hi)(check)(transfer))

// extern "C" {
//    void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
//       if(code == wasm::name( "wasmio" ).value){
//          switch( action ) { 
//              case wasm::name( "transfer" ).value: 
// 			            wasm::execute_action( wasm::name(receiver), wasm::name(code), &hello::transfer ); 
// 			       break;
//          }
//       }
//    }
// }