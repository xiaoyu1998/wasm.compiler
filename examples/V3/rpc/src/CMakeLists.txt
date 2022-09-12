project(rpc)
cmake_minimum_required(VERSION 3.5)

find_package(wasm.cdt)

add_contract( rpc rpc rpc_get.cpp )
target_include_directories( rpc PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( rpc ${CMAKE_SOURCE_DIR}/../ricardian )
