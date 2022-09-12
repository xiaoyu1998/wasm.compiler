#include <wasm.hpp>
#include <asset.hpp>
#include <inline_transaction.hpp>

#include <string>
using namespace wasm;

namespace wasm {

   using std::string;

   CONTRACT call : public contract {
   	  public:

         uint32_t balance1;
         uint32_t balance2;

      public:
         using contract::contract;
 
         ACTION get_balance( regid token, regid owner, const symbol& symbol);

         void print_log();

   };
} /// namespace wasm
