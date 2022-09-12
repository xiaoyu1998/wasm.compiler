project(index)
cmake_minimum_required(VERSION 3.5)

find_package(wasm.cdt)

add_contract( index index index.cpp )
target_include_directories( index PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( index ${CMAKE_SOURCE_DIR}/../ricardian )
