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
CMAKE_SOURCE_DIR = /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline

# Include any dependencies generated for this target.
include CMakeFiles/send_inline.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/send_inline.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/send_inline.dir/flags.make

CMakeFiles/send_inline.dir/send_inline.obj: CMakeFiles/send_inline.dir/flags.make
CMakeFiles/send_inline.dir/send_inline.obj: /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src/send_inline.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/send_inline.dir/send_inline.obj"
	/usr/local/eosio.cdt/bin/eosio-cpp  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/send_inline.dir/send_inline.obj -c /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src/send_inline.cpp

CMakeFiles/send_inline.dir/send_inline.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/send_inline.dir/send_inline.i"
	/usr/local/eosio.cdt/bin/eosio-cpp $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src/send_inline.cpp > CMakeFiles/send_inline.dir/send_inline.i

CMakeFiles/send_inline.dir/send_inline.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/send_inline.dir/send_inline.s"
	/usr/local/eosio.cdt/bin/eosio-cpp $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src/send_inline.cpp -o CMakeFiles/send_inline.dir/send_inline.s

# Object files for target send_inline
send_inline_OBJECTS = \
"CMakeFiles/send_inline.dir/send_inline.obj"

# External object files for target send_inline
send_inline_EXTERNAL_OBJECTS =

send_inline.wasm: CMakeFiles/send_inline.dir/send_inline.obj
send_inline.wasm: CMakeFiles/send_inline.dir/build.make
send_inline.wasm: CMakeFiles/send_inline.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable send_inline.wasm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/send_inline.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/send_inline.dir/build: send_inline.wasm

.PHONY : CMakeFiles/send_inline.dir/build

CMakeFiles/send_inline.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/send_inline.dir/cmake_clean.cmake
.PHONY : CMakeFiles/send_inline.dir/clean

CMakeFiles/send_inline.dir/depend:
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/src /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline /Users/xiaoyu/pegasus/eosio.cdt/examples/send_inline/build/send_inline/CMakeFiles/send_inline.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/send_inline.dir/depend
