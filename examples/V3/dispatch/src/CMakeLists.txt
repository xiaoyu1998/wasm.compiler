project(dispatch)
cmake_minimum_required(VERSION 3.5)

find_package(wasm.cdt)

add_contract( dispatch dispatch dispatch.cpp )
target_include_directories( dispatch PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( dispatch ${CMAKE_SOURCE_DIR}/../ricardian )
