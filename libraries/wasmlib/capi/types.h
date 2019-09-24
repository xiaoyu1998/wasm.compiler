/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

/**
 *  @defgroup c_types
 *  @ingroup c_api
 *  @brief Specifies builtin types, typedefs and aliases
 */

/**
 *  @addtogroup c_types
 *  @brief Specifies builtin types, typedefs and aliases
 *  @{
 */

/**
 * Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned
 */

/* macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned */
#define ALIGNED(X) __attribute__ ((aligned (16))) X

//typedef uint64_t name;

/**
 * WASM Public Key. It is 34 bytes.
 */
struct public_key {
   char data[34];
};

/**
 * WASM Signature. It is 66 bytes.
 */
struct signature {
   uint8_t data[66];
};

/**
 * 256-bit hash
 */
struct ALIGNED(checksum256) {
   uint8_t hash[32];
};

/**
 * 160-bit hash
 */
struct ALIGNED(checksum160) {
   uint8_t hash[20];
};

/**
 * 512-bit hash
 */
struct ALIGNED(checksum512) {
   uint8_t hash[64];
};

/// @}
