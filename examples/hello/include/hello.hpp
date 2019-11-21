#include <wasm.hpp>
#include <asset.hpp>
#include <string>
using namespace wasm;
using namespace std;

CONTRACT hello : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm );
      ACTION check( name nm );
      ACTION transfer( name    from,
                       name    to,
                       asset   quantity,
                       string  memo );

      // using hi_action = wasm::transaction_wrapper<"hi"_n, &hello::hi>;
      // using check_action = wasm::transaction_wrapper<"check"_n, &hello::check>;
};
