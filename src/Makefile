CXX=c++
CXXFLAGS=-w -std=c++11
EXTRA_CXXFLAGS=-Wno-deprecated -fpermissive 
AR=ar
ARFLAGS=rvs

MEMORY=./memory/
DYOBJ=./dyobj/
GC=./gc/
TYPES=./types/
RUNTIME=./runtime/
FRONTEND=./frontend/

SUBDIRS=$(MEMORY) $(DYOBJ) $(GC) $(TYPES) $(RUNTIME) $(FRONTEND)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CXXFLAGS += -D LINUX
endif
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
endif

.PHONY: all
all: memory dyobj gc types runtime frontend

.PHONY: memory
memory:
	cd $(MEMORY) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: dyobj
dyobj:
	cd $(DYOBJ) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: gc
gc:
	cd $(GC) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: types
types:
	cd $(TYPES) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c $$f); done

.PHONY: runtime
runtime:
	cd $(RUNTIME) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c $$f); done

.PHONY: frontend
frontend:
	cd $(FRONTEND) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do (rm -rf $$dir*.o $$dir*.a); done
