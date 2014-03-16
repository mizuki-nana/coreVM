#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dyobj_heap_allocator.h"
#include "../../include/dyobj/dyobj_heap_alloc_policy.h"


class dyobj_heap_allocator_unit_test : public ::testing::Test {};
class dyobj_heap_allocator_simple_test : public dyobj_heap_allocator_unit_test {};


TEST_F(dyobj_heap_allocator_simple_test, TestAllocation)
{
  corevm::dyobj::dyobj_heap_allocator<int> allocator;
  int *a = allocator.allocate(10);

  assert(a);

  for(int i = 0; i < 10; ++i) a[i] = i;
  for(int i = 0; i < 10; ++i) ASSERT_EQ(i, a[i]);

  allocator.deallocate(a, 10);
}

TEST_F(dyobj_heap_allocator_simple_test, TestAllocationAndDestroy)
{
  corevm::dyobj::dyobj_heap_allocator<std::string> allocator;
  std::string* s = allocator.allocate(2);

  allocator.construct(s, "foo");
  allocator.construct(s + 1, "bar");

  ASSERT_STREQ("foo", s[0].c_str());
  ASSERT_STREQ("bar", s[1].c_str());

  allocator.destroy(s);
  allocator.destroy(s + 1);
  allocator.deallocate(s, 2);
}


class dyobj_heap_allocator_container_test : public dyobj_heap_allocator_unit_test {
public:
  template<typename T>
  using vector_type = typename std::vector<T, corevm::dyobj::dyobj_heap_allocator<T>>;
};


TEST_F(dyobj_heap_allocator_container_test, TestInitialization)
{
  vector_type<int> v(10);
  ASSERT_EQ(10, v.capacity());
  ASSERT_EQ(10, v.size());
}

TEST_F(dyobj_heap_allocator_container_test, TestPushback)
{
  vector_type<char> *v = new vector_type<char>();
  assert(v);

  v->push_back('a');
  v->push_back('b');
  v->push_back('c');

  ASSERT_EQ(3, v->size());

  delete v;
}

TEST_F(dyobj_heap_allocator_container_test, TestPushBackWithPointers)
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


class dyobj_heap_allocator_equality_test : public dyobj_heap_allocator_unit_test {};


TEST_F(dyobj_heap_allocator_equality_test, TestEquality1)
{
  corevm::dyobj::dyobj_heap_allocator<int> allocator1;
  corevm::dyobj::dyobj_heap_allocator<int> allocator2;

  ASSERT_TRUE(allocator1 == allocator2);
  ASSERT_FALSE(allocator1 != allocator2);
}

TEST_F(dyobj_heap_allocator_equality_test, TestEquality2)
{
  corevm::dyobj::dyobj_heap_allocator<int, 100> allocator1;
  corevm::dyobj::dyobj_heap_allocator<int, 200> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}

TEST_F(dyobj_heap_allocator_equality_test, TestEquality3)
{
  corevm::dyobj::dyobj_heap_allocator<int, 100> allocator1;
  corevm::dyobj::dyobj_heap_allocator<float, 100> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}

TEST_F(dyobj_heap_allocator_equality_test, TestEquality4)
{
  corevm::dyobj::dyobj_heap_allocator<int, 100> allocator1;
  corevm::dyobj::dyobj_heap_allocator<float, 200> allocator2;

  ASSERT_TRUE(allocator1 != allocator2);
  ASSERT_FALSE(allocator1 == allocator2);
}