#include <asset.hpp>
#include <wasm.hpp>
#include <table.hpp>
#include <inline_transaction.hpp>

#include <string>
using namespace wasm;

namespace wasm {

   using std::string;
   CONTRACT prints : public contract {
      public:
         using contract::contract;

         ACTION printnormal( );
         // ACTION print1( wasm::time_point a, wasm::time_point b, wasm::time_point c );
         // ACTION print2( int128_t a, int128_t b );
         // ACTION print3( float a, float b, float c );
         // ACTION print4( double a, double b, double c );
         // ACTION print5( checksum160 a, checksum256 b, checksum512 c );
         // ACTION printinf();
   };

} /// namespace wasm
