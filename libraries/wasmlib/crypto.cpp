#include "core/crypto.hpp"

extern "C" {
   // struct __attribute__((aligned (16))) checksum160 { uint8_t hash[20]; };
   // struct __attribute__((aligned (16))) checksum256 { uint8_t hash[32]; };
   // struct __attribute__((aligned (16))) checksum512 { uint8_t hash[64]; };


   __attribute__((wasm_wasm_import))
   void sha256( const char* data, uint32_t length, checksum256* hash );

   __attribute__((wasm_wasm_import))
   void sha1( const char* data, uint32_t length, checksum160* hash );

   __attribute__((wasm_wasm_import))
   void sha512( const char* data, uint32_t length, checksum512* hash );

   __attribute__((wasm_wasm_import))
   void ripemd160( const char* data, uint32_t length, checksum160* hash );

}

namespace wasm {


   checksum256 sha256( const char* data, uint32_t length ) {
      ::checksum256 hash;
      ::sha256( data, length, &hash );
      //return {hash.hash};
      return hash;
   }

   checksum160 sha1( const char* data, uint32_t length ) {
      ::checksum160 hash;
      ::sha1( data, length, &hash );
      //return {hash.hash};
      return hash;
   }

   checksum512 sha512( const char* data, uint32_t length ) {
      ::checksum512 hash;
      ::sha512( data, length, &hash );
      //return {hash.hash};
      return hash;
   }

   checksum160 ripemd160( const char* data, uint32_t length ) {
      ::checksum160 hash;
      ::ripemd160( data, length, &hash );
      //return {hash.hash};
      return hash;
   }

}