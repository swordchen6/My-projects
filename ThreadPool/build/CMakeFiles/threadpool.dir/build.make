# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cdz/Desktop/ThreadPool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cdz/Desktop/ThreadPool/build

# Include any dependencies generated for this target.
include CMakeFiles/threadpool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/threadpool.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/threadpool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/threadpool.dir/flags.make

CMakeFiles/threadpool.dir/src/ThreadPool.c.o: CMakeFiles/threadpool.dir/flags.make
CMakeFiles/threadpool.dir/src/ThreadPool.c.o: ../src/ThreadPool.c
CMakeFiles/threadpool.dir/src/ThreadPool.c.o: CMakeFiles/threadpool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/Desktop/ThreadPool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/threadpool.dir/src/ThreadPool.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/threadpool.dir/src/ThreadPool.c.o -MF CMakeFiles/threadpool.dir/src/ThreadPool.c.o.d -o CMakeFiles/threadpool.dir/src/ThreadPool.c.o -c /home/cdz/Desktop/ThreadPool/src/ThreadPool.c

CMakeFiles/threadpool.dir/src/ThreadPool.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threadpool.dir/src/ThreadPool.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cdz/Desktop/ThreadPool/src/ThreadPool.c > CMakeFiles/threadpool.dir/src/ThreadPool.c.i

CMakeFiles/threadpool.dir/src/ThreadPool.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threadpool.dir/src/ThreadPool.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cdz/Desktop/ThreadPool/src/ThreadPool.c -o CMakeFiles/threadpool.dir/src/ThreadPool.c.s

CMakeFiles/threadpool.dir/src/main.c.o: CMakeFiles/threadpool.dir/flags.make
CMakeFiles/threadpool.dir/src/main.c.o: ../src/main.c
CMakeFiles/threadpool.dir/src/main.c.o: CMakeFiles/threadpool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/Desktop/ThreadPool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/threadpool.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/threadpool.dir/src/main.c.o -MF CMakeFiles/threadpool.dir/src/main.c.o.d -o CMakeFiles/threadpool.dir/src/main.c.o -c /home/cdz/Desktop/ThreadPool/src/main.c

CMakeFiles/threadpool.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threadpool.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cdz/Desktop/ThreadPool/src/main.c > CMakeFiles/threadpool.dir/src/main.c.i

CMakeFiles/threadpool.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threadpool.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cdz/Desktop/ThreadPool/src/main.c -o CMakeFiles/threadpool.dir/src/main.c.s

# Object files for target threadpool
threadpool_OBJECTS = \
"CMakeFiles/threadpool.dir/src/ThreadPool.c.o" \
"CMakeFiles/threadpool.dir/src/main.c.o"

# External object files for target threadpool
threadpool_EXTERNAL_OBJECTS =

../bin/threadpool: CMakeFiles/threadpool.dir/src/ThreadPool.c.o
../bin/threadpool: CMakeFiles/threadpool.dir/src/main.c.o
../bin/threadpool: CMakeFiles/threadpool.dir/build.make
../bin/threadpool: CMakeFiles/threadpool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cdz/Desktop/ThreadPool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../bin/threadpool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/threadpool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/threadpool.dir/build: ../bin/threadpool
.PHONY : CMakeFiles/threadpool.dir/build

CMakeFiles/threadpool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/threadpool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/threadpool.dir/clean

CMakeFiles/threadpool.dir/depend:
	cd /home/cdz/Desktop/ThreadPool/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cdz/Desktop/ThreadPool /home/cdz/Desktop/ThreadPool /home/cdz/Desktop/ThreadPool/build /home/cdz/Desktop/ThreadPool/build /home/cdz/Desktop/ThreadPool/build/CMakeFiles/threadpool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/threadpool.dir/depend

