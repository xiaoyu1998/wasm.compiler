/**
 *  @file
 *  @copyright defined in wasm/LICENSE
 */
#pragma once
#include <cstdlib>

#include "../core/serialize.hpp"
#include "../core/datastream.hpp"
#include "../core/name.hpp"
#include "../core/ignore.hpp"
#include "../core/time.hpp"

#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_tuple.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/facilities/overload.hpp>

// using namespace std;
using namespace wasm;

namespace wasm {

   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((wasm_wasm_import))
         uint32_t read_action_data( void* msg, uint32_t len );

         __attribute__((wasm_wasm_import))
         uint32_t action_data_size();

         __attribute__((wasm_wasm_import))
         void require_recipient( uint64_t name );

         __attribute__((wasm_wasm_import))
         void require_auth( uint64_t name );

         __attribute__((wasm_wasm_import))
         bool has_auth( uint64_t name );

         __attribute__((wasm_wasm_import))
         void require_auth2( uint64_t name, uint64_t permission );

         __attribute__((wasm_wasm_import))
         bool is_account( uint64_t name );

         __attribute__((wasm_wasm_import))
         void send_inline(char *serialized_transaction, size_t size);

         __attribute__((wasm_wasm_import))
         void send_context_free_inline(char *serialized_transaction, size_t size);

         __attribute__((wasm_wasm_import))
         uint64_t  publication_time();

