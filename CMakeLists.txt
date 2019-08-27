cmake_minimum_required(VERSION 3.5)
project(wasm_cdt)

find_program(SCCACHE_FOUND sccache)
if (SCCACHE_FOUND)
   message(STATUS "Using sccache")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
else()
   find_program(CCACHE_FOUND ccache)
   if (CCACHE_FOUND)
      message(STATUS "Using ccache")
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
   endif()
endif()


set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)
set(VERSION_PATCH 2)
#set(VERSION_SUFFIX rc2)

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set(CDT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/wasm.cdt)

include(GNUInstallDirs)

include(modules/ClangExternalProject.txt)
include(modules/ToolsExternalProject.txt)

set(WASM_SDK_BUILD true)

### Configure the WasmWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/modules/wasm.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/wasm.cdt/wasm.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/WasmCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/wasm.cdt/WasmCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/WasmWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/wasm.cdt/WasmWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CDT_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/modules/wasm.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/wasm.cdt-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/wasm.cdt-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/wasm.cdt)
configure_file(${CMAKE_SOURCE_DIR}/modules/WasmCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/WasmCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/WasmWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/WasmWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/WasmWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/wasm.cdt)
install(FILES ${CMAKE_BINARY_DIR}/modules/WasmCDTMacros.cmake DESTINATION ${CDT_INSTALL_PREFIX}/lib/cmake/wasm.cdt)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/WasmCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/WasmCDTMacrosPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/WasmWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/WasmWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/wasm.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/wasm.cdt-config.cmake.package @ONLY)

include(modules/LibrariesExternalProject.txt)

include(modules/InstallCDT.cmake)

configure_file(${CMAKE_SOURCE_DIR}/imports/wasm.imports.in ${CMAKE_BINARY_DIR}/wasm.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/wasm.imports DESTINATION ${CDT_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${CDT_INSTALL_PREFIX}/scripts)

# section for package construction
set(VENDOR "wasm.one")
set(PROJECT_NAME "wasm.cdt")
set(DESC "Toolchain and supporting tools for the WASM.IO platform")
set(URL "https://github.com/wasm/wasm.cdt")
set(EMAIL "support@wasm.one")
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_package.sh.in ${CMAKE_BINARY_DIR}/packages/generate_package.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_deb.sh ${CMAKE_BINARY_DIR}/packages/generate_deb.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_bottle.sh ${CMAKE_BINARY_DIR}/packages/generate_bottle.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_rpm.sh ${CMAKE_BINARY_DIR}/packages/generate_rpm.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_tarball.sh ${CMAKE_BINARY_DIR}/packages/generate_tarball.sh COPYONLY)

# add licenses
configure_file(${CMAKE_SOURCE_DIR}/wasm_llvm/LICENSE.TXT ${CMAKE_BINARY_DIR}/licenses/llvm.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/boost/boost.license ${CMAKE_BINARY_DIR}/licenses/boost.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/external/wabt/LICENSE ${CMAKE_BINARY_DIR}/licenses/wabt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/jsoncons/LICENSE ${CMAKE_BINARY_DIR}/licenses/jsoncons.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/wasm.cdt.license COPYONLY)

# include(modules/TestsExternalProject.txt)

# include(CTest)
# enable_testing()
# add_subdirectory(tests)
