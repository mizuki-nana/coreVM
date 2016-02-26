# The MIT License (MIT)

# Copyright (c) 2015 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

MEMORY=memory
DYOBJ=dyobj
GC=gc
TYPES=types
RUNTIME=runtime
FRONTEND=frontend

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(MEMORY)/allocation_policy_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(MEMORY)/allocator_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(MEMORY)/block_allocator_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(MEMORY)/object_container_unittest.cc

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(DYOBJ)/dynamic_object_heap_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(DYOBJ)/dynamic_object_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(DYOBJ)/heap_allocator_unittest.cc

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(GC)/garbage_collection_unittest.cc

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/unary_operators_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/binary_operators_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/interfaces_test.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_array_type_interfaces_test.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_array_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_map_type_interfaces_test.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_map_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_string_type_interfaces_test.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_string_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/native_type_handle_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(TYPES)/variant_unittest.cc

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/compartment_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/frame_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/instr_info_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/instrs_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/invocation_ctx_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/native_types_pool_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/process_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/sighandler_registrar_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(RUNTIME)/utils_unittest.cc

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(FRONTEND)/configuration_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(FRONTEND)/program_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(FRONTEND)/runner_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(FRONTEND)/signal_vector_loader_unittest.cc
TEST_SOURCES += $(TOP_DIR)/$(TESTS)/$(FRONTEND)/utils_unittest.cc

TEST_SOURCES += $(TOP_DIR)/$(TESTS)/main.cc
