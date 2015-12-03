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
#include "memory/allocator.h"
#include "memory/allocation_policy.h"
#include "memory/sequential_allocation_scheme.h"
#include "memory/errors.h"
#include "memory/object_container.h"

#include <sneaker/allocator/object_traits.h>
#include <sneaker/testing/_unittest.h>

#include <algorithm>
#include <set>
#include <sstream>


using sneaker::allocator::object_traits;


class object_container_unittest : public ::testing::Test
{
protected:
  typedef struct Dummy
  {
    int data;
  } T;

  /**
   * `std::allocator` cannot be used here for this unit test, as much as it is
   * desired for the sake of unit testing. The reason is that
   * `std::allocator::max_size()` returns the max size of an individual element,
   * where as the meaning of `max_size()` in coreVM is defined as returning the
   * maximum number of elements can be allocated by an allocator.
   */
  template<typename T, typename CoreAllocatorType>
  class Allocator : public corevm::memory::allocation_policy<T, CoreAllocatorType>, public object_traits<T>
  {
    public:
      using AllocationPolicyType = corevm::memory::allocation_policy<T, CoreAllocatorType>;

      explicit Allocator(uint64_t total_size)
        :
        AllocationPolicyType(total_size)
      {
      }
  };

  typedef Allocator<Dummy, corevm::memory::allocator<corevm::memory::first_fit_allocation_scheme>> MyAllocator;

  object_container_unittest()
    :
    m_container(1024)
  {
  }

  virtual void TearDown()
  {
    // Make sure test cases clean up the container properly.
    ASSERT_EQ(0, m_container.size());
    ASSERT_EQ(m_container.end(), m_container.begin());
  }

