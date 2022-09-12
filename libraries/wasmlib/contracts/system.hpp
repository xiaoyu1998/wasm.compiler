/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once
#include "../core/time.hpp"
#include "../core/check.hpp"
#include "../core/asset.hpp"
#include "../core/symbol.hpp"
#include "../core/regid.hpp"
#include "../core/hash256.hpp"
#include "../core/exception.hpp"

namespace wasm {
  namespace internal_use_do_not_use {
    extern "C" {
      __attribute__((wasm_wasm_import, noreturn))
      void wasm_exit( int32_t code );
    }
  }

  /**
   *  @addtogroup system System
   *  @ingroup contracts
   *  @brief Defines time related functions and wasm_exit
   */

   /**
    *  This method will abort execution of wasm without failing the contract. This is used to bypass all cleanup / destructors that would normally be called.
    *
    *  @ingroup system
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
   inline void wasm_exit( int32_t code ) {
     internal_use_do_not_use::wasm_exit(code);
   }


   /**
   *  Returns the time in seconds from 1970 of the current block as a block_timestamp
   *
   *  @ingroup system
   *  @return time in seconds from 1970 of the current block as a block_timestamp
   */
   uint64_t current_block_time();


   /**
   *  Returns the txid in hash of the current txid
   *
   *  @ingroup system
   *  @return time the txid in hash of the current txid
   */
   wasm::hash256 get_txid();


   /**
   *  Returns the maintainer of this contract
   *
   *  @ingroup system
   *  @return the mantainer of this contract
   */
   wasm::regid get_maintainer(wasm::regid contract);

  /**
   *  Returns the txid in hash from 1970 of the current txid
   *
   *  @ingroup system
   *  @return time the txid in hash from 1970 of the current txid
   */
   wasm::asset get_system_asset_price(symbol base, symbol quote);


   void         set_return(const vector<char>& bytes);
   vector<char> get_return(uint32_t size);
   void         make_log(regid owner, const string& topic, const vector<char>& value);


}
