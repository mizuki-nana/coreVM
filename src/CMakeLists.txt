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


add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/src/corevm/corevm_bytecode_schema.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND sh scripts/compile_bytecode_schema.sh)


set(sources
    memory/allocation_scheme.cc
    memory/sequential_allocation_scheme.cc
    dyobj/dynamic_object_manager.cc
    dyobj/flags.cc
    dyobj/util.cc
    gc/garbage_collection_scheme.cc
    gc/mark_and_sweep_garbage_collection_scheme.cc
    gc/reference_count_garbage_collection_scheme.cc
    types/interfaces.cc
    types/native_array.cc
    types/native_map.cc
    types/native_string.cc
    runtime/closure.cc
    runtime/compartment.cc
    runtime/compartment_printer.cc
    runtime/dbgmem_printer.cc
    runtime/frame.cc
    runtime/frame_printer.cc
    runtime/gc_rule.cc
    runtime/instr.cc
    runtime/instr_info.cc
    runtime/instr_printer.cc
    runtime/invocation_ctx.cc
    runtime/native_types_pool.cc
    runtime/process.cc
    runtime/process_printer.cc
    runtime/process_runner.cc
    runtime/sighandler.cc
    runtime/sighandler_registrar.cc
    runtime/utils.cc
    frontend/binary_bytecode_loader.cc
    frontend/bytecode_loader.cc
    frontend/configuration.cc
    frontend/program.cc
    frontend/runner.cc
    frontend/signal_vector_loader.cc
    frontend/utils.cc
    corevm/errors.cc
    corevm/logging.cc
    corevm/corevm_bytecode_schema.h
    )


# Library `libcorevm`.
add_library(libcorevm ${sources})


set_target_properties(libcorevm PROPERTIES COMPILE_FLAGS "-O3" OUTPUT_NAME "corevm")


add_custom_command(TARGET libcorevm POST_BUILD COMMAND ls -al ./libcorevm.a)


# Binary `coreVM`.
add_executable(coreVM corevm/main.cc)


set_target_properties(coreVM PROPERTIES COMPILE_FLAGS "-O3")


target_link_libraries(coreVM
    libcorevm
    boost_system
    boost_regex
    boost_iostreams
    boost_program_options
    pthread
    sneaker
    avrocpp_s)


add_custom_command(TARGET coreVM POST_BUILD COMMAND ls -al ./coreVM)


### Debug version (pure).


# Library `libcorevm_dbg`.
add_library(libcorevm_dbg ${sources})


set_target_properties(libcorevm_dbg PROPERTIES COMPILE_FLAGS "-D__DEBUG__=1 -DINSTRUMENTAL_MEASUREMENT=1 -g -fno-inline -fno-omit-frame-pointer -fno-optimize-sibling-calls" OUTPUT_NAME "corevm_dbg")


add_custom_command(TARGET libcorevm_dbg POST_BUILD COMMAND ls -al ./libcorevm_dbg.a)


# Binary `coreVM_dbg`.
add_executable(coreVM_dbg corevm/main.cc)


target_link_libraries(coreVM_dbg
    libcorevm_dbg
    boost_system
    boost_regex
    boost_iostreams
    boost_timer
    boost_program_options
    pthread
    sneaker_dbg
    avrocpp_s
    -fno-inline
    -fno-omit-frame-pointer
    -fno-optimize-sibling-calls
    )


add_custom_command(TARGET coreVM_dbg POST_BUILD COMMAND ls -al ./coreVM_dbg)


### Debug version with Address Sanitizer support.


# Library `libcorevm_dbg_asan`.
if(BUILD_SANITY_BIN)


add_library(libcorevm_dbg_asan ${sources})


set_target_properties(libcorevm_dbg_asan PROPERTIES COMPILE_FLAGS "-D__DEBUG__=1 -DINSTRUMENTAL_MEASUREMENT=1 -g -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls" OUTPUT_NAME "corevm_dbg_asan")


add_custom_command(TARGET libcorevm_dbg_asan POST_BUILD COMMAND ls -al ./libcorevm_dbg_asan.a)


# Binary `coreVM_dbg_asan`.
add_executable(coreVM_dbg_asan corevm/main.cc)


target_link_libraries(coreVM_dbg_asan
    libcorevm_dbg_asan
    boost_system
    boost_regex
    boost_iostreams
    boost_timer
    boost_program_options
    pthread
    sneaker_dbg
    avrocpp_s
    -fsanitize=address
    -fno-omit-frame-pointer
    -fno-optimize-sibling-calls
    )


add_custom_command(TARGET coreVM_dbg_asan POST_BUILD COMMAND ls -al ./coreVM_dbg_asan)


endif() # if(BUILD_SANITY_BIN)
