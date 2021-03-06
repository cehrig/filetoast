# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/cehrig/dev/filetoast

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cehrig/dev/filetoast

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/cehrig/dev/filetoast/CMakeFiles /home/cehrig/dev/filetoast/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/cehrig/dev/filetoast/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named filetoast

# Build rule for target.
filetoast: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 filetoast
.PHONY : filetoast

# fast build rule for target.
filetoast/fast:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/build
.PHONY : filetoast/fast

src/job.o: src/job.c.o

.PHONY : src/job.o

# target to build an object file
src/job.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/job.c.o
.PHONY : src/job.c.o

src/job.i: src/job.c.i

.PHONY : src/job.i

# target to preprocess a source file
src/job.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/job.c.i
.PHONY : src/job.c.i

src/job.s: src/job.c.s

.PHONY : src/job.s

# target to generate assembly for a file
src/job.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/job.c.s
.PHONY : src/job.c.s

src/log.o: src/log.c.o

.PHONY : src/log.o

# target to build an object file
src/log.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/log.c.o
.PHONY : src/log.c.o

src/log.i: src/log.c.i

.PHONY : src/log.i

# target to preprocess a source file
src/log.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/log.c.i
.PHONY : src/log.c.i

src/log.s: src/log.c.s

.PHONY : src/log.s

# target to generate assembly for a file
src/log.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/log.c.s
.PHONY : src/log.c.s

src/main.o: src/main.c.o

.PHONY : src/main.o

# target to build an object file
src/main.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/main.c.o
.PHONY : src/main.c.o

src/main.i: src/main.c.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/main.c.s
.PHONY : src/main.c.s

src/reverse.o: src/reverse.c.o

.PHONY : src/reverse.o

# target to build an object file
src/reverse.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/reverse.c.o
.PHONY : src/reverse.c.o

src/reverse.i: src/reverse.c.i

.PHONY : src/reverse.i

# target to preprocess a source file
src/reverse.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/reverse.c.i
.PHONY : src/reverse.c.i

src/reverse.s: src/reverse.c.s

.PHONY : src/reverse.s

# target to generate assembly for a file
src/reverse.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/reverse.c.s
.PHONY : src/reverse.c.s

src/ssl.o: src/ssl.c.o

.PHONY : src/ssl.o

# target to build an object file
src/ssl.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/ssl.c.o
.PHONY : src/ssl.c.o

src/ssl.i: src/ssl.c.i

.PHONY : src/ssl.i

# target to preprocess a source file
src/ssl.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/ssl.c.i
.PHONY : src/ssl.c.i

src/ssl.s: src/ssl.c.s

.PHONY : src/ssl.s

# target to generate assembly for a file
src/ssl.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/ssl.c.s
.PHONY : src/ssl.c.s

src/supervisor.o: src/supervisor.c.o

.PHONY : src/supervisor.o

# target to build an object file
src/supervisor.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/supervisor.c.o
.PHONY : src/supervisor.c.o

src/supervisor.i: src/supervisor.c.i

.PHONY : src/supervisor.i

# target to preprocess a source file
src/supervisor.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/supervisor.c.i
.PHONY : src/supervisor.c.i

src/supervisor.s: src/supervisor.c.s

.PHONY : src/supervisor.s

# target to generate assembly for a file
src/supervisor.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/supervisor.c.s
.PHONY : src/supervisor.c.s

src/worker.o: src/worker.c.o

.PHONY : src/worker.o

# target to build an object file
src/worker.c.o:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/worker.c.o
.PHONY : src/worker.c.o

src/worker.i: src/worker.c.i

.PHONY : src/worker.i

# target to preprocess a source file
src/worker.c.i:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/worker.c.i
.PHONY : src/worker.c.i

src/worker.s: src/worker.c.s

.PHONY : src/worker.s

# target to generate assembly for a file
src/worker.c.s:
	$(MAKE) -f CMakeFiles/filetoast.dir/build.make CMakeFiles/filetoast.dir/src/worker.c.s
.PHONY : src/worker.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... filetoast"
	@echo "... src/job.o"
	@echo "... src/job.i"
	@echo "... src/job.s"
	@echo "... src/log.o"
	@echo "... src/log.i"
	@echo "... src/log.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/reverse.o"
	@echo "... src/reverse.i"
	@echo "... src/reverse.s"
	@echo "... src/ssl.o"
	@echo "... src/ssl.i"
	@echo "... src/ssl.s"
	@echo "... src/supervisor.o"
	@echo "... src/supervisor.i"
	@echo "... src/supervisor.s"
	@echo "... src/worker.o"
	@echo "... src/worker.i"
	@echo "... src/worker.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

