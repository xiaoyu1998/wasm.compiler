#pragma once

/**
 * Define specific things for the wasm system
 */

extern "C" {
   void wasm_assert(unsigned int, const char*);
   void __cxa_pure_virtual() { wasm_assert(false, "pure virtual method called"); }
}
