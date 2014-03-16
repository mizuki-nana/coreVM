CXX=c++
CFLAGS=-Wall -arch x86_64 -std=c++11 -DGTEST_HAS_TR1_TUPLE=0

GTEST_INCLUDE=-I/usr/local/gtest-1.6.0/include
LIBGTEST=/usr/lib/libgtest.a
LIBSNEAKER=/usr/local/lib/libsneaker.a

LIBRARIES=$(LIBGTEST) $(LIBSNEAKER)

SRC=\
	../../src/memory/*.cc \
	../../src/dyobj/*.cc  \
	../../src/gc/*.cc

dyobj_heap_alloc_policy_unittest.test: dyobj_heap_alloc_policy_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(SRC) dyobj_heap_alloc_policy_unittest.cc -o dyobj_heap_alloc_policy_unittest.test

dyobj_heap_allocator_unittest.test: dyobj_heap_allocator_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(SRC) dyobj_heap_allocator_unittest.cc -o dyobj_heap_allocator_unittest.test

dynamic_object_unittest.test: dynamic_object_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(SRC) dynamic_object_unittest.cc -o dynamic_object_unittest.test

dynamic_object_heap_unittest.test: dynamic_object_heap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(SRC) dynamic_object_heap_unittest.cc -o dynamic_object_heap_unittest.test

all:\
	dyobj_heap_alloc_policy_unittest.test \
	dyobj_heap_allocator_unittest.test \
	dynamic_object_unittest.test \
	dynamic_object_heap_unittest.test

clean:
	rm -rf *.test
