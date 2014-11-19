# The MIT License (MIT)

# Copyright (c) 2014 Yanzheng Li

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

CXX=`which clang++`
CFLAGS=-Wall -std=c++11
EXTRA_CFLAGS=-Wno-deprecated

LIBGTEST=/usr/lib/libgtest.a
LIBSNEAKER=/usr/local/lib/libsneaker.a
LIBRARIES=$(LIBGTEST)

LIBCOREVM=../../libcorevm.a

LFLAGS=$(LIBCOREVM) $(LIBSNEAKER) -lgtest -lpthread

MEMORY=./memory/
DYOBJ=./dyobj/
GC=./gc/
TYPES=./types/
RUNTIME=./runtime/
FRONTEND=./frontend/

SUBDIRS=$(MEMORY) $(DYOBJ) $(GC) $(TYPES) $(RUNTIME) $(FRONTEND)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CFLAGS += -D LINUX
	LFLAGS += -lboost_system -lboost_regex
endif
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	LFLAGS += -lboost_system-mt -lboost_regex-mt
endif


.PHONY: all
all: memory dyobj gc types runtime frontend

.PHONY: memory
memory:
	cd $(MEMORY) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: dyobj
dyobj:
	cd $(DYOBJ) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: gc
gc:
	cd $(GC) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: types
types:
	cd $(TYPES) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(EXTRA_CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: runtime
runtime:
	cd $(RUNTIME) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: frontend
frontend:
	cd $(FRONTEND) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do (rm -rf $$dir*.test); done
