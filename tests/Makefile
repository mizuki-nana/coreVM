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


.PHONY: memory
memory:
	-cd $(MEMORY) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: dyobj
dyobj:
	-cd $(DYOBJ) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: gc
gc:
	-cd $(GC) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: types
types:
	-cd $(TYPES) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(EXTRA_CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: runtime
runtime:
	-cd $(RUNTIME) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: frontend
frontend:
	-cd $(FRONTEND) && for f in ./*.cc; do ($(CXX) $(CFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: all
all: memory dyobj gc types runtime frontend

.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do (rm -rf $$dir*.test); done
