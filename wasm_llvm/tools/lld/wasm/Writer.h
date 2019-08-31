//===- Writer.h -------------------------------------------------*- C++ -*-===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_WASM_WRITER_H
#define LLD_WASM_WRITER_H

namespace lld {
namespace wasm {

void writeResult(bool is_entry_defined);

} // namespace wasm
} // namespace lld

#endif
