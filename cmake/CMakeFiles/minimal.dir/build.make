# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /user/7/marestj/ProjetC/projet-c-fantastique

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /user/7/marestj/ProjetC/projet-c-fantastique/cmake

# Include any dependencies generated for this target.
include CMakeFiles/minimal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/minimal.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/minimal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/minimal.dir/flags.make

CMakeFiles/minimal.dir/tests/minimal.c.o: CMakeFiles/minimal.dir/flags.make
CMakeFiles/minimal.dir/tests/minimal.c.o: ../tests/minimal.c
CMakeFiles/minimal.dir/tests/minimal.c.o: CMakeFiles/minimal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/user/7/marestj/ProjetC/projet-c-fantastique/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/minimal.dir/tests/minimal.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/minimal.dir/tests/minimal.c.o -MF CMakeFiles/minimal.dir/tests/minimal.c.o.d -o CMakeFiles/minimal.dir/tests/minimal.c.o -c /user/7/marestj/ProjetC/projet-c-fantastique/tests/minimal.c

CMakeFiles/minimal.dir/tests/minimal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minimal.dir/tests/minimal.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /user/7/marestj/ProjetC/projet-c-fantastique/tests/minimal.c > CMakeFiles/minimal.dir/tests/minimal.c.i

CMakeFiles/minimal.dir/tests/minimal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minimal.dir/tests/minimal.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /user/7/marestj/ProjetC/projet-c-fantastique/tests/minimal.c -o CMakeFiles/minimal.dir/tests/minimal.c.s

# Object files for target minimal
minimal_OBJECTS = \
"CMakeFiles/minimal.dir/tests/minimal.c.o"

# External object files for target minimal
minimal_EXTERNAL_OBJECTS =

minimal: CMakeFiles/minimal.dir/tests/minimal.c.o
minimal: CMakeFiles/minimal.dir/build.make
minimal: CMakeFiles/minimal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/user/7/marestj/ProjetC/projet-c-fantastique/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable minimal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minimal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/minimal.dir/build: minimal
.PHONY : CMakeFiles/minimal.dir/build

CMakeFiles/minimal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/minimal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/minimal.dir/clean

CMakeFiles/minimal.dir/depend:
	cd /user/7/marestj/ProjetC/projet-c-fantastique/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /user/7/marestj/ProjetC/projet-c-fantastique /user/7/marestj/ProjetC/projet-c-fantastique /user/7/marestj/ProjetC/projet-c-fantastique/cmake /user/7/marestj/ProjetC/projet-c-fantastique/cmake /user/7/marestj/ProjetC/projet-c-fantastique/cmake/CMakeFiles/minimal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/minimal.dir/depend

