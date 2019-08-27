project(wasm_libraries)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

list(APPEND CMAKE_MODULE_PATH ${WASM_CDT_BIN})
include(WasmCDTMacros)

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O3 -Wall ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O3 -Wall ")

add_subdirectory(libc)
add_subdirectory(libc++)
add_subdirectory(boost)
add_subdirectory(wasmlib)
