project(crypto)

set(WASM_WASM_OLD_BEHAVIOR "Off")
find_package(wasm.cdt)

add_contract( crypto crypto crypto.cpp )
target_include_directories( crypto PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( crypto ${CMAKE_SOURCE_DIR}/../ricardian )
