CXX=c++
CFLAGS=-Wall -arch x86_64 -std=c++11 -DGTEST_HAS_TR1_TUPLE=0 -Wno-deprecated

GTEST_INCLUDE=-I/usr/local/gtest-1.6.0/include
LIBGTEST=/usr/lib/libgtest.a
LIBSNEAKER=/usr/local/lib/libsneaker.a

LIBRARIES=$(LIBGTEST) $(LIBSNEAKER)


native_string_unittest.test: native_string_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) native_string_unittest.cc -o native_string_unittest.test


native_type_handle_unittest.test: native_type_handle_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) native_type_handle_unittest.cc -o native_type_handle_unittest.test


all: native_string_unittest.test \
		 native_type_handle_unittest.test

clean:
	rm -rf *.test
