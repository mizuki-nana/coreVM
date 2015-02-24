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

CXX=`which clang++`
CFLAGS=-Wall -std=c++11
EXTRA_CFLAGS=-Wno-deprecated

LIBGTEST=/usr/lib/libgtest.a
LIBSNEAKER=/usr/local/lib/libsneaker.a
LIBRARIES=$(LIBGTEST)

LIBCOREVM=libcorevm.a
LFLAGS=$(LIBCOREVM) $(LIBSNEAKER) -lgtest -lpthread

MAIN=./src/main.cc

COREVM=coreVM

AR=ar
ARFLAGS=rvs

INCLUDE=./include
SRC=./src
TESTS=./tests

SUBDIRS=$(SRC) $(TESTS)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CFLAGS += -D LINUX
	LFLAGS += -lboost_system -lboost_regex -lboost_program_options
endif
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	LFLAGS += -lboost_system-mt -lboost_regex-mt -lboost_program_options
endif


export GTEST_COLOR=true


.PHONY: all
all:
	@$(MAKE) -C $(SRC)
	@find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBCOREVM)
	@echo "\033[35mGenerated $(LIBCOREVM)"
	@$(CXX) $(CFLAGS) $(EXTRA_CFLAGS) $(LIBRARIES) $(MAIN) $(LFLAGS) -o $(COREVM)
	@echo "\033[35mGenerated $(COREVM)"

.PHONY: test
test:
	@$(MAKE) -C $(TESTS)
	@-for dir in $(TESTS); do (find $$dir -type f -name "*.test" -exec '{}' \;); done
	@echo "\033[32mTests run completed...\033[39m";

.PHONY: clean
clean:
	@-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done
	@-rm -rf $(LIBCOREVM)
	@-rm -rf $(COREVM)
