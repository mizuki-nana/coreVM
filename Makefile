# The MIT License (MIT)

# Copyright (c) 2017 Yanzheng Li

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


## Top level Makefile. Used to bootstrap cmake to build from source.


## -----------------------------------------------------------------------------

TOP_DIR=$(CURDIR)
BUILD_DIR=$(TOP_DIR)/build
BUILD_GCC_DIR=$(TOP_DIR)/build_gcc
PYTHON_DIR=python
PYTHON_TESTS_DIR=$(PYTHON_DIR)/tests
BOOTSTRAP_TESTS=bootstrap_tests.py
BOOTSTRAP_TESTS_ARGS=--color
DOCS_DIR=./docs
DOCS_BUILD_DIR=$(DOCS_DIR)/build
SANITY_TESTS_ARGS=--sanity-test
DYNAMIC_ANALYSIS_TESTS_ARGS=--dynamic-analysis
PYTHON=`which python`
RESOURCES_DIR=$(TOP_DIR)/resources
SRC_DIR=$(TOP_DIR)/src
TESTS_DIR=$(TOP_DIR)/tests
BENCHMARKS_DIR=$(TOP_DIR)/benchmarks
SCHEMAS_DIR=$(TOP_DIR)/schemas
TOOLS_DIR=$(TOP_DIR)/tools
VALGRIND_DIR=$(TOP_DIR)/valgrind
IR_FORMAT_FILEPATH=$(SRC_DIR)/ir/format.h

## -----------------------------------------------------------------------------

VERSION=v0.1.0-alpha-1.0
LIBCOREVM=libcorevm
LIBCOREVM_A=$(LIBCOREVM).a
LIBCOREVM_GZIP=$(LIBCOREVM)-$(VERSION).tar.gz
LIBCOREVM_DBG=libcorevm_dbg
LIBCOREVM_DBG_A=$(LIBCOREVM_DBG).a

LIB_INSTALL_DIR=/usr/local/lib
LIB_INSTALL_PATH=$(LIB_INSTALL_DIR)/$(LIBCOREVM_A)
LIB_DBG_INSTALL_PATH=$(LIB_INSTALL_DIR)/$(LIBCOREVM_DBG_A)
HEADER_INSTALL_PATH=/usr/local/include/corevm/

## -----------------------------------------------------------------------------

BASE_TARGETS=build python_tests docs

ifeq ($(full), 1)
	BUILD_TARGETS=$(BASE_TARGETS) benchmarks
	BUILD_TARGET_CMAKE_ARGS=-DBUILD_BENCHMARKS=ON
else ifeq ($(full), 2)
	BUILD_TARGETS=$(BASE_TARGETS) benchmarks sanity_tests dynamic_analysis_tests
	BUILD_TARGET_CMAKE_ARGS=-DBUILD_BENCHMARKS=ON -DBUILD_SANITY_BIN=ON
else
	BUILD_TARGETS=$(BASE_TARGETS)
	BUILD_TARGET_CMAKE_ARGS=
endif

ifeq ($(travis_ci), 1)
	BUILD_TARGET_CMAKE_ARGS+=-DBUILD_BENCHMARKS_STRICT=OFF
endif

ifeq ($(build_gcc_target), 1)
	BUILD_TARGETS+=build_gcc
endif

BUILD_GCC_TARGET_CMAKE_ARGS=-DCMAKE_CXX_COMPILER=g++

## -----------------------------------------------------------------------------

.PHONY: all
all: $(BUILD_TARGETS)

## -----------------------------------------------------------------------------

.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR); cmake $(BUILD_TARGET_CMAKE_ARGS) ../ && make

## -----------------------------------------------------------------------------

.PHONY:
build_gcc:
	@echo "Compiling using GCC..."
	mkdir -p $(BUILD_GCC_DIR)
	cd $(BUILD_GCC_DIR); cmake $(BUILD_TARGET_CMAKE_ARGS) $(BUILD_GCC_TARGET_CMAKE_ARGS) ../ && make

## -----------------------------------------------------------------------------

.PHONY: python_tests
python_tests:
	$(PYTHON) $(PYTHON_DIR)/$(BOOTSTRAP_TESTS) $(BOOTSTRAP_TESTS_ARGS)

## -----------------------------------------------------------------------------

.PHONY: docs
docs:
	$(MAKE) -C docs html

## -----------------------------------------------------------------------------

.PHONY: sanity_tests
sanity_tests:
	export ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer-3.4)
	$(PYTHON) $(PYTHON_DIR)/$(BOOTSTRAP_TESTS) $(SANITY_TESTS_ARGS)

## -----------------------------------------------------------------------------

.PHONY: dynamic_analysis_tests
dynamic_analysis_tests:
	$(PYTHON) $(PYTHON_DIR)/$(BOOTSTRAP_TESTS) $(DYNAMIC_ANALYSIS_TESTS_ARGS)

## -----------------------------------------------------------------------------

.PHONY: benchmarks
benchmarks:
	$(BUILD_DIR)/benchmarks/run_benchmarks

## -----------------------------------------------------------------------------

.PHONY: install
install:
	@echo "\033[32mInstalling $(VERSION)...\033[39m";
	sudo mkdir -p $(HEADER_INSTALL_PATH)
	sudo cp -vr src/api/*.h $(HEADER_INSTALL_PATH)
	sudo mkdir -p $(LIB_INSTALL_DIR)
	sudo cp -v $(BUILD_DIR)/src/$(LIBCOREVM_A) $(LIB_INSTALL_DIR)
	sudo cp -v $(BUILD_DIR)/src/$(LIBCOREVM_DBG_A) $(LIB_INSTALL_DIR)
	tar -zcvf $(LIBCOREVM_GZIP) $(SRC_DIR) $(TESTS_DIR) $(BENCHMARKS_DIR) $(RESOURCES_DIR) $(SCHEMAS_DIR) $(TOOLS_DIR) $(VALGRIND_DIR) $(DOCS) LICENSE Makefile README.md
	@echo "\033[32mInstall complete...\033[39m";
	@echo "\033[32mHeader files: \033[35m$(HEADER_INSTALL_PATH)\033[39m";
	@echo "\033[32mStatic lib: \033[35m$(LIB_INSTALL_PATH)\033[39m";
	@echo "\033[32mStatic lib: \033[35m$(LIB_DBG_INSTALL_PATH)\033[39m";

## -----------------------------------------------------------------------------

.PHONY: uninstall
uninstall:
	@echo "\033[32mUninstalling coreVM version $(VERSION)\033[39m";
	sudo rm -rf $(HEADER_INSTALL_PATH)
	sudo rm -f $(LIB_INSTALL_PATH)
	sudo rm -f $(LIB_DBG_INSTALL_PATH)
	rm -rf ./*.tar.gz
	@echo "\033[32mUninstalling complete.\033[39m";

## -----------------------------------------------------------------------------

.PHONY: clean
clean:
	# TODO: [COREVM-579] Re-enable IR schema compilation during builds
	# rm -f $(IR_FORMAT_FILEPATH)
	rm -rf $(BUILD_DIR)
	rm -rf $(BUILD_GCC_DIR)
	rm -f $(PYTHON_TESTS_DIR)/*.tmp.py
	rm -f $(PYTHON_TESTS_DIR)/*.tmp.py.core
	rm -rf $(DOCS_BUILD_DIR)/*

## -----------------------------------------------------------------------------
