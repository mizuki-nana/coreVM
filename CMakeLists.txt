#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2016 Yanzheng Li
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 2.6)


# Project.
project(coreVM CXX)


# Option.
option(BUILD_FULL "Build project in full" OFF)


# Release mode.
set(CMAKE_BUILD_TYPE Release ... FORCE)


# Compiler.
set(CMAKE_CXX_COMPILER "clang++")


# General compiler flags.
set(CMAKE_CXX_FLAGS "-std=c++11 -Weverything -Wno-deprecated -Wno-padded -Wno-float-equal -Wno-global-constructors -Wno-exit-time-destructors -Wno-c++98-compat -Wno-documentation -Wno-missing-prototypes -Wno-format-nonliteral")


# Linker options.
# Suppressing passing -rdynamic.
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")


# Reset per-target ar/ranlib flags.
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rvs -o <TARGET> <LINK_FLAGS> <OBJECTS>")


# Packages.
find_package(Boost 1.55 REQUIRED COMPONENTS
    system
    regex
    iostreams
    timer
    program_options
    )


# Paths.
include_directories("${CMAKE_SOURCE_DIR}/src/")
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)
set(TOOLS_DIR ${PROJECT_SOURCE_DIR}/tools)
set(BENCHMARKS_DIR ${PROJECT_SOURCE_DIR}/benchmarks)


# Sub-directories.
ADD_SUBDIRECTORY(${SRC_DIR})
ADD_SUBDIRECTORY(${TESTS_DIR})
ADD_SUBDIRECTORY(${TOOLS_DIR})

if(BUILD_FULL)
  ADD_SUBDIRECTORY(${BENCHMARKS_DIR})
endif()
