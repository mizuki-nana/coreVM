/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include <sneaker/testing/_unittest.h>
#include "../../include/memory/sequential_allocation_scheme.h"
#include "../../include/dyobj/heap_allocator.h"


class heap_allocator_unit_test : public ::testing::Test {
public:
  typedef corevm::memory::first_fit_allocation_scheme AllocationScheme;
  typedef corevm::memory::best_fit_allocation_scheme OtherAllocationScheme;
};


class heap_allocator_simple_test : public heap_allocator_unit_test {};


TEST_F(heap_allocator_simple_test, TestAllocation)
{
  corevm::dyobj::heap_allocator<int, AllocationScheme> allocator;
  int *a = allocator.allocate(10);

  assert(a);

  for(int i = 0; i < 10; ++i) a[i] = i;
  for(int i = 0; i < 10; ++i) ASSERT_EQ(i, a[i]);

  allocator.deallocate(a, 10);
}

TEST_F(heap_allocator_simple_test, TestAllocationAndDestroy)
{
  corevm::dyobj::heap_allocator<std::string, AllocationScheme> allocator;
  std::string* s = allocator.allocate(2);

  allocator.construct(s, "foo");
  allocator.construct(s + 1, "bar");

  ASSERT_STREQ("foo", s[0].c_str());
  ASSERT_STREQ("bar", s[1].c_str());

  allocator.destroy(s);
  allocator.destroy(s + 1);
  allocator.deallocate(s, 2);
}


class heap_allocator_container_test : public heap_allocator_unit_test {
public:
  template<typename T>
  using vector_type = typename std::vector<T, corevm::dyobj::heap_allocator<T, AllocationScheme>>;
};


TEST_F(heap_allocator_container_test, TestInitialization)
{
  vector_type<int> v(10);
  ASSERT_EQ(10, v.capacity());
  ASSERT_EQ(10, v.size());
}

TEST_F(heap_allocator_container_test, TestPushback)
{
  vector_type<char> *v = new vector_type<char>();
  assert(v);

  v->push_back('a');
  v->push_back('b');
  v->push_back('c');

  ASSERT_EQ(3, v->size());

  delete v;
}

TEST_F(heap_allocator_container_test, TestPushBackWithPointers)
{
  vector_type<int*> v;

  v.push_back(new int(0));
  v.push_back(new int(1));
  v.push_back(new int(2));

  ASSERT_EQ(0, static_cast<int>(*v[0]));
  ASSERT_EQ(1, static_cast<int>(*v[1]));
  ASSERT_EQ(2, static_cast<int>(*v[2]));

  ASSERT_EQ(3, v.size());
}


class heap_allocator_equality_test : public heap_allocator_unit_test {};


TEST_F(heap_allocator_equality_test, TestEquality1)
{
  corevm::dyobj::heap_allocator<int, AllocationScheme> allocator1;
  corevm::dyobj::heap_allocator<int, AllocationScheme> allocator2;

  ASSERT_TRUE(allocator1 == allocator2);
  ASSERT_FALSE(allocator1 != allocator2);
}

TEST_F(heap_allocator_equality_test, TestEquality2)
{
  corevm::dyobj::heap_allocator<int, AllocationScheme, 100> allocator1;
  corevm::dyobj::heap_allocator<int, AllocationScheme, 200> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}

TEST_F(heap_allocator_equality_test, TestEquality3)
{
  corevm::dyobj::heap_allocator<int, AllocationScheme, 100> allocator1;
  corevm::dyobj::heap_allocator<float, AllocationScheme, 100> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}

TEST_F(heap_allocator_equality_test, TestEquality4)
{
  corevm::dyobj::heap_allocator<int, AllocationScheme, 100> allocator1;
  corevm::dyobj::heap_allocator<int, OtherAllocationScheme, 100> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}

TEST_F(heap_allocator_equality_test, TestEquality5)
{
  corevm::dyobj::heap_allocator<int, AllocationScheme, 100> allocator1;
  corevm::dyobj::heap_allocator<float, OtherAllocationScheme, 200> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}
