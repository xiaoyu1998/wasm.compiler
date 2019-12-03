project(prints)

set(WASM_WASM_OLD_BEHAVIOR "Off")
find_package(wasm.cdt)

add_contract( prints prints prints.cpp )
target_include_directories( prints PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( prints ${CMAKE_SOURCE_DIR}/../ricardian )
