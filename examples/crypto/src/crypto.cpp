#include <cryptotest.hpp>
#include "crypto.hpp"

ACTION crypto::cryptotest( string h ) {

   wasm::checksum160 hash160 = wasm::sha1((const char*)h.data(), h.size());
   wasm::assert_sha1((const char*)h.data(), h.size(), hash160);
   hash160.print();
   print("\n");

   wasm::checksum256 hash256 = wasm::sha256((const char*)h.data(), h.size());
   wasm::assert_sha256((const char*)h.data(), h.size(), hash256);
   hash256.print();
   print("\n");

   wasm::checksum512 hash512 = wasm::sha512((const char*)h.data(), h.size());
   wasm::assert_sha512((const char*)h.data(), h.size(), hash512);
   hash512.print();
   print("\n");

   wasm::checksum160 ripemd160 = wasm::ripemd160((const char*)h.data(), h.size());
   wasm::assert_ripemd160((const char*)h.data(), h.size(), ripemd160);
   ripemd160.print();

}

WASM_DISPATCH( crypto, (cryptotest))