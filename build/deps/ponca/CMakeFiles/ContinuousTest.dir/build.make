# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aniket-pc/Desktop/GSoC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aniket-pc/Desktop/GSoC/build

# Utility rule file for ContinuousTest.

# Include the progress variables for this target.
include deps/ponca/CMakeFiles/ContinuousTest.dir/progress.make

deps/ponca/CMakeFiles/ContinuousTest:
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca && /usr/bin/ctest -D ContinuousTest

ContinuousTest: deps/ponca/CMakeFiles/ContinuousTest
ContinuousTest: deps/ponca/CMakeFiles/ContinuousTest.dir/build.make

.PHONY : ContinuousTest

# Rule to build all files generated by this target.
deps/ponca/CMakeFiles/ContinuousTest.dir/build: ContinuousTest

.PHONY : deps/ponca/CMakeFiles/ContinuousTest.dir/build

deps/ponca/CMakeFiles/ContinuousTest.dir/clean:
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousTest.dir/cmake_clean.cmake
.PHONY : deps/ponca/CMakeFiles/ContinuousTest.dir/clean

deps/ponca/CMakeFiles/ContinuousTest.dir/depend:
	cd /home/aniket-pc/Desktop/GSoC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aniket-pc/Desktop/GSoC /home/aniket-pc/Desktop/GSoC/deps/ponca /home/aniket-pc/Desktop/GSoC/build /home/aniket-pc/Desktop/GSoC/build/deps/ponca /home/aniket-pc/Desktop/GSoC/build/deps/ponca/CMakeFiles/ContinuousTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : deps/ponca/CMakeFiles/ContinuousTest.dir/depend

