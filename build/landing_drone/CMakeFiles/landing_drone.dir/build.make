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
CMAKE_SOURCE_DIR = /home/victor/catkin_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/catkin_ws/build

# Include any dependencies generated for this target.
include landing_drone/CMakeFiles/landing_drone.dir/depend.make

# Include the progress variables for this target.
include landing_drone/CMakeFiles/landing_drone.dir/progress.make

# Include the compile flags for this target's objects.
include landing_drone/CMakeFiles/landing_drone.dir/flags.make

landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o: landing_drone/CMakeFiles/landing_drone.dir/flags.make
landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o: /home/victor/catkin_ws/src/landing_drone/src/KalmanFilter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victor/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o"
	cd /home/victor/catkin_ws/build/landing_drone && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o -c /home/victor/catkin_ws/src/landing_drone/src/KalmanFilter.cpp

landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.i"
	cd /home/victor/catkin_ws/build/landing_drone && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victor/catkin_ws/src/landing_drone/src/KalmanFilter.cpp > CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.i

landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.s"
	cd /home/victor/catkin_ws/build/landing_drone && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victor/catkin_ws/src/landing_drone/src/KalmanFilter.cpp -o CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.s

landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.requires:

.PHONY : landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.requires

landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.provides: landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.requires
	$(MAKE) -f landing_drone/CMakeFiles/landing_drone.dir/build.make landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.provides.build
.PHONY : landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.provides

landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.provides.build: landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o


# Object files for target landing_drone
landing_drone_OBJECTS = \
"CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o"

# External object files for target landing_drone
landing_drone_EXTERNAL_OBJECTS =

/home/victor/catkin_ws/devel/lib/liblanding_drone.so: landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o
/home/victor/catkin_ws/devel/lib/liblanding_drone.so: landing_drone/CMakeFiles/landing_drone.dir/build.make
/home/victor/catkin_ws/devel/lib/liblanding_drone.so: landing_drone/CMakeFiles/landing_drone.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/victor/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library /home/victor/catkin_ws/devel/lib/liblanding_drone.so"
	cd /home/victor/catkin_ws/build/landing_drone && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/landing_drone.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
landing_drone/CMakeFiles/landing_drone.dir/build: /home/victor/catkin_ws/devel/lib/liblanding_drone.so

.PHONY : landing_drone/CMakeFiles/landing_drone.dir/build

landing_drone/CMakeFiles/landing_drone.dir/requires: landing_drone/CMakeFiles/landing_drone.dir/src/KalmanFilter.cpp.o.requires

.PHONY : landing_drone/CMakeFiles/landing_drone.dir/requires

landing_drone/CMakeFiles/landing_drone.dir/clean:
	cd /home/victor/catkin_ws/build/landing_drone && $(CMAKE_COMMAND) -P CMakeFiles/landing_drone.dir/cmake_clean.cmake
.PHONY : landing_drone/CMakeFiles/landing_drone.dir/clean

landing_drone/CMakeFiles/landing_drone.dir/depend:
	cd /home/victor/catkin_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/catkin_ws/src /home/victor/catkin_ws/src/landing_drone /home/victor/catkin_ws/build /home/victor/catkin_ws/build/landing_drone /home/victor/catkin_ws/build/landing_drone/CMakeFiles/landing_drone.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : landing_drone/CMakeFiles/landing_drone.dir/depend

