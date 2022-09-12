project(mpas)
cmake_minimum_required(VERSION 3.5)

find_package(wasm.cdt)

add_contract( mpas mpas mpas.cpp )
target_include_directories( mpas PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( mpas ${CMAKE_SOURCE_DIR}/../ricardian )
