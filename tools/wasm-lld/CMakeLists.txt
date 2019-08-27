configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasm-lld.cpp.in ${CMAKE_BINARY_DIR}/wasm-lld.cpp)

add_tool(wasm-lld)
