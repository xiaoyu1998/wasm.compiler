/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup system
 * @ingroup c_api
 * @brief Defines API for interacting with system level intrinsics
 * @{
 */

/**
 *  Aborts processing of this action and unwinds all pending changes if the test condition is true
 *
 *  @param test - 0 to abort, 1 to ignore
 *
 *  Example:
 *
 *  @code
 *  wasm_assert(1 == 2, "One is not equal to two.");
 *  wasm_assert(1 == 1, "One is not equal to one.");
 *  @endcode
 *
 *  @param msg - a null terminated string explaining the reason for failure
 */
__attribute__((wasm_wasm_import))
void  wasm_assert( uint32_t test, const char* msg );

/**
 *  Aborts processing of this action and unwinds all pending changes if the test condition is true
 *
 *  @param test - 0 to abort, 1 to ignore
 *  @param msg - a pointer to the start of string explaining the reason for failure
 *  @param msg_len - length of the string
 */
__attribute__((wasm_wasm_import))
void  wasm_assert_message( uint32_t test, const char* msg, uint32_t msg_len );

/**
 *  Aborts processing of this action and unwinds all pending changes if the test condition is true
 *
 *  @brief Aborts processing of this action and unwinds all pending changes
 *  @param test - 0 to abort, 1 to ignore
 *  @param code - the error code
 */
__attribute__((wasm_wasm_import))
void  wasm_assert_code( uint32_t test, uint64_t code );

 /**
 *  This method will abort execution of wasm without failing the contract. This is used to bypass all cleanup / destructors that would normally be called.
 *
 *  @param code - the exit code
 *  Example:
 *
 *  @code
 *  wasm_exit(0);
 *  wasm_exit(1);
 *  wasm_exit(2);
 *  wasm_exit(3);
 *  @endcode
 */
__attribute__((wasm_wasm_import, noreturn))
void wasm_exit( int32_t code );

/**
 *  Returns the time in microseconds from 1970 of the current block
 *
 *  @return time in microseconds from 1970 of the current block
 */
__attribute__((wasm_wasm_import))
uint64_t  current_time();

#ifdef __cplusplus
}
#endif
///@}
