CXX=c++
CFLAGS=-Wall -arch x86_64 -std=c++11 -DGTEST_HAS_TR1_TUPLE=0

GTEST_INCLUDE=-I/usr/local/gtest-1.6.0/include
LIBGTEST=/usr/lib/libgtest.a
LIBSNEAKER=/usr/local/lib/libsneaker.a

LIBRARIES=$(LIBGTEST) $(LIBSNEAKER)

SRC=\
	../../src/memory/*.cc \
	../../src/dyobj/*.cc \
	../../src/gc/*.cc

mark_and_sweep_garbage_collection_scheme_unittest.test: mark_and_sweep_garbage_collection_scheme_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(SRC) mark_and_sweep_garbage_collection_scheme_unittest.cc -o mark_and_sweep_garbage_collection_scheme_unittest.test

all:\
	mark_and_sweep_garbage_collection_scheme_unittest.test

clean:
	rm -rf *.test