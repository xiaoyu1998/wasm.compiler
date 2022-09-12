#include <wasm.hpp>
#include <asset.hpp>
#include <string>
using namespace wasm;
using namespace std;

CONTRACT crypto : public contract {
   public:
      using contract::contract;

      ACTION cryptotest( string h );

};
