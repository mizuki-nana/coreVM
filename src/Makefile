CXX=c++
CXXFLAGS=-Wall -std=c++11
AR=ar
ARFLAGS=rvs

MEMORY=./memory/
DYOBJ=./dyobj/
GC=./gc/
TYPES=./types/

SUBDIRS=$(MEMORY) $(DYOBJ) $(GC) $(TYPES)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CXXFLAGS += -D LINUX
endif
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
endif

.PHONY: memory
memory:
	-cd $(MEMORY) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: dyobj
dyobj:
	-cd $(DYOBJ) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: gc
gc:
	-cd $(GC) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: types
types:
	-cd $(TYPES) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: all
all: memory dyobj gc types

.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do (rm -rf $$dir*.o $$dir*.a); done
