# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /storage4/lxj/Cplus/MudCash

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage4/lxj/Cplus/MudCash/build

# Include any dependencies generated for this target.
include src/slaveserver/CMakeFiles/slaveserver2.dir/depend.make

# Include the progress variables for this target.
include src/slaveserver/CMakeFiles/slaveserver2.dir/progress.make

# Include the compile flags for this target's objects.
include src/slaveserver/CMakeFiles/slaveserver2.dir/flags.make

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o: src/slaveserver/CMakeFiles/slaveserver2.dir/flags.make
src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o: ../src/slaveserver/slaveServer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage4/lxj/Cplus/MudCash/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/slaveserver2.dir/slaveServer.cpp.o -c /storage4/lxj/Cplus/MudCash/src/slaveserver/slaveServer.cpp

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/slaveserver2.dir/slaveServer.cpp.i"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage4/lxj/Cplus/MudCash/src/slaveserver/slaveServer.cpp > CMakeFiles/slaveserver2.dir/slaveServer.cpp.i

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/slaveserver2.dir/slaveServer.cpp.s"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage4/lxj/Cplus/MudCash/src/slaveserver/slaveServer.cpp -o CMakeFiles/slaveserver2.dir/slaveServer.cpp.s

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.requires:

.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.requires

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.provides: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.requires
	$(MAKE) -f src/slaveserver/CMakeFiles/slaveserver2.dir/build.make src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.provides.build
.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.provides

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.provides.build: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o


src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o: src/slaveserver/CMakeFiles/slaveserver2.dir/flags.make
src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o: ../src/slaveserver/slaveService.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage4/lxj/Cplus/MudCash/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/slaveserver2.dir/slaveService.cpp.o -c /storage4/lxj/Cplus/MudCash/src/slaveserver/slaveService.cpp

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/slaveserver2.dir/slaveService.cpp.i"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage4/lxj/Cplus/MudCash/src/slaveserver/slaveService.cpp > CMakeFiles/slaveserver2.dir/slaveService.cpp.i

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/slaveserver2.dir/slaveService.cpp.s"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage4/lxj/Cplus/MudCash/src/slaveserver/slaveService.cpp -o CMakeFiles/slaveserver2.dir/slaveService.cpp.s

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.requires:

.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.requires

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.provides: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.requires
	$(MAKE) -f src/slaveserver/CMakeFiles/slaveserver2.dir/build.make src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.provides.build
.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.provides

src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.provides.build: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o


src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o: src/slaveserver/CMakeFiles/slaveserver2.dir/flags.make
src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o: ../src/client/caclient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage4/lxj/Cplus/MudCash/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o -c /storage4/lxj/Cplus/MudCash/src/client/caclient.cpp

src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.i"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage4/lxj/Cplus/MudCash/src/client/caclient.cpp > CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.i

src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.s"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage4/lxj/Cplus/MudCash/src/client/caclient.cpp -o CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.s

src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.requires:

.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.requires

src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.provides: src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.requires
	$(MAKE) -f src/slaveserver/CMakeFiles/slaveserver2.dir/build.make src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.provides.build
.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.provides

src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.provides.build: src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o


src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o: src/slaveserver/CMakeFiles/slaveserver2.dir/flags.make
src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o: ../src/slaveserver/test_slave_server2.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage4/lxj/Cplus/MudCash/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o -c /storage4/lxj/Cplus/MudCash/src/slaveserver/test_slave_server2.cc

src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/slaveserver2.dir/test_slave_server2.cc.i"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage4/lxj/Cplus/MudCash/src/slaveserver/test_slave_server2.cc > CMakeFiles/slaveserver2.dir/test_slave_server2.cc.i

src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/slaveserver2.dir/test_slave_server2.cc.s"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage4/lxj/Cplus/MudCash/src/slaveserver/test_slave_server2.cc -o CMakeFiles/slaveserver2.dir/test_slave_server2.cc.s

src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.requires:

.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.requires

src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.provides: src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.requires
	$(MAKE) -f src/slaveserver/CMakeFiles/slaveserver2.dir/build.make src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.provides.build
.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.provides

src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.provides.build: src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o


# Object files for target slaveserver2
slaveserver2_OBJECTS = \
"CMakeFiles/slaveserver2.dir/slaveServer.cpp.o" \
"CMakeFiles/slaveserver2.dir/slaveService.cpp.o" \
"CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o" \
"CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o"

# External object files for target slaveserver2
slaveserver2_EXTERNAL_OBJECTS =

../bin/slaveserver2: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o
../bin/slaveserver2: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o
../bin/slaveserver2: src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o
../bin/slaveserver2: src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o
../bin/slaveserver2: src/slaveserver/CMakeFiles/slaveserver2.dir/build.make
../bin/slaveserver2: src/slaveserver/CMakeFiles/slaveserver2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage4/lxj/Cplus/MudCash/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ../../../bin/slaveserver2"
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/slaveserver2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/slaveserver/CMakeFiles/slaveserver2.dir/build: ../bin/slaveserver2

.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/build

src/slaveserver/CMakeFiles/slaveserver2.dir/requires: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveServer.cpp.o.requires
src/slaveserver/CMakeFiles/slaveserver2.dir/requires: src/slaveserver/CMakeFiles/slaveserver2.dir/slaveService.cpp.o.requires
src/slaveserver/CMakeFiles/slaveserver2.dir/requires: src/slaveserver/CMakeFiles/slaveserver2.dir/__/client/caclient.cpp.o.requires
src/slaveserver/CMakeFiles/slaveserver2.dir/requires: src/slaveserver/CMakeFiles/slaveserver2.dir/test_slave_server2.cc.o.requires

.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/requires

src/slaveserver/CMakeFiles/slaveserver2.dir/clean:
	cd /storage4/lxj/Cplus/MudCash/build/src/slaveserver && $(CMAKE_COMMAND) -P CMakeFiles/slaveserver2.dir/cmake_clean.cmake
.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/clean

src/slaveserver/CMakeFiles/slaveserver2.dir/depend:
	cd /storage4/lxj/Cplus/MudCash/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage4/lxj/Cplus/MudCash /storage4/lxj/Cplus/MudCash/src/slaveserver /storage4/lxj/Cplus/MudCash/build /storage4/lxj/Cplus/MudCash/build/src/slaveserver /storage4/lxj/Cplus/MudCash/build/src/slaveserver/CMakeFiles/slaveserver2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/slaveserver/CMakeFiles/slaveserver2.dir/depend

