#pragma once
#include "strings.hpp"

namespace wasm {
	struct exception : public std::exception {
		virtual const char* what()const throw()=0;
		virtual uint32_t code()const throw()=0;
	};

	#define WASM_DECLARE_EXCEPTION(name, _code, _what)                                \
		struct name : public wasm::exception {                                        \
		  name(){}                                                                    \
		  name(const char* msg) : msg(msg) {}                                         \
		  virtual const char* what()const throw() { return _what; }                   \
		  virtual const char* detail()const throw() { return msg; }                   \
		  uint32_t code()const { return _code; }                                      \
		  const char* msg = nullptr;                                                  \
		};


	inline void check(bool pred, const wasm::exception& e, const char *s) {
	  if (!pred) {
	  	 std::string str = "contract_err_code:" + to_string(e.code()) + " " + to_string(s);
	     internal_use_do_not_use::wasm_assert(false, str.c_str());
	  }
	}

	template <typename Arg, typename... Args>
	inline void check(bool pred, const wasm::exception& e, const char *s, Arg val, Args... rest) {
	  if (!pred) {
	  	 std::string str = "contract_err_code:" + to_string(e.code()) + " " + to_string(s, val, rest...);
	     internal_use_do_not_use::wasm_assert(false, str.c_str());
	  }
	}

} // namespace wasm
