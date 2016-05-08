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


# Build executable `run_benchmarks`.
add_executable(run_benchmarks
    micro/instr_benchmarks_fixture.cc
    micro/allocators_benchmark.cc
    micro/variant_benchmark.cc
    micro/complex_native_type_benchmark.cc
    micro/object_container_benchmark.cc
    micro/ntvhndl_benchmark.cc
    micro/process_benchmark.cc
    micro/frame_benchmark.cc
    micro/object_instrs_benchmark.cc
    micro/control_instrs_benchmark.cc
    micro/function_instrs_benchmark.cc
    micro/ntvhndl_arithmetic_instrs_benchmark.cc
    micro/ntvhndl_creation_instrs_benchmark.cc
    micro/ntvhndl_conversion_instrs_benchmark.cc
    micro/ntvhndl_manipulation_instrs_benchmark.cc
    micro/native_string_instrs_benchmark.cc
    micro/native_array_instrs_benchmark.cc
    micro/native_map_instrs_benchmark.cc
    micro/native_string_interfaces_benchmark.cc
    micro/native_array_interfaces_benchmark.cc
    micro/native_map_interfaces_benchmark.cc
    micro/runtime_utils_benchmark.cc
    main.cc
    )


if(BUILD_BENCHMARKS_STRICT)
    set_target_properties(run_benchmarks PROPERTIES COMPILE_FLAGS "-O3 -DBUILD_BENCHMARKS_STRICT")
else()
    set_target_properties(run_benchmarks PROPERTIES COMPILE_FLAGS "-O3")
endif()


target_link_libraries(run_benchmarks
    libcorevm
    boost_system
    boost_regex
    boost_iostreams
    boost_program_options
    pthread
    sneaker
    avrocpp_s
    LLVMSupport
    benchmark
    )
