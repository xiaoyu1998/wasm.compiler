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
    get_return_wasm(wasm::transaction{token, name("balance_of"), std::vector<permission>{{_self, name("wasmio_code")}}, std::tuple<regid, class symbol>(owner, symbol)}.call_with_return());

int64_t get_return_wasm(int64_t value){
    return value;
}

ACTION call::get_balance( regid token, regid owner, const symbol& symbol)
{

    int64_t return_value = BALANCE_OF(token, owner, symbol)
    print(return_value);
    
    WASM_LOG_PRINT(true,
      " return_value:", return_value)

}


WASM_DISPATCH( call, (get_balance))
