add_custom_command( TARGET WasmClang POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin )
macro( wasm_clang_install file )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/wasm_llvm/bin)
   add_custom_command( TARGET WasmClang POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro( wasm_clang_install )

macro( wasm_clang_install_and_symlink file symlink )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/wasm_llvm/bin)
   add_custom_command( TARGET WasmClang POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   add_custom_command( TARGET WasmClang POST_BUILD COMMAND cd ${CMAKE_BINARY_DIR}/bin && ln -sf ${file} ${symlink} )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
endmacro( wasm_clang_install_and_symlink )

macro( wasm_tool_install file )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
   add_custom_command( TARGET WasmTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro( wasm_tool_install )

macro( wasm_tool_install_and_symlink file symlink )
   set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
   add_custom_command( TARGET WasmTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file} ${CMAKE_BINARY_DIR}/bin/ )
   install(FILES ${BINARY_DIR}/${file}
      DESTINATION ${CDT_INSTALL_PREFIX}/bin
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/bin)")
   install(CODE "execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink ${CDT_INSTALL_PREFIX}/bin/${file} ${CMAKE_INSTALL_PREFIX}/bin/${symlink})")
endmacro( wasm_tool_install_and_symlink )

macro( wasm_libraries_install)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lib)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION ${CDT_INSTALL_PREFIX}/lib)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CDT_INSTALL_PREFIX}/include)
endmacro( wasm_libraries_install )

wasm_clang_install_and_symlink(llvm-ranlib wasm-ranlib)
wasm_clang_install_and_symlink(llvm-ar wasm-ar)
wasm_clang_install(opt)
wasm_clang_install(llc)
wasm_clang_install(lld)
wasm_clang_install(ld.lld)
wasm_clang_install(ld64.lld)
wasm_clang_install(clang-7)
wasm_clang_install(wasm-ld)
wasm_tool_install_and_symlink(wasm-pp wasm-pp)
wasm_tool_install_and_symlink(wast2wasm wast2wasm)
wasm_tool_install_and_symlink(wasm2wast wasm2wast)
wasm_tool_install_and_symlink(wasm-cc wasm-cc)
wasm_tool_install_and_symlink(wasm-cpp wasm-cpp)
wasm_tool_install_and_symlink(wasm-lld wasm-lld)
wasm_clang_install(../lib/LLVMWasmApply${CMAKE_SHARED_LIBRARY_SUFFIX})
wasm_clang_install(../lib/wasm_plugin${CMAKE_SHARED_LIBRARY_SUFFIX})
wasm_libraries_install()
