project(hashtimelock)

set(WASM_WASM_OLD_BEHAVIOR "Off")
find_package(wasm.cdt)

add_contract( hashtimelock hashtimelock hashtimelock.cpp )
target_include_directories( hashtimelock PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( hashtimelock ${CMAKE_SOURCE_DIR}/../ricardian )
