# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ahmed/ardrone_simulator/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahmed/ardrone_simulator/build

# Include any dependencies generated for this target.
include cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/depend.make

# Include the progress variables for this target.
include cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/progress.make

# Include the compile flags for this target's objects.
include cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/flags.make

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/flags.make
cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o: /home/ahmed/ardrone_simulator/src/cvg_sim_gazebo_plugins/src/reset_plugin.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed/ardrone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o"
	cd /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o -c /home/ahmed/ardrone_simulator/src/cvg_sim_gazebo_plugins/src/reset_plugin.cpp

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.i"
	cd /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed/ardrone_simulator/src/cvg_sim_gazebo_plugins/src/reset_plugin.cpp > CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.i

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.s"
	cd /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed/ardrone_simulator/src/cvg_sim_gazebo_plugins/src/reset_plugin.cpp -o CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.s

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.requires:

.PHONY : cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.requires

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.provides: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.requires
	$(MAKE) -f cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/build.make cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.provides.build
.PHONY : cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.provides

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.provides.build: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o


# Object files for target reset_plugin
reset_plugin_OBJECTS = \
"CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o"

# External object files for target reset_plugin
reset_plugin_EXTERNAL_OBJECTS =

/home/ahmed/ardrone_simulator/devel/lib/libreset_plugin.so: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o
/home/ahmed/ardrone_simulator/devel/lib/libreset_plugin.so: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/build.make
/home/ahmed/ardrone_simulator/devel/lib/libreset_plugin.so: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ahmed/ardrone_simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library /home/ahmed/ardrone_simulator/devel/lib/libreset_plugin.so"
	cd /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reset_plugin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/build: /home/ahmed/ardrone_simulator/devel/lib/libreset_plugin.so

.PHONY : cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/build

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/requires: cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/src/reset_plugin.cpp.o.requires

.PHONY : cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/requires

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/clean:
	cd /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins && $(CMAKE_COMMAND) -P CMakeFiles/reset_plugin.dir/cmake_clean.cmake
.PHONY : cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/clean

cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/depend:
	cd /home/ahmed/ardrone_simulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmed/ardrone_simulator/src /home/ahmed/ardrone_simulator/src/cvg_sim_gazebo_plugins /home/ahmed/ardrone_simulator/build /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins /home/ahmed/ardrone_simulator/build/cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cvg_sim_gazebo_plugins/CMakeFiles/reset_plugin.dir/depend

