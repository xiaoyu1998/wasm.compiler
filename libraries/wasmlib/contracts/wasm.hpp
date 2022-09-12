/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once
#include "../core/check.hpp"
#include "../core/print.hpp"
#include "inline_transaction.hpp"
#include "dispatcher.hpp"
#include "contract.hpp"
#include "system.hpp"

#ifndef WASM_NATIVE
static_assert( sizeof(long) == sizeof(int), "unexpected size difference" );
#endif

/**
 * @defgroup core Core API
 * @brief C++ Core API for chain-agnostic smart-contract functionality
 */

 /**
  * @defgroup contracts Contracts API
  * @brief C++ Contracts API for chain-dependent smart-contract functionality
  */

/**
 * @defgroup types Types
 * @brief C++ Types API for data layout of data-structures available for the WASM platform
 */
