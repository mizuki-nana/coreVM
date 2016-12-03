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
option(BUILD_BENCHMARKS "Build micro benchmarks" OFF)
option(BUILD_SANITY_BIN "Build sanity-check binaries" OFF)
option(BUILD_BENCHMARKS_STRICT "Build all benchmarks" ON)


# Release mode.
set(CMAKE_BUILD_TYPE Release ... FORCE)


# Compiler.
set(CMAKE_CXX_COMPILER "clang++" CACHE string "")


set(CMAKE_MACOSX_RPATH 1)


# General compiler flags.
if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_CXX_FLAGS "-std=c++11 -w")
else ()
    set(CMAKE_CXX_FLAGS "-std=c++11 -Weverything -Wno-old-style-cast -Wno-sign-conversion -Wno-deprecated -Wno-padded -Wno-float-equal -Wno-global-constructors -Wno-exit-time-destructors -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -Wno-missing-prototypes -Wno-format-nonliteral")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} MATCHES "7.3.0")
        # Settings for local macOS dev machine.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro -Wno-shorten-64-to-32 -Wno-double-promotion")
    endif()
endif (CMAKE_COMPILER_IS_GNUCXX)


# Linker options.
# Suppressing passing -rdynamic.
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")


# Reset per-target ar/ranlib flags.
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rvs -o <TARGET> <LINK_FLAGS> <OBJECTS>")
endif ()


# Find Boost.
find_package(Boost 1.55 REQUIRED COMPONENTS
    system
    regex
    iostreams
    timer
    program_options
    )


message("Boost libraries found: ${Boost_LIBRARIES}")


# Find LLVM.
find_library(LIBLLVM_LIBRARY NAMES "LLVMSupport"
    PATH_SUFFIXES "lib" "lib32" "lib64")


message("LLVMSupport library path found: " ${LIBLLVM_LIBRARY})


# Find Sneaker.
find_path(LIBSNEAKER_INCLUDE_DIR NAMES "sneaker"
    PATHS
    "/opt/local/include"
    "/usr/local/include"
    "/usr/include"
    )


find_library(LIBSNEAKER_LIBRARY NAMES "sneaker"
    PATH_SUFFIXES "lib" "lib32" "lib64")


message("Sneaker include path found: " ${LIBSNEAKER_INCLUDE_DIR})
message("Sneaker library path found: " ${LIBSNEAKER_LIBRARY})


# Find avrocpp.
find_library(LIBAVROCPP_LIBRARY NAMES "avrocpp_s"
    PATH_SUFFIXES "lib" "lib32" "lib64")


message("avrocpp library path found: " ${LIBAVROCPP_LIBRARY})


# Find googletest.
find_library(LIBGTEST_LIBRARY NAMES "gtest"
    PATH_SUFFIXES "lib" "lib32" "lib64")

message("gtest library path found: " ${LIBGTEST_LIBRARY})


# Find benchmark.
find_library(LIBBENCHMARK_LIBRARY NAMES "benchmark"
    PATH_SUFFIXES "lib" "lib32" "lib64")


message("benchmark library path found: " ${LIBBENCHMARK_LIBRARY})


# Paths.
include_directories(SYSTEM ${LIBSNEAKER_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/src/")
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)
set(TOOLS_DIR ${PROJECT_SOURCE_DIR}/tools)
set(PYTHON_DIR ${PROJECT_SOURCE_DIR}/python)
set(BENCHMARKS_DIR ${PROJECT_SOURCE_DIR}/benchmarks)


# Dependency libraries.
set(DEPENDENCY_LINK_LIBRARIES
    pthread
    ${Boost_LIBRARIES}
    ${LIBAVROCPP_LIBRARY}
    ${LIBSNEAKER_LIBRARY}
    ${LIBLLVM_LIBRARY}
    )


# Sub-directories.
ADD_SUBDIRECTORY(${SRC_DIR})
if (NOT CMAKE_COMPILER_IS_GNUCXX)
    ADD_SUBDIRECTORY(${TOOLS_DIR})
    ADD_SUBDIRECTORY(${TESTS_DIR})
    ADD_SUBDIRECTORY(${PYTHON_DIR})

    if(BUILD_BENCHMARKS)
        ADD_SUBDIRECTORY(${BENCHMARKS_DIR})
    endif()
endif()
