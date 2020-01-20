project(uniswap)

set(WASM_WASM_OLD_BEHAVIOR "Off")
find_package(wasm.cdt)

add_contract( uniswap uniswap uniswap.cpp )
target_include_directories( uniswap PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( uniswap ${CMAKE_SOURCE_DIR}/../ricardian )
