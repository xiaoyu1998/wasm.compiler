#include <wasm.hpp>
using namespace wasm;

CONTRACT hello : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm );
      ACTION check( name nm );

      // using hi_action = wasm::transaction_wrapper<"hi"_n, &hello::hi>;
      // using check_action = wasm::transaction_wrapper<"check"_n, &hello::check>;
};
