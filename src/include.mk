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
COREVM_DIR=corevm

SOURCES += $(TOP_DIR)/$(SRC)/$(MEMORY)/sequential_allocation_scheme.cc

SOURCES += $(TOP_DIR)/$(SRC)/$(DYOBJ)/flags.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(DYOBJ)/util.cc

SOURCES += $(TOP_DIR)/$(SRC)/$(GC)/mark_and_sweep_garbage_collection_scheme.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(GC)/reference_count_garbage_collection_scheme.cc

SOURCES += $(TOP_DIR)/$(SRC)/$(TYPES)/interfaces.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(TYPES)/native_array.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(TYPES)/native_map.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(TYPES)/native_string.cc

SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/closure.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/compartment.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/frame.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/gc_rule.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/instr.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/instr_info.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/invocation_ctx.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/native_types_pool.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/process.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/process_runner.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/sighandler.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/sighandler_registrar.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/vector.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(RUNTIME)/utils.cc

SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/bytecode_loader_text.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/bytecode_loader_binary.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/configuration.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/program.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/runner.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/signal_vector_loader.cc
SOURCES += $(TOP_DIR)/$(SRC)/$(FRONTEND)/utils.cc

EXEC_SOURCES += $(TOP_DIR)/$(SRC)/$(COREVM_DIR)/main.cc
