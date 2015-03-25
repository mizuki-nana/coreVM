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

TOP_DIR=$(CURDIR)
BUILD_DIR=$(TOP_DIR)/build
BIN=$(TOP_DIR)/bin
SRC=src
TESTS=tests
TOOLS=tools
PYTHON_DIR=python
MAIN_CC=$(SRC)/corevm/main.cc

PYTHON=`which python`
CXX=`which clang++`
CXXFLAGS=-Wall -std=c++11 -I$(TOP_DIR)/$(SRC)
EXTRA_CXXFLAGS=-Wno-deprecated

LFLAGS=-lsneaker -lpthread
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CXXFLAGS += -D LINUX
	LFLAGS += -lboost_system -lboost_regex -lboost_program_options
endif
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	LFLAGS += -lboost_system-mt -lboost_regex-mt -lboost_program_options
endif

AR=ar
ARFLAGS=rvs

LIBCOREVM=libcorevm.a
COREVM=coreVM

BOOTSTRAP_TESTS=bootstrap_tests.py
PYTHON_TESTS=python_tests
RUN_TESTS=run_tests


export GTEST_COLOR=true


include src/include.mk
include tests/include.mk


OBJECTS = $(patsubst $(TOP_DIR)/%.cc, $(BUILD_DIR)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst $(TOP_DIR)/%.cc,$(BUILD_DIR)/%.o,$(TEST_SOURCES))


.PHONY: all
all: $(LIBCOREVM) $(COREVM) $(TOOLS) $(TESTS) $(PYTHON_TESTS)


$(BUILD_DIR)/%.o: $(TOP_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c $(TOP_DIR)/$*.cc -o $@


$(LIBCOREVM): $(OBJECTS)
	mkdir -p $(@D)
	@find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM)
	@echo "\033[35mGenerated $(LIBCOREVM)\033[0m"


.PHONY: $(COREVM)
$(COREVM): $(LIBCOREVM)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(MAIN_CC) -o $(COREVM) $^ $(LFLAGS)
	@echo "\033[35mGenerated $(COREVM)\033[0m"


.PHONY: $(TOOLS)
$(TOOLS): $(LIBCOREVM)
	mkdir -p $(BIN)
	-for f in $(TOOLS)/*.cc; do ($(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $$f -o $$f.out $^ $(LFLAGS); mv $$f.out $(BIN)/); done


.PHONY: $(PYTHON_TESTS)
$(PYTHON_TESTS): $(COREVM)
	@$(PYTHON) $(PYTHON_DIR)/$(BOOTSTRAP_TESTS)


.PHONY: $(TESTS)
$(TESTS): $(TEST_OBJECTS)
	mkdir -p $(@D)
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(TEST_OBJECTS) -o $(BIN)/$(RUN_TESTS) libcorevm.a $(LFLAGS) -lgtest
	exec $(BIN)/$(RUN_TESTS)
	@echo "\033[32mTests run completed...\033[0m";


.PHONY: clean
clean:
	@-rm -rf $(BUILD_DIR)
	@-rm -rf $(BIN)
	@-rm -f $(LIBCOREVM)
	@-rm -f $(COREVM)
