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

# Include any dependencies generated for this target.
include deps/ponca/tests/src/CMakeFiles/projection.dir/depend.make

# Include the progress variables for this target.
include deps/ponca/tests/src/CMakeFiles/projection.dir/progress.make

# Include the compile flags for this target's objects.
include deps/ponca/tests/src/CMakeFiles/projection.dir/flags.make

deps/ponca/tests/src/CMakeFiles/projection.dir/projection.cpp.o: deps/ponca/tests/src/CMakeFiles/projection.dir/flags.make
deps/ponca/tests/src/CMakeFiles/projection.dir/projection.cpp.o: ../deps/ponca/tests/src/projection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aniket-pc/Desktop/GSoC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object deps/ponca/tests/src/CMakeFiles/projection.dir/projection.cpp.o"
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projection.dir/projection.cpp.o -c /home/aniket-pc/Desktop/GSoC/deps/ponca/tests/src/projection.cpp

deps/ponca/tests/src/CMakeFiles/projection.dir/projection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projection.dir/projection.cpp.i"
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aniket-pc/Desktop/GSoC/deps/ponca/tests/src/projection.cpp > CMakeFiles/projection.dir/projection.cpp.i

deps/ponca/tests/src/CMakeFiles/projection.dir/projection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projection.dir/projection.cpp.s"
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aniket-pc/Desktop/GSoC/deps/ponca/tests/src/projection.cpp -o CMakeFiles/projection.dir/projection.cpp.s

# Object files for target projection
projection_OBJECTS = \
"CMakeFiles/projection.dir/projection.cpp.o"

# External object files for target projection
projection_EXTERNAL_OBJECTS =

bin/projection: deps/ponca/tests/src/CMakeFiles/projection.dir/projection.cpp.o
bin/projection: deps/ponca/tests/src/CMakeFiles/projection.dir/build.make
bin/projection: /usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so
bin/projection: /usr/lib/x86_64-linux-gnu/libpthread.so
bin/projection: deps/ponca/tests/src/CMakeFiles/projection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aniket-pc/Desktop/GSoC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../bin/projection"
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/projection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
deps/ponca/tests/src/CMakeFiles/projection.dir/build: bin/projection

.PHONY : deps/ponca/tests/src/CMakeFiles/projection.dir/build

deps/ponca/tests/src/CMakeFiles/projection.dir/clean:
	cd /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src && $(CMAKE_COMMAND) -P CMakeFiles/projection.dir/cmake_clean.cmake
.PHONY : deps/ponca/tests/src/CMakeFiles/projection.dir/clean

deps/ponca/tests/src/CMakeFiles/projection.dir/depend:
	cd /home/aniket-pc/Desktop/GSoC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aniket-pc/Desktop/GSoC /home/aniket-pc/Desktop/GSoC/deps/ponca/tests/src /home/aniket-pc/Desktop/GSoC/build /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src /home/aniket-pc/Desktop/GSoC/build/deps/ponca/tests/src/CMakeFiles/projection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : deps/ponca/tests/src/CMakeFiles/projection.dir/depend

