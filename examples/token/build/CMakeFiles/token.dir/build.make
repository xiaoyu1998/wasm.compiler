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
CMAKE_SOURCE_DIR = /Users/xiaoyu/pegasus/wasm.cdt/examples/token

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build

# Utility rule file for token.

# Include the progress variables for this target.
include CMakeFiles/token.dir/progress.make

CMakeFiles/token: CMakeFiles/token-complete


CMakeFiles/token-complete: token-prefix/src/token-stamp/token-install
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-mkdir
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-download
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-update
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-patch
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-configure
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-build
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-install
CMakeFiles/token-complete: token-prefix/src/token-stamp/token-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'token'"
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles/token-complete
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-done

token-prefix/src/token-stamp/token-install: token-prefix/src/token-stamp/token-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'token'"
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-install

token-prefix/src/token-stamp/token-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'token'"
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/tmp
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-mkdir

token-prefix/src/token-stamp/token-download: token-prefix/src/token-stamp/token-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'token'"
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-download

token-prefix/src/token-stamp/token-update: token-prefix/src/token-stamp/token-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'token'"
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-update

token-prefix/src/token-stamp/token-patch: token-prefix/src/token-stamp/token-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'token'"
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-patch

token-prefix/src/token-stamp/token-configure: token-prefix/tmp/token-cfgcmd.txt
token-prefix/src/token-stamp/token-configure: token-prefix/src/token-stamp/token-update
token-prefix/src/token-stamp/token-configure: token-prefix/src/token-stamp/token-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'token'"
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && /usr/local/Cellar/cmake/3.14.5/bin/cmake -DCMAKE_TOOLCHAIN_FILE=/usr/local/wasm.cdt/lib/cmake/wasm.cdt/WasmWasmToolchain.cmake "-GUnix Makefiles" /Users/xiaoyu/pegasus/wasm.cdt/examples/token/src
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-configure

token-prefix/src/token-stamp/token-build: token-prefix/src/token-stamp/token-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'token'"
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && $(MAKE)

token-prefix/src/token-stamp/token-test: token-prefix/src/token-stamp/token-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No test step for 'token'"
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/token-prefix/src/token-stamp/token-test

token: CMakeFiles/token
token: CMakeFiles/token-complete
token: token-prefix/src/token-stamp/token-install
token: token-prefix/src/token-stamp/token-mkdir
token: token-prefix/src/token-stamp/token-download
token: token-prefix/src/token-stamp/token-update
token: token-prefix/src/token-stamp/token-patch
token: token-prefix/src/token-stamp/token-configure
token: token-prefix/src/token-stamp/token-build
token: token-prefix/src/token-stamp/token-test
token: CMakeFiles/token.dir/build.make

.PHONY : token

# Rule to build all files generated by this target.
CMakeFiles/token.dir/build: token

.PHONY : CMakeFiles/token.dir/build

CMakeFiles/token.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/token.dir/cmake_clean.cmake
.PHONY : CMakeFiles/token.dir/clean

CMakeFiles/token.dir/depend:
	cd /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xiaoyu/pegasus/wasm.cdt/examples/token /Users/xiaoyu/pegasus/wasm.cdt/examples/token /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build /Users/xiaoyu/pegasus/wasm.cdt/examples/token/build/CMakeFiles/token.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/token.dir/depend
