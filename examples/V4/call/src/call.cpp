#include <call.hpp>

using namespace wasm;

#define WASM_FUNCTION_PRINT_LENGTH 50

#define WASM_LOG_PRINT( debug,  ... ) {     \
if ( debug ) {                               \
   std::string str = std::string(__FILE__); \
   str += std::string(":");                 \
   str += std::to_string(__LINE__);         \
   str += std::string(":[");                \
   str += std::string(__FUNCTION__);        \
   str += std::string("]");                 \
   while(str.size() <= WASM_FUNCTION_PRINT_LENGTH) str += std::string(" ");\
   print(str);                                                             \
   print( __VA_ARGS__ ); }}

#define BALANCE_OF(token, owner, symbol) \
    get_return_wasm<asset>(wasm::transaction{token, name("balance_of"), std::vector<permission>{{_self, name("wasmio_code")}}, std::tuple<regid, class symbol>(owner, symbol)}.call());

template<typename T>
T get_return_wasm(uint64_t size){
    T return_value = wasm::unpack<T>(get_return(size));
    return return_value;
}

ACTION call::get_balance( regid token, regid owner, const symbol& symbol)
{

    asset return_value = BALANCE_OF(token, owner, symbol)
    
    WASM_LOG_PRINT(true,
      " return_value:", return_value)
}


WASM_DISPATCH( call, (get_balance))
