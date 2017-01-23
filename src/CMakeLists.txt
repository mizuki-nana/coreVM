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


# TODO: [COREVM-579] Re-enable IR schema compilation during builds
# add_custom_target(IRFormat WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMAND sh scripts/compile_ir_schema.sh)


set(sources
    ir/reader.cc
    ir/format_util.cc
    ir/ir_parser.tab.cc
    ir/lex.yy.cc
    ir/ir_parser_driver.cc
    ir/ir_module_index.cc
    ir/verifier.cc
    jit/pass.cc
    jit/analysis_pass.cc
    jit/analysis_result.cc
    jit/transform_pass.cc
    jit/verification_pass.cc
    jit/constant_folding_pass.cc
    jit/cse_pass.cc
    jit/pass_manager.cc
    jit/jit_compiler_backend.cc
    jit/jit_compiler_llvmmcjit_backend.cc
    jit/lowering_pass_target_llvm_ir.cc
    memory/allocation_scheme.cc
    memory/sequential_allocation_scheme.cc
    dyobj/dynamic_object_manager.cc
    dyobj/flags.cc
    dyobj/util.cc
    gc/garbage_collection_scheme.cc
    gc/refcount_gc_scheme.cc
    types/interfaces.cc
    types/native_array.cc
    types/native_map.cc
    types/native_string.cc
    runtime/closure.cc
    runtime/compartment.cc
    runtime/compartment_printer.cc
    runtime/dbgmem_printer.cc
    runtime/dbgvar_printer.cc
    runtime/frame.cc
    runtime/frame_cache.cc
    runtime/frame_printer.cc
    runtime/gc_rule.cc
    runtime/instr.cc
    runtime/instr_info.cc
    runtime/instr_printer.cc
    runtime/invocation_ctx.cc
    runtime/native_types_pool.cc
    runtime/process.cc
    runtime/process_runner.cc
    runtime/signal_handler.cc
    runtime/utils.cc
    frontend/binary_bytecode_loader.cc
    frontend/bytecode_loader.cc
    frontend/runner.cc
    corevm/errors.cc
    corevm/logging.cc
    api/core/configuration.cc
    api/core/entry.cc
    corevm/corevm_bytecode_schema.h
    )


# Library `libcorevm`.
add_library(libcorevm ${sources})


# add_dependencies(libcorevm IRFormat)


set_target_properties(libcorevm PROPERTIES COMPILE_FLAGS "-O3" OUTPUT_NAME "corevm")


add_custom_command(TARGET libcorevm POST_BUILD COMMAND ls -al $<TARGET_FILE:libcorevm>)


# Binary `coreVM`.
set(coreVM_sources
    corevm/program.cc
    corevm/main.cc
    )


add_executable(coreVM ${coreVM_sources})


set_target_properties(coreVM PROPERTIES COMPILE_FLAGS "-O3")


target_link_libraries(coreVM
    libcorevm
    ${DEPENDENCY_LINK_LIBRARIES}
    )


add_custom_command(TARGET coreVM POST_BUILD COMMAND ls -al $<TARGET_FILE:coreVM>)


### Debug version (pure).


# Library `libcorevm_dbg`.
if (NOT CMAKE_COMPILER_IS_GNUCXX)


add_library(libcorevm_dbg ${sources})


# add_dependencies(libcorevm_dbg IRFormat)


set_target_properties(libcorevm_dbg PROPERTIES COMPILE_FLAGS "-D__DEBUG__=1 -DINSTRUMENTAL_MEASUREMENT=1 -g -fno-inline -fno-omit-frame-pointer -fno-optimize-sibling-calls" OUTPUT_NAME "corevm_dbg")


add_custom_command(TARGET libcorevm_dbg POST_BUILD COMMAND ls -al $<TARGET_FILE:libcorevm_dbg>)


# Binary `coreVM_dbg`.
add_executable(coreVM_dbg ${coreVM_sources})


target_link_libraries(coreVM_dbg
    libcorevm_dbg
    ${DEPENDENCY_LINK_LIBRARIES}
    )


set_target_properties(coreVM_dbg PROPERTIES LINK_FLAGS "-rdynamic -fno-inline -fno-omit-frame-pointer -fno-optimize-sibling-calls")


add_custom_command(TARGET coreVM_dbg POST_BUILD COMMAND ls -al $<TARGET_FILE:coreVM_dbg>)


endif() # if (NOT CMAKE_COMPILER_IS_GNUCXX)


### Debug version with Address Sanitizer support.


# Library `libcorevm_dbg_asan`.
if (NOT CMAKE_COMPILER_IS_GNUCXX)


if(BUILD_SANITY_BIN)


add_library(libcorevm_dbg_asan ${sources})


# add_dependencies(libcorevm_dbg_asan IRFormat)


set_target_properties(libcorevm_dbg_asan PROPERTIES COMPILE_FLAGS "-D__DEBUG__=1 -DINSTRUMENTAL_MEASUREMENT=1 -g -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls" OUTPUT_NAME "corevm_dbg_asan")


add_custom_command(TARGET libcorevm_dbg_asan POST_BUILD COMMAND ls -al $<TARGET_FILE:libcorevm_dbg_asan>)


# Binary `coreVM_dbg_asan`.
add_executable(coreVM_dbg_asan ${coreVM_sources})


target_link_libraries(coreVM_dbg_asan
    libcorevm_dbg_asan
    ${DEPENDENCY_LINK_LIBRARIES}
    )


set_target_properties(coreVM_dbg_asan PROPERTIES LINK_FLAGS "-rdynamic -fno-inline -fno-omit-frame-pointer -fno-optimize-sibling-calls")


add_custom_command(TARGET coreVM_dbg_asan POST_BUILD COMMAND ls -al $<TARGET_FILE:coreVM_dbg_asan>)


endif() # if(BUILD_SANITY_BIN)


endif() # if (NOT CMAKE_COMPILER_IS_GNUCXX)