         __attribute__((wasm_wasm_import))
         uint64_t current_receiver();
      }
   };

   /**
    *  @defgroup transaction Action
    *  @ingroup contracts
    *  @brief Defines type-safe C++ wrappers for querying transaction and sending transaction
    *  @note There are some methods from the @ref transaction that can be used directly from C++
    */

   /**
    *  @ingroup transaction
    *  @return Unpacked transaction data casted as T.
    *
    *  Example:
    *
    *  @code
    *  struct dummy_transaction {
    *    char a; //1
    *    unsigned long long b; //8
    *    int  c; //4
    *
    *    WASMLIB_SERIALIZE( dummy_transaction, (a)(b)(c) )
    *  };
    *  dummy_transaction msg = unpack_transaction_data<dummy_transaction>();
    *  @endcode
    */
   template<typename T>
   T unpack_action_data() {
      constexpr size_t max_stack_buffer_size = 512;
      size_t size = internal_use_do_not_use::action_data_size();
      char* buffer = (char*)( max_stack_buffer_size < size ? malloc(size) : alloca(size) );
      internal_use_do_not_use::read_action_data( buffer, size );
      return unpack<T>( buffer, size );
   }

   /**
    *  Add the specified account to set of accounts to be notified
    *
    *  @ingroup transaction
    *  @brief Add the specified account to set of accounts to be notified
    *  @param notify_account - name of the account to be verified
    */
   inline void require_recipient( name notify_account ){
      internal_use_do_not_use::require_recipient( notify_account.value );
   }

   /**
    *  All of the listed accounts will be added to the set of accounts to be notified
    *
    *  This helper method enables you to add multiple accounts to accounts to be notified list with a single
    *  call rather than having to call the similar C API multiple times.
    *
    *  @ingroup transaction
    *  @param notify_account account to be notified
    *  @param remaining_accounts accounts to be notified
    *  @note transaction.code is also considered as part of the set of notified accounts
    *
    *  Example:
    *
    *  @code
    *  require_recipient("Account1"_n, "Account2"_n, "Account3"_n); // throws exception if any of them not in set.
    *  @endcode
    */
   template<typename... accounts>
   void require_recipient( name notify_account, accounts... remaining_accounts ){
      internal_use_do_not_use::require_recipient( notify_account.value );
      require_recipient( remaining_accounts... );
   }

   /**
    *  Verifies that @ref name exists in the set of provided auths on a transaction. Fails if not found.
    *
    *  @ingroup transaction
    *  @param name - name of the account to be verified
    */
   inline void require_auth( name n ) {
      internal_use_do_not_use::require_auth( n.value );
   }

   /**
   *  Returns the time in microseconds from 1970 of the publication_time
   *
   *  @ingroup transaction
   *  @return the time in microseconds from 1970 of the publication_time
   */
   inline time_point  publication_time() {
     return time_point( microseconds ( internal_use_do_not_use::publication_time() ) );
   }

   /**
   *  Get the current receiver of the transaction
   *  @return the account which specifies the current receiver of the transaction
   */
   inline name current_receiver() {
     return name{internal_use_do_not_use::current_receiver()};
   }

   /**
    *  Copy up to length bytes of current transaction data to the specified location
    *
    *  @ingroup transaction
    *  @param msg - a pointer where up to length bytes of the current transaction data will be copied
    *  @param len - len of the current transaction data to be copied, 0 to report required size
    *  @return the number of bytes copied to msg, or number of bytes that can be copied if len==0 passed
    *  @pre `msg` is a valid pointer to a range of memory at least `len` bytes long
    *  @post `msg` is filled with packed transaction data
    */
   inline uint32_t read_action_data( void* msg, uint32_t len ) {
     return internal_use_do_not_use::read_action_data(msg, len);
   }

   /**
    * Get the length of the current transaction's data field. This method is useful for dynamically sized transactions
    *
    * @return the length of the current transaction's data field
    */
   inline uint32_t action_data_size() {
     return internal_use_do_not_use::action_data_size();
   }

   /**
    *  Verifies that @ref n has auth.
    *
    *  @ingroup transaction
    *  @param n - name of the account to be verified
    */
   inline bool has_auth( name n ) {
      return internal_use_do_not_use::has_auth( n.value );
   }

   /**
    *  Verifies that @ref n is an existing account.
    *
    *  @ingroup transaction
    *  @param n - name of the account to check
    */
   inline bool is_account( name n ) {
      return internal_use_do_not_use::is_account( n.value );
   }

   /**
    *  This is the packed representation of an transaction along with
    *  meta-data about the authorization levels.
    *
    *  @ingroup transaction
    */
   struct transaction {
      /**
       *  Name of the account the transaction is intended for
       */
      name                       account;

      /**
       *  Name of the transaction
       */
      name                       name;

      /**
       *  Payload data
       */
      std::vector<char>               data;

      /**
       *  Default Constructor
       */
      transaction() = default;

      /**
       * Construct a new transaction object with the given permission, transaction receiver, transaction name, transaction struct
       *
       * @tparam T  - Type of transaction struct, must be serializable by `pack(...)`
       * @param auth - The permissions that authorizes this transaction
       * @param a -  The name of the account this transaction is intended for (transaction receiver)
       * @param n - The name of the transaction
       * @param value - The transaction struct that will be serialized via pack into data
       */
      template<typename T>
      transaction( struct name a, struct name n, T&& value )
      :account(a), name(n), data(pack(std::forward<T>(value))) {}


      /// @cond INTERNAL

      WASMLIB_SERIALIZE( transaction, (account)(name)(data) )

      /// @endcond

      /**
       * Send the transaction as inline transaction
       */
      void send() const {
         auto serialize = pack(*this);
         internal_use_do_not_use::send_inline(serialize.data(), serialize.size());
      }

      /**
       * Retrieve the unpacked data as T
       *
       * @tparam T expected type of data
       * @return the transaction data
       */
      template<typename T>
      T data_as() {
         return unpack<T>( &data[0], data.size() );
      }

   };



   namespace detail {

      /// @cond INTERNAL

      template <typename T>
      struct unwrap { typedef T type; };

      template <typename T>
      struct unwrap<ignore<T>> { typedef T type; };

      template <typename R, typename Act, typename... Args>
      auto get_args(R(Act::*p)(Args...)) {
         return std::tuple<std::decay_t<typename unwrap<Args>::type>...>{};
      }

      template <typename R, typename Act, typename... Args>
      auto get_args_nounwrap(R(Act::*p)(Args...)) {
         return std::tuple<std::decay_t<Args>...>{};
      }

      template <auto Action>
      using deduced = decltype(get_args(Action));

      template <auto Action>
      using deduced_nounwrap = decltype(get_args_nounwrap(Action));

      template <typename T>
      struct convert { typedef T type; };

      template <>
      struct convert<const char*> { typedef std::string type; };

      template <>
      struct convert<char*> { typedef std::string type; };

      template <typename T, typename U>
      struct is_same { static constexpr bool value = std::is_convertible<T,U>::value; };

      template <typename U>
      struct is_same<bool,U> { static constexpr bool value = std::is_integral<U>::value; };

      template <typename T>
      struct is_same<T,bool> { static constexpr bool value = std::is_integral<T>::value; };

      template <size_t N, size_t I, auto Arg, auto... Args>
      struct get_nth_impl { static constexpr auto value  = get_nth_impl<N,I+1,Args...>::value; };

      template <size_t N, auto Arg, auto... Args>
      struct get_nth_impl<N, N, Arg, Args...> { static constexpr auto value = Arg; };

      template <size_t N, auto... Args>
      struct get_nth { static constexpr auto value  = get_nth_impl<N,0,Args...>::value; };

      template <auto Action, size_t I, typename T, typename... Rest>
      struct check_types {
         static_assert(detail::is_same<typename convert<T>::type, typename convert<typename std::tuple_element<I, deduced<Action>>::type>::type>::value);
         using type = check_types<Action, I+1, Rest...>;
         static constexpr bool value = true;
      };
      template <auto Action, size_t I, typename T>
      struct check_types<Action, I, T> {
         static_assert(detail::is_same<typename convert<T>::type, typename convert<typename std::tuple_element<I, deduced<Action>>::type>::type>::value);
         static constexpr bool value = true;
      };

      template <auto Action, typename... Ts>
      constexpr bool type_check() {
         static_assert(sizeof...(Ts) == std::tuple_size<deduced<Action>>::value);
         return check_types<Action, 0, Ts...>::value;
      }

      /// @endcond
   }

   /**
    * Wrapper for an transaction object. 
    *
    * @brief Used to wrap an a particular transaction to simplify the process of other contracts sending inline transactions to "wrapped" transaction. 
    * Example:
    * @code
    * // defined by contract writer of the transactions
    * using transfer act = transaction_wrapper<"transfer"_n, &token::transfer>( *this, transfer, {st.issuer, to, quantity, memo} );
    * // usage by different contract writer
    * transfer_act{"wasm.token"_n}.send(st.issuer, to, quantity, memo);
    * // or
    * transfer_act trans_transaction{ "wasm.token"_n};
    * trans_transaction.send(st.issuer, to, quantity, memo);
    * @endcode
    */
   template <wasm::name::raw Name, auto Action>
   struct transaction_wrapper {
      template <typename Code>
      constexpr transaction_wrapper(Code&& code)
         : code_name(std::forward<Code>(code)) {}

      static constexpr wasm::name transaction_name = wasm::name(Name);
      wasm::name code_name;

      static constexpr auto get_mem_ptr() {
         return Action;
      }

      template <typename... Args>
      transaction to_transaction(Args&&... args)const {
         static_assert(detail::type_check<Action, Args...>());
         return transaction(code_name, transaction_name, detail::deduced<Action>{std::forward<Args>(args)...});
      }
      template <typename... Args>
      void send(Args&&... args)const {
         to_transaction(std::forward<Args>(args)...).send();
      }

   };

   template <wasm::name::raw Name, auto... Actions>
   struct variant_transaction_wrapper {
      template <typename Code>
      constexpr variant_transaction_wrapper(Code&& code)
         : code_name(std::forward<Code>(code)) {}

      static constexpr wasm::name transaction_name = wasm::name(Name);
      wasm::name code_name;

      template <size_t Variant>
      static constexpr auto get_mem_ptr() {
         return detail::get_nth<Variant, Actions...>::value;
      }

      template <size_t Variant, typename... Args>
      transaction to_transaction(Args&&... args)const {
         static_assert(detail::type_check<detail::get_nth<Variant, Actions...>::value, Args...>());
         unsigned_int var = Variant;
         return transaction(code_name, transaction_name, std::tuple_cat(std::make_tuple(var), detail::deduced<detail::get_nth<Variant, Actions...>::value>{std::forward<Args>(args)...}));
      }

      template <size_t Variant, typename... Args>
      void send(Args&&... args)const {
         to_transaction<Variant>(std::forward<Args>(args)...).send();
      }

   };

   template<typename... Args>
   void dispatch_inline( name code, name act,
                         std::tuple<Args...> args ) {
      transaction( code, act, std::move(args) ).send();
   }

   template<typename, name::raw>
   struct inline_dispatcher;


   template<typename T, name::raw Name, typename... Args>
   struct inline_dispatcher<void(T::*)(Args...), Name> {
      static void call(name code, std::tuple<Args...> args) {
         dispatch_inline(code, name(Name), std::move(args));
      }
   };

} // namespace wasm

