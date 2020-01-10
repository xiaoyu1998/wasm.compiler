/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

#include "fixed_bytes.hpp"


namespace wasm {

   /**
    *  Tests if the SHA256 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    *  @note This method is optimized to a NO-OP when in fast evaluation mode.
    */
   void assert_sha256( const char* data, uint32_t length, const checksum256& hash );

   /**
    *  Tests if the SHA1 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    *  @note This method is optimized to a NO-OP when in fast evaluation mode.
    */
   void assert_sha1( const char* data, uint32_t length, const checksum160& hash );

   /**
    *  Tests if the SHA512 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    *  @note This method is optimized to a NO-OP when in fast evaluation mode.
    */
   void assert_sha512( const char* data, uint32_t length, const checksum512& hash );

   /**
    *  Tests if the RIPEMD160 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_ripemd160( const char* data, uint32_t length, const checksum160& hash );


    /**
    *  Hashes `data` using SHA256.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum256 - Computed digest
    */
   wasm::checksum256 sha256( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA1.
    *
    *  @ingroup crypto
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum160 - Computed digest
    */
   wasm::checksum160 sha1( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA512.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum512 - Computed digest
    */
   wasm::checksum512 sha512( const char* data, uint32_t length );

   /**
    *  Hashes `data` using RIPEMD160.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum160 - Computed digest
    */
   wasm::checksum160 ripemd160( const char* data, uint32_t length );
} // namespace wasm
