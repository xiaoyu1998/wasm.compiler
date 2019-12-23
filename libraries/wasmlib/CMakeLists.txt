file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(wasm
            wasmlib.cpp
            # crypto.cpp
            ${HEADERS})

add_library(wasm_malloc
            malloc.cpp
            ${HEADERS})

add_library(wasm_dsm
            simple_malloc.cpp
            ${HEADERS})

add_library(wasm_cmem
            memory.cpp
            ${HEADERS})

add_native_library(native_wasm
                   wasmlib.cpp
                   # crypto.cpp
                   malloc.cpp
                   ${HEADERS})

set_target_properties(wasm_malloc PROPERTIES LINKER_LANGUAGE C)

target_include_directories(wasm PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 ${CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( wasm c c++ )
add_dependencies( native_wasm wasm )

add_custom_command( TARGET wasm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wasm> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET wasm_malloc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wasm_malloc> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET wasm_dsm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wasm_dsm> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET wasm_cmem POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wasm_cmem> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_wasm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_wasm> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../wasmlib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
