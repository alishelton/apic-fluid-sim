# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alishelton/Desktop/184/Projects/assignment3starter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug

# Include any dependencies generated for this target.
include glfw-3.3/tests/CMakeFiles/vulkan.dir/depend.make

# Include the progress variables for this target.
include glfw-3.3/tests/CMakeFiles/vulkan.dir/progress.make

# Include the compile flags for this target's objects.
include glfw-3.3/tests/CMakeFiles/vulkan.dir/flags.make

glfw-3.3/tests/CMakeFiles/vulkan.dir/vulkan.c.o: glfw-3.3/tests/CMakeFiles/vulkan.dir/flags.make
glfw-3.3/tests/CMakeFiles/vulkan.dir/vulkan.c.o: ../glfw-3.3/tests/vulkan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw-3.3/tests/CMakeFiles/vulkan.dir/vulkan.c.o"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vulkan.dir/vulkan.c.o   -c /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/tests/vulkan.c

glfw-3.3/tests/CMakeFiles/vulkan.dir/vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vulkan.dir/vulkan.c.i"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/tests/vulkan.c > CMakeFiles/vulkan.dir/vulkan.c.i

glfw-3.3/tests/CMakeFiles/vulkan.dir/vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vulkan.dir/vulkan.c.s"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/tests/vulkan.c -o CMakeFiles/vulkan.dir/vulkan.c.s

glfw-3.3/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o: glfw-3.3/tests/CMakeFiles/vulkan.dir/flags.make
glfw-3.3/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o: ../glfw-3.3/deps/glad_vulkan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw-3.3/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o   -c /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/deps/glad_vulkan.c

glfw-3.3/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/deps/glad_vulkan.c > CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i

glfw-3.3/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/deps/glad_vulkan.c -o CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s

# Object files for target vulkan
vulkan_OBJECTS = \
"CMakeFiles/vulkan.dir/vulkan.c.o" \
"CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o"

# External object files for target vulkan
vulkan_EXTERNAL_OBJECTS =

glfw-3.3/tests/vulkan: glfw-3.3/tests/CMakeFiles/vulkan.dir/vulkan.c.o
glfw-3.3/tests/vulkan: glfw-3.3/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o
glfw-3.3/tests/vulkan: glfw-3.3/tests/CMakeFiles/vulkan.dir/build.make
glfw-3.3/tests/vulkan: glfw-3.3/src/libglfw3.a
glfw-3.3/tests/vulkan: glfw-3.3/tests/CMakeFiles/vulkan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable vulkan"
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vulkan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw-3.3/tests/CMakeFiles/vulkan.dir/build: glfw-3.3/tests/vulkan

.PHONY : glfw-3.3/tests/CMakeFiles/vulkan.dir/build

glfw-3.3/tests/CMakeFiles/vulkan.dir/clean:
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests && $(CMAKE_COMMAND) -P CMakeFiles/vulkan.dir/cmake_clean.cmake
.PHONY : glfw-3.3/tests/CMakeFiles/vulkan.dir/clean

glfw-3.3/tests/CMakeFiles/vulkan.dir/depend:
	cd /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alishelton/Desktop/184/Projects/assignment3starter /Users/alishelton/Desktop/184/Projects/assignment3starter/glfw-3.3/tests /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests /Users/alishelton/Desktop/184/Projects/assignment3starter/cmake-build-debug/glfw-3.3/tests/CMakeFiles/vulkan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw-3.3/tests/CMakeFiles/vulkan.dir/depend

