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
 *  Copy up to length bytes of current action data to the specified location
 *
 *  @brief Copy current action data to the specified location
 *  @param msg - a pointer where up to length bytes of the current action data will be copied
 *  @param len - len of the current action data to be copied, 0 to report required size
 *  @return the number of bytes copied to msg, or number of bytes that can be copied if len==0 passed
 *  @pre `msg` is a valid pointer to a range of memory at least `len` bytes long
 *  @post `msg` is filled with packed action data
 */
__attribute__((wasm_wasm_import))
uint32_t read_action_data( void* msg, uint32_t len );

/**
 * Get the length of the current action's data field. This method is useful for dynamically sized actions
 *
 * @brief Get the length of current action's data field
 * @return the length of the current action's data field
 */
__attribute__((wasm_wasm_import))
uint32_t action_data_size();

/**
 *  Add the specified account to set of accounts to be notified
 *
 *  @brief Add the specified account to set of accounts to be notified
 *  @param name - name of the account to be verified
 */
__attribute__((wasm_wasm_import))
void require_recipient( capi_name name );

/**
 *  Verifies that name exists in the set of provided auths on a action. Throws if not found.
 *
 *  @brief Verify specified account exists in the set of provided auths
 *  @param name - name of the account to be verified
 */
__attribute__((wasm_wasm_import))
void require_auth( capi_name name );

 /**
 *  Verifies that name has auth.
 *
 *  @brief Verifies that name has auth.
 *  @param name - name of the account to be verified
 */
__attribute__((wasm_wasm_import))
bool has_auth( capi_name name );

/**
 *  Verifies that name exists in the set of provided auths on a action. Throws if not found.
 *
 *  @brief Verify specified account exists in the set of provided auths
 *  @param name - name of the account to be verified
 *  @param permission - permission level to be verified
 */
__attribute__((wasm_wasm_import))
void require_auth2( capi_name name, capi_name permission );

/**
 *  Verifies that @ref name is an existing account.
 *
 *  @brief Verifies that @ref name is an existing account.
 *  @param name - name of the account to check
 */
__attribute__((wasm_wasm_import))
bool is_account( capi_name name );

/**
 *  Send an inline action in the context of this action's parent transaction
 *
 *  @param serialized_action - serialized action
 *  @param size - size of serialized action in bytes
 *  @pre `serialized_action` is a valid pointer to an array at least `size` bytes long
 */
__attribute__((wasm_wasm_import))
void send_inline(char *serialized_action, size_t size);

/**
 *  Returns the time in microseconds from 1970 of the publication_time
 *  @brief Get the publication time
 *  @return the time in microseconds from 1970 of the publication_time
 */
__attribute__((wasm_wasm_import))
uint64_t  publication_time();

/**
 *  Get the current receiver of the action
 *  @brief Get the current receiver of the action
 *  @return the account which specifies the current receiver of the action
 */
__attribute__((wasm_wasm_import))
capi_name current_receiver();

#ifdef __cplusplus
}
#endif
/// @} action