  corevm::memory::object_container<Dummy, MyAllocator> m_container;
};

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestCreateAndUpdate)
{
  int data = 888;

  T* p = m_container.create();
  ASSERT_NE(nullptr, p);

  p->data = data;

  T* t = m_container[p];

  ASSERT_EQ(data, t->data);

  t = m_container.at(p);

  ASSERT_EQ(data, t->data);

  m_container.destroy(p);

  ASSERT_EQ(nullptr, m_container[p]);

  ASSERT_THROW(
    {
      m_container.at(p);
    },
    corevm::memory::invalid_address_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestBulkCreateWithZeroNumber)
{
  const size_t N = 0;

  T* p = m_container.create(N);
  ASSERT_EQ(nullptr, p);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestBulkCreateAndUpdate)
{
  const size_t N = 8;

  T* p = m_container.create(N);
  ASSERT_NE(nullptr, p);

  for (size_t i = 0; i < N; ++i)
  {
    p[i].data = static_cast<int>(i);
  }

  for (size_t i = 0; i < N; ++i)
  {
    T* t = m_container[&p[i]];
    ASSERT_EQ(i, t->data);
  }

  ASSERT_EQ(N, m_container.size());

  // Destroy objects 1, 3, 5, 7
  for (size_t i = 0; (i * 2 + 1) < N; ++i)
  {
    m_container.destroy(&p[i * 2 + 1]);
  }

  ASSERT_EQ(4, m_container.size());

  // Create four times.
  for (size_t i = 0; i < 4; ++i)
  {
    T* t = m_container.create();
    ASSERT_NE(nullptr, t);
  }

  ASSERT_EQ(N, m_container.size());

  // Destroy all allocated objects.
  for (size_t i = 0; i < N; ++i)
  {
    m_container.destroy(&p[i]);

    ASSERT_EQ(nullptr, m_container[&p[i]]);
  }
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestIterator)
{
  int data1 = 666;
  int data2 = 777;
  int data3 = 888;

  T* p1 = m_container.create();
  ASSERT_NE(nullptr, p1);

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);

  T* p3 = m_container.create();
  ASSERT_NE(nullptr, p3);

  p1->data = data1;
  p2->data = data2;
  p3->data = data3;

  std::set<int> set;

  for (auto itr = m_container.begin(); itr != m_container.end(); ++itr)
  {
    T t = *itr;
    set.insert(t.data);
  }

  ASSERT_EQ(3, set.size());

  ASSERT_NE(set.end(), set.find(data1));
  ASSERT_NE(set.end(), set.find(data2));
  ASSERT_NE(set.end(), set.find(data3));

  m_container.destroy(p1);
  m_container.destroy(p2);
  m_container.destroy(p3);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestConstIterator)
{
  int data1 = 666;
  int data2 = 777;
  int data3 = 888;

  T* p1 = m_container.create();
  ASSERT_NE(nullptr, p1);

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);

  T* p3 = m_container.create();
  ASSERT_NE(nullptr, p3);

  p1->data = data1;
  p2->data = data2;
  p3->data = data3;

  std::set<int> set;

  for (auto itr = m_container.cbegin(); itr != m_container.cend(); ++itr)
  {
    T t = *itr;
    set.insert(t.data);
  }

  ASSERT_EQ(3, set.size());

  ASSERT_NE(set.end(), set.find(data1));
  ASSERT_NE(set.end(), set.find(data2));
  ASSERT_NE(set.end(), set.find(data3));

  m_container.destroy(p1);
  m_container.destroy(p2);
  m_container.destroy(p3);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestIteratorWithStdForEach)
{
  int data1 = 666;
  int data2 = 777;
  int data3 = 888;

  T* p1 = m_container.create();
  ASSERT_NE(nullptr, p1);

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);

  T* p3 = m_container.create();
  ASSERT_NE(nullptr, p3);

  p1->data = data1;
  p2->data = data2;
  p3->data = data3;

  std::set<int> set;

  std::for_each(
    m_container.begin(),
    m_container.end(),
    [&set](T& t) {
      set.insert(t.data);
    }
  );

  ASSERT_EQ(3, set.size());

  ASSERT_NE(set.end(), set.find(data1));
  ASSERT_NE(set.end(), set.find(data2));
  ASSERT_NE(set.end(), set.find(data3));

  m_container.destroy(p1);
  m_container.destroy(p2);
  m_container.destroy(p3);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestConstIteratorWithStdForEach)
{
  int data1 = 666;
  int data2 = 777;
  int data3 = 888;

  T* p1 = m_container.create();
  ASSERT_NE(nullptr, p1);

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);

  T* p3 = m_container.create();
  ASSERT_NE(nullptr, p3);

  p1->data = data1;
  p2->data = data2;
  p3->data = data3;

  std::set<int> set;

  std::for_each(
    m_container.cbegin(),
    m_container.cend(),
    [&set](const T& t) {
      set.insert(t.data);
    }
  );

  ASSERT_EQ(3, set.size());

  ASSERT_NE(set.end(), set.find(data1));
  ASSERT_NE(set.end(), set.find(data2));
  ASSERT_NE(set.end(), set.find(data3));

  m_container.destroy(p1);
  m_container.destroy(p2);
  m_container.destroy(p3);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestIteratorReflectsChange)
{
  T* p = m_container.create();
  ASSERT_NE(nullptr, p);

  int data = 1;
  p->data = data;

  auto itr = m_container.begin();

  ASSERT_NE(m_container.end(), itr);

  ASSERT_EQ(data, (*itr).data);

  int data2 = 2;
  p->data = data2;

  itr = m_container.begin();

  ASSERT_NE(m_container.end(), itr);

  ASSERT_EQ(data2, (*itr).data);

  m_container.destroy(p);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestPostIncrement)
{
  int data1 = 666;
  int data2 = 777;
  int data3 = 888;

  T* p1 = m_container.create();
  ASSERT_NE(nullptr, p1);
  p1->data = data1;

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);
  p2->data = data2;

  T* p3 = m_container.create();
  ASSERT_NE(nullptr, p3);
  p3->data = data3;

  auto itr = m_container.begin();
  auto itr1 = itr++;
  auto itr2 = itr++;
  auto itr3 = itr++;

  /* Cannot directly compare the value of the iterators here because
   * the allocator might not be linear.
   * */
  std::set<int> data_set;

  ASSERT_NE(m_container.end(), itr3);

  data_set.insert((*itr1).data);
  data_set.insert((*itr2).data);
  data_set.insert((*itr3).data);

  ASSERT_NE(data_set.end(), data_set.find(data1));
  ASSERT_NE(data_set.end(), data_set.find(data2));
  ASSERT_NE(data_set.end(), data_set.find(data3));

  m_container.destroy(p1);
  m_container.destroy(p2);
  m_container.destroy(p3);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestIteratorReflectsChange2)
{
  T* p = m_container.create();
  ASSERT_NE(nullptr, p);

  int data = 1;
  p->data = data;

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);

  auto itr = m_container.begin();
  ASSERT_NE(m_container.end(), itr);

  auto itr2 = itr;
  ASSERT_NE(m_container.end(), ++itr2);

  m_container.destroy(p2);

  ASSERT_NE(m_container.end(), itr);

  m_container.destroy(p);
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestErase)
{
  int data1 = 666;
  int data2 = 777;
  int data3 = 888;

  std::set<int> data_set = {
    data1,
    data2,
    data3,
  };

  T* p1 = m_container.create();
  ASSERT_NE(nullptr, p1);
  p1->data = data1;

  T* p2 = m_container.create();
  ASSERT_NE(nullptr, p2);
  p2->data = data2;

  T* p3 = m_container.create();
  ASSERT_NE(nullptr, p3);
  p3->data = data3;

  ASSERT_EQ(3, m_container.size());

  auto itr = m_container.begin();

  m_container.erase(itr);

  ASSERT_EQ(2, m_container.size());

  itr = m_container.begin();

  /* Cannot directly compare the value of the iterators here because
   * the allocator might not be linear.
   * */
  ASSERT_NE(data_set.end(), data_set.find((*itr).data));

  m_container.erase(++itr);

  ASSERT_EQ(1, m_container.size());

  itr = m_container.begin();

  ASSERT_NE(data_set.end(), data_set.find((*itr).data));

  m_container.erase(itr);

  ASSERT_EQ(0, m_container.size());
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestAllocationOverMaxSize)
{
  uint64_t max_size = m_container.max_size();

  std::vector<T*> ptrs(max_size);

  for (size_t i = 0; i < max_size; ++i)
  {
    T* ptr = m_container.create();
    ASSERT_NE(nullptr, ptr);
    ptrs[i] = ptr;
  }

  T* ptr = m_container.create();
  ASSERT_EQ(nullptr, ptr);

  // Clean up.
  for (size_t i = 0; i < ptrs.size(); ++i)
  {
    T* ptr_ = ptrs[i];
    m_container.destroy(ptr_);
  }
}

// -----------------------------------------------------------------------------

TEST_F(object_container_unittest, TestOutputStream)
{
  T* p1 = m_container.create();
  T* p2 = m_container.create();
  T* p3 = m_container.create();

  ASSERT_NE(nullptr, p1);
  ASSERT_NE(nullptr, p2);
  ASSERT_NE(nullptr, p3);

  std::stringstream ss;

  ss << m_container;

  ASSERT_NE(0, ss.str().size());

  m_container.destroy(p1);
  m_container.destroy(p2);
  m_container.destroy(p3);
}

// -----------------------------------------------------------------------------
