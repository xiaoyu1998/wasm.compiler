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
CMAKE_SOURCE_DIR = /Users/xiaoyu/pegasus/eosio.cdt/examples/hello

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build

# Utility rule file for hello_project.

# Include the progress variables for this target.
include CMakeFiles/hello_project.dir/progress.make

CMakeFiles/hello_project: CMakeFiles/hello_project-complete


CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-install
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-mkdir
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-download
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-update
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-patch
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-configure
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-build
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-install
CMakeFiles/hello_project-complete: hello_project-prefix/src/hello_project-stamp/hello_project-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'hello_project'"
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles/hello_project-complete
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-done

hello_project-prefix/src/hello_project-stamp/hello_project-install: hello_project-prefix/src/hello_project-stamp/hello_project-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'hello_project'"
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-install

hello_project-prefix/src/hello_project-stamp/hello_project-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'hello_project'"
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/src
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/tmp
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E make_directory /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-mkdir

hello_project-prefix/src/hello_project-stamp/hello_project-download: hello_project-prefix/src/hello_project-stamp/hello_project-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'hello_project'"
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	/usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-download

hello_project-prefix/src/hello_project-stamp/hello_project-update: hello_project-prefix/src/hello_project-stamp/hello_project-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'hello_project'"
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-update

hello_project-prefix/src/hello_project-stamp/hello_project-patch: hello_project-prefix/src/hello_project-stamp/hello_project-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'hello_project'"
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/src && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-patch

hello_project-prefix/src/hello_project-stamp/hello_project-configure: hello_project-prefix/tmp/hello_project-cfgcmd.txt
hello_project-prefix/src/hello_project-stamp/hello_project-configure: hello_project-prefix/src/hello_project-stamp/hello_project-update
hello_project-prefix/src/hello_project-stamp/hello_project-configure: hello_project-prefix/src/hello_project-stamp/hello_project-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'hello_project'"
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && /usr/local/Cellar/cmake/3.14.5/bin/cmake -DCMAKE_TOOLCHAIN_FILE=/usr/local/eosio.cdt/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake "-GUnix Makefiles" /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/src
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-configure

hello_project-prefix/src/hello_project-stamp/hello_project-build: hello_project-prefix/src/hello_project-stamp/hello_project-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'hello_project'"
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && $(MAKE)

hello_project-prefix/src/hello_project-stamp/hello_project-test: hello_project-prefix/src/hello_project-stamp/hello_project-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No test step for 'hello_project'"
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E echo_append
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello && /usr/local/Cellar/cmake/3.14.5/bin/cmake -E touch /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/hello_project-prefix/src/hello_project-stamp/hello_project-test

hello_project: CMakeFiles/hello_project
hello_project: CMakeFiles/hello_project-complete
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-install
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-mkdir
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-download
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-update
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-patch
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-configure
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-build
hello_project: hello_project-prefix/src/hello_project-stamp/hello_project-test
hello_project: CMakeFiles/hello_project.dir/build.make

.PHONY : hello_project

# Rule to build all files generated by this target.
CMakeFiles/hello_project.dir/build: hello_project

.PHONY : CMakeFiles/hello_project.dir/build

CMakeFiles/hello_project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hello_project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hello_project.dir/clean

CMakeFiles/hello_project.dir/depend:
	cd /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xiaoyu/pegasus/eosio.cdt/examples/hello /Users/xiaoyu/pegasus/eosio.cdt/examples/hello /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build /Users/xiaoyu/pegasus/eosio.cdt/examples/hello/build/CMakeFiles/hello_project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hello_project.dir/depend