#define INLINE_TRANSACTION_SENDER3( CONTRACT_CLASS, FUNCTION_NAME, TRANSACTION_NAME  )\
::wasm::inline_dispatcher<decltype(&CONTRACT_CLASS::FUNCTION_NAME), TRANSACTION_NAME>::call

#define INLINE_TRANSACTION_SENDER2( CONTRACT_CLASS, NAME )\
INLINE_TRANSACTION_SENDER3( CONTRACT_CLASS, NAME, ::wasm::name(#NAME) )

#define INLINE_TRANSACTION_SENDER(...) BOOST_PP_OVERLOAD(INLINE_TRANSACTION_SENDER,__VA_ARGS__)(__VA_ARGS__)

/**
 * Send an inline-transaction from inside a contract.
 *
 * @brief A macro to simplify calling inline transactions
 * @details The send inline transaction macro is intended to simplify the process of calling inline transactions. When calling new transactions from existing transactions.
 * Inline transactions are executed as part of the current transtransaction. This macro
 * creates an @ref transaction using the supplied parameters and automatically calls transaction.send() on this newly created transaction.
 *
 * Example:
 * @code
 * SEND_INLINE_TRANSACTION( *this, transfer, {st.issuer, to, quantity, memo} );
 * @endcode
 * 
 * The example above is taken from wasm.token. 
 * This example:  
 *       uses the passed in, dereferenced `this` pointer, to call this.get_self() i.e. the wasm.token contract; 
 *       calls the wasm.token::transfer() transaction; 
 *       uses parameters st.issuer, to, quantity and memo. 
 * This macro creates an transaction struct used to 'send()' (call) transfer(account_name from, account_name to, asset quantity, string memo)
 * 
 * @param CONTRACT - The contract to call, which contains the transaction being sent, maps to the @ref account
 * @param NAME - The name of the transaction to be called, maps to a @ref name
 * @param ... - The authorising permission, maps to an @ref authorization , followed by the parameters of the transaction, maps to a @ref data.
 */

#define SEND_INLINE_TRANSACTION( CONTRACT, NAME, DATA )\
INLINE_TRANSACTION_SENDER(std::decay_t<decltype(CONTRACT)>, NAME)( (CONTRACT).get_self(),\
BOOST_PP_TUPLE_ENUM(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), BOOST_PP_VARIADIC_TO_TUPLE(__VA_ARGS__)) );
