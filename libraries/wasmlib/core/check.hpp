/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once

#include <alloca.h>
#include <string>

namespace wasm {

   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((wasm_wasm_import))
         void wasm_assert( uint32_t test, const char* msg );

         __attribute__((wasm_wasm_import))
         void wasm_assert_message( uint32_t test, const char* msg, uint32_t msg_len );

         __attribute__((wasm_wasm_import))
         void wasm_assert_code( uint32_t test, uint64_t code );
      }
   }

   /**
    *  @defgroup system System
    *  @ingroup core
    *  @brief Defines wrappers over wasm_assert
    */

   /**
    *  Assert if the predicate fails and use the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  wasm::check(a == b, "a does not equal b");
    *  @endcode
    */
   inline void check(bool pred, const char* msg) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert(false, msg);
      }
   }

    /**
    *  Assert if the predicate fails and use the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  wasm::check(a == b, "a does not equal b");
    *  @endcode
    */
   inline void check(bool pred, const std::string& msg) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert(false, msg.c_str());
      }
   }

   /**
    *  Assert if the predicate fails and use the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  wasm::check(a == b, "a does not equal b");
    *  @endcode
    */
   inline void check(bool pred, std::string&& msg) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert(false, msg.c_str());
      }
   }

   /**
    *  Assert if the predicate fails and use a subset of the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  const char* msg = "a does not equal b b does not equal a";
    *  wasm::check(a == b, "a does not equal b", 18);
    *  @endcode
    */
   inline void check(bool pred, const char* msg, size_t n) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert_message(false, msg, n);
      }
   }

   /**
    *  Assert if the predicate fails and use a subset of the supplied message.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  std::string msg = "a does not equal b b does not equal a";
    *  wasm::check(a == b, msg, 18);
    *  @endcode
    */
   inline void check(bool pred, const std::string& msg, size_t n) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert_message(false, msg.c_str(), n);
      }
   }

    /**
    *  Assert if the predicate fails and use the supplied error code.
    *
    *  @ingroup system
    *
    *  Example:
    *  @code
    *  wasm::check(a == b, 13);
    *  @endcode
    */
   inline void check(bool pred, uint64_t code) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert_code(false, code);
      }
   }

   template <typename... Ts>
   inline std::string format_str(const char *fmt, const Ts &... vals) {
      auto size = snprintf(nullptr, 0, fmt, vals...);
      if (size <= 0) return "error fmt: " + std::string(fmt);
      std::string str;
      str.resize(size + 1);
      auto new_size = snprintf((char*)str.data(), size + 1, fmt, vals...);
      str.resize(new_size);
      return str;
   }

   template <typename... Ts>
   inline void check_fmt(bool pred, const char *fmt, const Ts &... vals) {
      if (!pred) {
         internal_use_do_not_use::wasm_assert(false, format_str(fmt, vals...).c_str());
      }
   }


} // namespace wasm
