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

};
