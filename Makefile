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

# TODO: [COREVM-161] Cleanup final build dependencies in Makefile


## -----------------------------------------------------------------------------

TOP_DIR=$(CURDIR)
BUILD_DIR=$(TOP_DIR)/build
RLS_BUILD_DIR=$(BUILD_DIR)/release
DBG_BUILD_DIR=$(BUILD_DIR)/debug
BIN=$(TOP_DIR)/bin
SRC=src
TESTS=tests
BENCHMARKS=benchmarks
TOOLS=tools
ARTIFACTS=artifacts
PYTHON_DIR=python
ARCHIVES=$(TOP_DIR)/archives
MICRO_BENCHMARKS_ARCHIVES_DIR=$(ARCHIVES)/benchmarks/micro
BUILD_INFO_ARCHIVES_DIR=$(ARCHIVES)/build_info
PYTHON_TESTS_DIR=$(PYTHON_DIR)/tests
MAIN_CC=$(SRC)/corevm/main.cc
COMPILED_BYTECODE_SCHEMA_HEADER=$(SRC)/corevm/corevm_bytecode_schema.h

PYTHON=`which python`
CXX=`which clang++`
CXXFLAGS=-Wall -std=c++11 -I$(TOP_DIR)/$(SRC)
EXTRA_CXXFLAGS=-O3
DBG_CXXFLAGS=-D__DEBUG__=1 -DINSTRUMENTAL_MEASUREMENT=1

LFLAGS=-lsneaker -lpthread -lavrocpp_s
DBG_LFLAGS=-lsneaker -lpthread -lavrocpp_s

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LFLAGS += -lboost_system -lboost_regex -lboost_program_options -lboost_iostreams
	DBG_LFLAGS += -lboost_system -lboost_regex -lboost_program_options -lboost_iostreams -lboost_timer
endif
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	DBG_CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	LFLAGS += -lboost_system-mt -lboost_regex-mt -lboost_program_options
	DBG_LFLAGS += -lboost_system-mt -lboost_regex-mt -lboost_program_options
endif

AR=ar
ARFLAGS=rvs

LIBCOREVM=libcorevm.a
LIBCOREVM_DBG=libcorevm_dbg.a
COREVM=coreVM
COREVM_DBG=coreVM_dbg
EXTRACT_METADATA=$(BIN)/extract_metadata.cc.out
BOOTSTRAP_TESTS=bootstrap_tests.py
PYTHON_TESTS=python_tests
RUN_TESTS=run_tests
RUN_BENCHMARKS=run_benchmarks

COREVM_METADATA=$(ARTIFACTS)/corevm_metadata.json
COMPILE_BYTECODE_SCHEMA=scripts/compile_bytecode_schema.sh

## -----------------------------------------------------------------------------

export GTEST_COLOR=true

## -----------------------------------------------------------------------------

include src/include.mk
include tests/include.mk
include benchmarks/include.mk

## -----------------------------------------------------------------------------

OBJECTS = $(patsubst $(TOP_DIR)/%.cc, $(RLS_BUILD_DIR)/%.o, $(SOURCES))
DBG_OBJECTS = $(patsubst $(TOP_DIR)/%.cc, $(DBG_BUILD_DIR)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst $(TOP_DIR)/%.cc,$(RLS_BUILD_DIR)/%.o,$(TEST_SOURCES))
BENCHMARK_OBJECTS = $(patsubst $(TOP_DIR)/%.cc,$(RLS_BUILD_DIR)/%.o,$(BENCHMARK_SOURCES))

## -----------------------------------------------------------------------------

.PHONY: all
all: $(LIBCOREVM_DBG) $(COREVM_DBG) $(LIBCOREVM) $(COREVM) $(TOOLS) $(ARTIFACTS) $(TESTS) $(PYTHON_TESTS) $(BENCHMARKS)

## -----------------------------------------------------------------------------

$(RLS_BUILD_DIR)/%.o: $(TOP_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c $(TOP_DIR)/$*.cc -o $@

## -----------------------------------------------------------------------------

.PHONY: $(COMPILED_BYTECODE_SCHEMA_HEADER)
$(COMPILED_BYTECODE_SCHEMA_HEADER):
	sh $(COMPILE_BYTECODE_SCHEMA)

## -----------------------------------------------------------------------------

$(LIBCOREVM): $(COMPILED_BYTECODE_SCHEMA_HEADER) $(OBJECTS)
	mkdir -p $(@D)
	@find $(RLS_BUILD_DIR)/$(SRC) -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM)
	@echo "\033[35mGenerated $(LIBCOREVM)\033[0m"
	mkdir -p $(BUILD_INFO_ARCHIVES_DIR)
	exec stat --format="File: %n - Size: %s bytes - Date modified: %z" ./$(LIBCOREVM) 2>&1 | tee $(BUILD_INFO_ARCHIVES_DIR)/info.build.libcorevm.$(shell date +"%Y-%m-%dT%H:%M:%S").txt

## -----------------------------------------------------------------------------

