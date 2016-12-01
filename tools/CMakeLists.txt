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


# Build executable `extract_metadata`.
add_executable(extract_metadata
    extract_metadata.cc
    )


set_target_properties(extract_metadata PROPERTIES COMPILE_FLAGS "-O3")


target_link_libraries(extract_metadata
    libcorevm
    ${DEPENDENCY_LINK_LIBRARIES}
    )


# Build executable `ir_stats`.
add_executable(ir_stats
    ir_stats.cc
    )


set_target_properties(ir_stats PROPERTIES COMPILE_FLAGS "-O3")


target_link_libraries(ir_stats
    libcorevm
    ${DEPENDENCY_LINK_LIBRARIES}
    )


# Build executable `ir_dis`.
add_executable(ir_dis
    ir_dis.cc
    )


set_target_properties(ir_dis PROPERTIES COMPILE_FLAGS "-O3")


target_link_libraries(ir_dis
    libcorevm
    ${DEPENDENCY_LINK_LIBRARIES}
    )


# Build executable `ir_asm`.
add_executable(ir_asm
    ir_asm.cc
    )


set_target_properties(ir_asm PROPERTIES COMPILE_FLAGS "-O3")


target_link_libraries(ir_asm
    libcorevm
    ${DEPENDENCY_LINK_LIBRARIES}
    )


# Validate IR tools.
add_custom_command(OUTPUT ir_validation_output WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS ir_dis ir_asm
    COMMAND python tools/ir_tools_integrity.py  --build_dir $<TARGET_FILE_DIR:ir_asm>)
add_custom_target(ir_validation_target ALL DEPENDS ir_validation_output)


# Generate artifacts.
set(ARTIFACTS_DIR "${PROJECT_SOURCE_DIR}/build/artifacts")
add_custom_command(TARGET extract_metadata POST_BUILD COMMAND
    mkdir -p ${ARTIFACTS_DIR}/build/artifacts)
add_custom_command(TARGET extract_metadata POST_BUILD COMMAND
    $<TARGET_FILE:extract_metadata> ${ARTIFACTS_DIR}/corevm_metadata.json)
