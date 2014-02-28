CXX=c++
CFLAGS=-Wall -arch x86_64 -std=c++11 -DGTEST_HAS_TR1_TUPLE=0

GTEST_INCLUDE=-I/usr/local/gtest-1.6.0/include
LIBGTEST=/usr/lib/libgtest.a
LIBSNEAKER=/usr/local/lib/libsneaker.a

LIBRARIES=$(LIBGTEST) $(LIBSNEAKER)

SRC=\
	../../src/memory/sequential_allocation_scheme.cc

heap_mem_allocator_unittest.test: heap_mem_allocator_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(SRC) heap_mem_allocator_unittest.cc -o heap_mem_allocator_unittest.test

all: heap_mem_allocator_unittest.test

clean:
	rm -rf heap_mem_allocator_unittest.test