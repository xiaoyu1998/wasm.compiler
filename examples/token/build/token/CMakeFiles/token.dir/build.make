# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.14.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.14.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token

# Include any dependencies generated for this target.
include CMakeFiles/token.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/token.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/token.dir/flags.make

CMakeFiles/token.dir/token.obj: CMakeFiles/token.dir/flags.make
CMakeFiles/token.dir/token.obj: /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src/token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/token.dir/token.obj"
	/Users/xiaoyu/pegasus/wasm.cdt/build/bin/wasm-cpp  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/token.dir/token.obj -c /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src/token.cpp

CMakeFiles/token.dir/token.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/token.dir/token.i"
	/Users/xiaoyu/pegasus/wasm.cdt/build/bin/wasm-cpp $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src/token.cpp > CMakeFiles/token.dir/token.i

CMakeFiles/token.dir/token.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/token.dir/token.s"
	/Users/xiaoyu/pegasus/wasm.cdt/build/bin/wasm-cpp $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src/token.cpp -o CMakeFiles/token.dir/token.s

# Object files for target token
token_OBJECTS = \
"CMakeFiles/token.dir/token.obj"

# External object files for target token
token_EXTERNAL_OBJECTS =

token.wasm: CMakeFiles/token.dir/token.obj
token.wasm: CMakeFiles/token.dir/build.make
token.wasm: CMakeFiles/token.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable token.wasm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/token.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/token.dir/build: token.wasm

.PHONY : CMakeFiles/token.dir/build

CMakeFiles/token.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/token.dir/cmake_clean.cmake
.PHONY : CMakeFiles/token.dir/clean

CMakeFiles/token.dir/depend:
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token/CMakeFiles/token.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/token.dir/depend
