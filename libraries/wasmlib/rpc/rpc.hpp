#pragma once
#include <vector>

namespace wasm {

/**
 *  emit result value for wasm_getresult.
 *
 *  @ingroup emit_result_value
 */
void emit_result_value(const string &name, const string &type, const std::vector<char> &value);


/**
 *  set the result for wasm rpc.
 *
 *  @ingroup emit_result
 */
  template <typename T>
  inline void emit_result(const string &name, const string &type, const T &value) {
    auto v = wasm::pack<T>(value);
    emit_result_value(name, type, v);
  }

} // namespace wasm
