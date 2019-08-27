configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasm-cc.cpp.in ${CMAKE_BINARY_DIR}/wasm-cc.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasm-cpp.cpp.in ${CMAKE_BINARY_DIR}/wasm-cpp.cpp)

add_tool(wasm-cc)
add_tool(wasm-cpp)
