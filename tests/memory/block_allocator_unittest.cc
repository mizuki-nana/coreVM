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
#include "memory/block_allocator.h"

#include <sneaker/testing/_unittest.h>


// -----------------------------------------------------------------------------

typedef int T;

// -----------------------------------------------------------------------------

const size_t T_SIZE = sizeof(T);

// -----------------------------------------------------------------------------

const size_t N = 8;

// -----------------------------------------------------------------------------

// Intentionally making the total size to be able to hold exactly `N` instances
// of `T`, plus a few extra bytes that's not enough for another instance of `T`.
const int HEAP_STORAGE_FOR_TEST = N * T_SIZE + (T_SIZE - 1);

// -----------------------------------------------------------------------------

class block_allocator_unittest : public ::testing::Test
{
protected:
  block_allocator_unittest()
    :
    m_allocator(HEAP_STORAGE_FOR_TEST)
  {
  }

  template<typename F>
  void run_twice(F func)
  {
    for (int i = 0; i < 2; ++i)
    {
      func();
    }
  }

  corevm::memory::block_allocator<T> m_allocator;
};

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestAllocatorWithZeroTotalSize)
{
  corevm::memory::block_allocator<T> allocator(0);

  run_twice([&allocator]()
  {
    void* p = allocator.allocate();
    ASSERT_EQ(nullptr, p);

    p = allocator.allocate_n(2);
    ASSERT_EQ(nullptr, p);
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestAllocatorWithInsufficentTotalSize)
{
  corevm::memory::block_allocator<T> allocator(sizeof(uint8_t));

  run_twice([&allocator]()
  {
    void* p = allocator.allocate();
    ASSERT_EQ(nullptr, p);

    p = allocator.allocate_n(2);
    ASSERT_EQ(nullptr, p);
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestSingleDeallocationOutsideBoundary)
{
  run_twice([this]()
  {
    void* p = reinterpret_cast<void*>(m_allocator.base_addr() - 1);
    int res = m_allocator.deallocate(p);
    ASSERT_EQ(-1, res);

    p = reinterpret_cast<void*>(m_allocator.base_addr() + HEAP_STORAGE_FOR_TEST);
    res = m_allocator.deallocate(p);
    ASSERT_EQ(-1, res);
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestTotalSize)
{
  ASSERT_EQ(HEAP_STORAGE_FOR_TEST, m_allocator.total_size());
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestSingleAllocation)
{
  run_twice([this]()
  {
    void* p = m_allocator.allocate();

    ASSERT_NE(nullptr, p);

    int res = m_allocator.deallocate(p);

    ASSERT_EQ(1, res);
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestConsecutiveAllocation)
{
  run_twice([this]()
  {
    void* p[N];

    for (size_t i = 0; i < N; ++i)
    {
      p[i] = m_allocator.allocate();
      ASSERT_NE(nullptr, p[i]);
    }

    void* p2 = m_allocator.allocate();
    ASSERT_EQ(nullptr, p2);

    for (size_t i = 0; i < N; ++i)
    {
      int res = m_allocator.deallocate( p[i] );
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestBulkAllocationWithExcessiveN)
{
  run_twice([this]()
  {
    void* p = m_allocator.allocate_n(N + 1);
    ASSERT_EQ(nullptr, p);
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocations)
{
  run_twice([this]()
  {
    void* p = m_allocator.allocate_n(N);

    ASSERT_NE(nullptr, p);

    for (size_t i = 0; i < N; ++i)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );

      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocationsWithReverseDeallocations)
{
  run_twice([this]()
  {
    void* p = m_allocator.allocate_n(N);

    ASSERT_NE(nullptr, p);

    for (size_t i = 0; i < N; ++i)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[N - i - 1] );

      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocationsWithRandomsDeallocations)
{
  run_twice([this]()
  {
    size_t indices[N] = { 4, 2, 7, 0, 1, 5, 3, 6 };

    void* p = m_allocator.allocate_n(N);

    ASSERT_NE(nullptr, p);

    for (size_t i = 0; i < N; ++i)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[indices[i]] );

      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocationsInDifferentChunks)
{
  run_twice([this]()
  {
    void* p = m_allocator.allocate_n(N);
    ASSERT_NE(nullptr, p);

    const size_t cutoff = 4;

    int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[cutoff] );
    ASSERT_EQ(1, res);

    for (size_t i = 0; i < cutoff; ++i)
    {
      res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );
      ASSERT_EQ(1, res);
    }

    for (size_t i = cutoff + 1; i < N; ++i)
    {
      res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );
      ASSERT_EQ(1, res);
    }

    p = m_allocator.allocate_n(6);
    ASSERT_NE(nullptr, p);

    for (size_t i = 0; i < 6; ++i)
    {
      res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestSingleAndBulkAllocations)
{
  run_twice([this]()
  {
    const size_t cutoff = 3;
    const size_t n = N - cutoff + 1;

    void* p[cutoff];
    int res = -1;

    for (size_t i = 0; i < cutoff; ++i)
    {
      p[i] = m_allocator.allocate();
      ASSERT_NE(nullptr, p[i]);
    }

    void* p2 = m_allocator.allocate_n(n);
    ASSERT_EQ(nullptr, p2);

    for (size_t i = 0; i < cutoff; ++i)
    {
      res = m_allocator.deallocate( p[cutoff - i - 1] );
      ASSERT_EQ(1, res);
    }

    p2 = m_allocator.allocate_n(n);
    ASSERT_NE(nullptr, p);

    for (size_t i = 0; i < n; ++i)
    {
      res = m_allocator.deallocate( &reinterpret_cast<T*>(p2)[n - i - 1] );
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------