.PHONY: $(COREVM)
$(COREVM): $(LIBCOREVM)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(MAIN_CC) -o $(COREVM) $^ $(LFLAGS)
	@echo "\033[35mGenerated $(COREVM)\033[0m"
	mkdir -p $(BUILD_INFO_ARCHIVES_DIR)
	exec stat --format="File: %n - Size: %s bytes - Date modified: %z" ./$(COREVM) 2>&1 | tee $(BUILD_INFO_ARCHIVES_DIR)/info.build.corevm.$(shell date +"%Y-%m-%dT%H:%M:%S").txt

## -----------------------------------------------------------------------------

$(DBG_BUILD_DIR)/%.o: $(TOP_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DBG_CXXFLAGS) -c $(TOP_DIR)/$*.cc -o $@

## -----------------------------------------------------------------------------

$(LIBCOREVM_DBG): $(COMPILED_BYTECODE_SCHEMA_HEADER) $(DBG_OBJECTS)
	mkdir -p $(@D)
	@find $(DBG_BUILD_DIR)/$(SRC) -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM_DBG)
	@echo "\033[35mGenerated $(LIBCOREVM_DBG)\033[0m"
	mkdir -p $(BUILD_INFO_ARCHIVES_DIR)
	exec stat --format="File: %n - Size: %s bytes - Date modified: %z" ./$(LIBCOREVM_DBG) 2>&1 | tee $(BUILD_INFO_ARCHIVES_DIR)/info.build.libcorevm_dbg.$(shell date +"%Y-%m-%dT%H:%M:%S").txt

## -----------------------------------------------------------------------------

.PHONY: $(COREVM_DBG)
$(COREVM_DBG): $(LIBCOREVM_DBG)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DBG_CXXFLAGS) $(MAIN_CC) -o $(COREVM_DBG) $^ $(DBG_LFLAGS)
	@echo "\033[35mGenerated $(COREVM_DBG)\033[0m"
	mkdir -p $(BUILD_INFO_ARCHIVES_DIR)
	exec stat --format="File: %n - Size: %s bytes - Date modified: %z" ./$(COREVM_DBG) 2>&1 | tee $(BUILD_INFO_ARCHIVES_DIR)/info.build.corevm_dbg.$(shell date +"%Y-%m-%dT%H:%M:%S").txt

## -----------------------------------------------------------------------------

.PHONY: $(TOOLS)
$(TOOLS): $(LIBCOREVM)
	mkdir -p $(BIN)
	-for f in $(TOOLS)/*.cc; do ($(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $$f -o $$f.out $^ $(LFLAGS); mv $$f.out $(BIN)/); done

## -----------------------------------------------------------------------------

.PHONY: $(ARTIFACTS)
$(ARTIFACTS): $(TOOLS)
	mkdir -p $(ARTIFACTS)
	$(EXTRACT_METADATA) $(COREVM_METADATA)

## -----------------------------------------------------------------------------

.PHONY: $(PYTHON_TESTS)
$(PYTHON_TESTS): $(COREVM) $(ARTIFACTS)
	@$(PYTHON) $(PYTHON_DIR)/$(BOOTSTRAP_TESTS)

## -----------------------------------------------------------------------------

.PHONY: $(TESTS)
$(TESTS): $(LIBCOREVM) $(TEST_OBJECTS)
	mkdir -p $(@D)
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(TEST_OBJECTS) -o $(BIN)/$(RUN_TESTS) libcorevm.a $(LFLAGS) -lgtest
	exec $(BIN)/$(RUN_TESTS)
	@echo "\033[32mTests run completed...\033[0m";

## -----------------------------------------------------------------------------

.PHONY: $(BENCHMARKS)
$(BENCHMARKS): $(LIBCOREVM) $(BENCHMARK_OBJECTS)
	mkdir -p $(@D)
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(BENCHMARK_OBJECTS) -o $(BIN)/$(RUN_BENCHMARKS) libcorevm.a $(LFLAGS) -lbenchmark
	mkdir -p $(MICRO_BENCHMARKS_ARCHIVES_DIR)
	exec $(BIN)/$(RUN_BENCHMARKS) 2>&1 | tee $(MICRO_BENCHMARKS_ARCHIVES_DIR)/micro.benchmark.corevm.$(shell date +"%Y-%m-%dT%H:%M:%S").txt
	@echo "\033[32mBenchmarks run completed...\033[0m";

## -----------------------------------------------------------------------------

.PHONY: clean
clean:
	@-rm -rf $(BUILD_DIR)
	@-rm -rf $(BIN)
	@-rm -rf $(ARTIFACTS)
	@-rm -f $(COMPILED_BYTECODE_SCHEMA_HEADER)
	@-rm -f $(LIBCOREVM)
	@-rm -f $(COREVM)
	@-rm -f $(LIBCOREVM_DBG)
	@-rm -f $(COREVM_DBG)
	@-rm -f $(PYTHON_TESTS_DIR)/*.tmp.py
	@-rm -f $(PYTHON_TESTS_DIR)/*.tmp.py.core

## -----------------------------------------------------------------------------
