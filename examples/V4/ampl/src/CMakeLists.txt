project(ampl)
cmake_minimum_required(VERSION 3.5)

find_package(wasm.cdt)

add_contract( ampl ampl ampl.cpp )
target_include_directories( ampl PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( ampl ${CMAKE_SOURCE_DIR}/../ricardian )
