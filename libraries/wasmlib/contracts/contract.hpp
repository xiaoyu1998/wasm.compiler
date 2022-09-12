#pragma once

#include "../core/name.hpp"
#include "../core/datastream.hpp"


/**
 * @defgroup contract Contract
 * @ingroup contracts
 * @ingroup types
 * @brief Defines contract type which is %base class for every WASM contract
 */

/**
 * Helper macros to reduce the verbosity for common contracts
 * @ingroup contract
 */
#define CONTRACT class [[wasm::contract]]
#define ACTION   [[wasm::action]] void
#define TABLE struct [[wasm::table]]

namespace wasm {

/**
 * %Base class for WASM contract.
 *
 * @ingroup contract
 * @details %Base class for WASM contract. %A new contract should derive from this class, so it can make use of WASM_ABI macro.
 */
class contract {
   public:
      /**
       * Construct a new contract given the contract regid
       *
       * @param self - The regid of the account this contract is deployed on
       * @param first_receiver - The account the incoming action was first received at.
       * @param ds - The datastream used
       */
      contract( regid self, regid first_receiver, datastream<const char*> ds ):_self(self),_first_receiver(first_receiver),_ds(ds) {}

      /**
       * pre action execution for application contract extension
       */
      virtual void pre_action() {};
      /**
       * post action execution for application contract extension
       */
      virtual void post_action() {};

      /**
       *
       * Get this contract regid
       *
       * @return regid - The regid of this contract
       */
      inline regid get_self()const { return _self; }

      /**
       * The first_receiver regid of the action this contract is processing.
       *
       * @return regid - The first_receiver regid of the action this contract is processing.
       */
      [[deprecated]]
      inline regid get_code()const { return _first_receiver; }

      /**
       * The account the incoming action was first received at.
       *
       * @return regid - The first_receiver regid of the action this contract is processing.
       */
      inline regid get_first_receiver()const { return _first_receiver; }

      /**
       * Get the datastream for this contract
       *
       * @return datastream<const char*> - The datastream for this contract
       */
      inline datastream<const char*>& get_datastream() { return _ds; }

      /**
       * Get the datastream for this contract
       *
       * @return datastream<const char*> - The datastream for this contract
       */
      inline const datastream<const char*>& get_datastream()const { return _ds; }

   protected:
      /**
       * The regid of the account this contract is deployed on.
       */
      regid _self;

      /**
       * The account the incoming action was first received at.
       */
      regid _first_receiver;

      /**
       * The datastream for this contract
       */
      datastream<const char*> _ds = datastream<const char*>(nullptr, 0);

};
}
