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
CMAKE_SOURCE_DIR = /home/cdz/TinyWebServer5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cdz/TinyWebServer5/build

# Include any dependencies generated for this target.
include CMakeFiles/TinyWebServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TinyWebServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TinyWebServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TinyWebServer.dir/flags.make

CMakeFiles/TinyWebServer.dir/src/Config.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/Config.cpp.o: ../src/Config.cpp
CMakeFiles/TinyWebServer.dir/src/Config.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TinyWebServer.dir/src/Config.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/Config.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/Config.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/Config.cpp.o -c /home/cdz/TinyWebServer5/src/Config.cpp

CMakeFiles/TinyWebServer.dir/src/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/Config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/Config.cpp > CMakeFiles/TinyWebServer.dir/src/Config.cpp.i

CMakeFiles/TinyWebServer.dir/src/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/Config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/Config.cpp -o CMakeFiles/TinyWebServer.dir/src/Config.cpp.s

CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o: ../src/ConnPool.cpp
CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o -c /home/cdz/TinyWebServer5/src/ConnPool.cpp

CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/ConnPool.cpp > CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.i

CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/ConnPool.cpp -o CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.s

CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o: ../src/HttpConn.cpp
CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o -c /home/cdz/TinyWebServer5/src/HttpConn.cpp

CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/HttpConn.cpp > CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.i

CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/HttpConn.cpp -o CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.s

CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o: ../src/Locker.cpp
CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o -c /home/cdz/TinyWebServer5/src/Locker.cpp

CMakeFiles/TinyWebServer.dir/src/Locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/Locker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/Locker.cpp > CMakeFiles/TinyWebServer.dir/src/Locker.cpp.i

CMakeFiles/TinyWebServer.dir/src/Locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/Locker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/Locker.cpp -o CMakeFiles/TinyWebServer.dir/src/Locker.cpp.s

CMakeFiles/TinyWebServer.dir/src/Log.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/Log.cpp.o: ../src/Log.cpp
CMakeFiles/TinyWebServer.dir/src/Log.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TinyWebServer.dir/src/Log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/Log.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/Log.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/Log.cpp.o -c /home/cdz/TinyWebServer5/src/Log.cpp

CMakeFiles/TinyWebServer.dir/src/Log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/Log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/Log.cpp > CMakeFiles/TinyWebServer.dir/src/Log.cpp.i

CMakeFiles/TinyWebServer.dir/src/Log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/Log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/Log.cpp -o CMakeFiles/TinyWebServer.dir/src/Log.cpp.s

CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o: ../src/MysqlConn.cpp
CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o -c /home/cdz/TinyWebServer5/src/MysqlConn.cpp

CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/MysqlConn.cpp > CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.i

CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/MysqlConn.cpp -o CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.s

CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o: ../src/Timer.cpp
CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o -c /home/cdz/TinyWebServer5/src/Timer.cpp

CMakeFiles/TinyWebServer.dir/src/Timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/Timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/Timer.cpp > CMakeFiles/TinyWebServer.dir/src/Timer.cpp.i

CMakeFiles/TinyWebServer.dir/src/Timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/Timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/Timer.cpp -o CMakeFiles/TinyWebServer.dir/src/Timer.cpp.s

CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o: ../src/WebServer.cpp
CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o -c /home/cdz/TinyWebServer5/src/WebServer.cpp

CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/WebServer.cpp > CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.i

CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/WebServer.cpp -o CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.s

CMakeFiles/TinyWebServer.dir/src/main.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/TinyWebServer.dir/src/main.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TinyWebServer.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/src/main.cpp.o -MF CMakeFiles/TinyWebServer.dir/src/main.cpp.o.d -o CMakeFiles/TinyWebServer.dir/src/main.cpp.o -c /home/cdz/TinyWebServer5/src/main.cpp

CMakeFiles/TinyWebServer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cdz/TinyWebServer5/src/main.cpp > CMakeFiles/TinyWebServer.dir/src/main.cpp.i

CMakeFiles/TinyWebServer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cdz/TinyWebServer5/src/main.cpp -o CMakeFiles/TinyWebServer.dir/src/main.cpp.s

# Object files for target TinyWebServer
TinyWebServer_OBJECTS = \
"CMakeFiles/TinyWebServer.dir/src/Config.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/Log.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o" \
"CMakeFiles/TinyWebServer.dir/src/main.cpp.o"

# External object files for target TinyWebServer
TinyWebServer_EXTERNAL_OBJECTS =

../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/Config.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/ConnPool.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/HttpConn.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/Locker.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/Log.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/MysqlConn.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/Timer.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/WebServer.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/src/main.cpp.o
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/build.make
../bin/TinyWebServer: CMakeFiles/TinyWebServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cdz/TinyWebServer5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable ../bin/TinyWebServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TinyWebServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TinyWebServer.dir/build: ../bin/TinyWebServer
.PHONY : CMakeFiles/TinyWebServer.dir/build

CMakeFiles/TinyWebServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TinyWebServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TinyWebServer.dir/clean

CMakeFiles/TinyWebServer.dir/depend:
	cd /home/cdz/TinyWebServer5/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cdz/TinyWebServer5 /home/cdz/TinyWebServer5 /home/cdz/TinyWebServer5/build /home/cdz/TinyWebServer5/build /home/cdz/TinyWebServer5/build/CMakeFiles/TinyWebServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TinyWebServer.dir/depend

