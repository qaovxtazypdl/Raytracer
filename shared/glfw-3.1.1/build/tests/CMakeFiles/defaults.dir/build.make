# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.4.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.4.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/defaults.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/defaults.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/defaults.dir/flags.make

tests/CMakeFiles/defaults.dir/defaults.c.o: tests/CMakeFiles/defaults.dir/flags.make
tests/CMakeFiles/defaults.dir/defaults.c.o: ../tests/defaults.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/defaults.dir/defaults.c.o"
	cd /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/defaults.dir/defaults.c.o   -c /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/tests/defaults.c

tests/CMakeFiles/defaults.dir/defaults.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/defaults.dir/defaults.c.i"
	cd /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/tests/defaults.c > CMakeFiles/defaults.dir/defaults.c.i

tests/CMakeFiles/defaults.dir/defaults.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/defaults.dir/defaults.c.s"
	cd /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/tests/defaults.c -o CMakeFiles/defaults.dir/defaults.c.s

tests/CMakeFiles/defaults.dir/defaults.c.o.requires:

.PHONY : tests/CMakeFiles/defaults.dir/defaults.c.o.requires

tests/CMakeFiles/defaults.dir/defaults.c.o.provides: tests/CMakeFiles/defaults.dir/defaults.c.o.requires
	$(MAKE) -f tests/CMakeFiles/defaults.dir/build.make tests/CMakeFiles/defaults.dir/defaults.c.o.provides.build
.PHONY : tests/CMakeFiles/defaults.dir/defaults.c.o.provides

tests/CMakeFiles/defaults.dir/defaults.c.o.provides.build: tests/CMakeFiles/defaults.dir/defaults.c.o


# Object files for target defaults
defaults_OBJECTS = \
"CMakeFiles/defaults.dir/defaults.c.o"

# External object files for target defaults
defaults_EXTERNAL_OBJECTS =

tests/defaults: tests/CMakeFiles/defaults.dir/defaults.c.o
tests/defaults: tests/CMakeFiles/defaults.dir/build.make
tests/defaults: src/libglfw3.a
tests/defaults: tests/CMakeFiles/defaults.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable defaults"
	cd /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/defaults.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/defaults.dir/build: tests/defaults

.PHONY : tests/CMakeFiles/defaults.dir/build

tests/CMakeFiles/defaults.dir/requires: tests/CMakeFiles/defaults.dir/defaults.c.o.requires

.PHONY : tests/CMakeFiles/defaults.dir/requires

tests/CMakeFiles/defaults.dir/clean:
	cd /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/defaults.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/defaults.dir/clean

tests/CMakeFiles/defaults.dir/depend:
	cd /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1 /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/tests /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests /Users/qaovxtazypdl/cs488/cs488/shared/glfw-3.1.1/build/tests/CMakeFiles/defaults.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/defaults.dir/depend

