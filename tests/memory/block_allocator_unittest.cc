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

#include <gtest/gtest.h>


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
  void run_n_times(F func, const size_t n)
  {
    for (size_t i = 0; i < n; ++i)
    {
      func();
    }
  }

  template<typename F>
  void run_3_times(F func)
  {
    run_n_times(func, 3);
  }

  corevm::memory::block_allocator<T> m_allocator;
};

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestAllocatorWithZeroTotalSize)
{
  corevm::memory::block_allocator<T> allocator(0);

  run_3_times([&allocator]()
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

  run_3_times([&allocator]()
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
  run_3_times([this]()
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
  run_3_times([this]()
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
  run_3_times([this]()
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
  run_3_times([this]()
  {
    void* p = m_allocator.allocate_n(N + 1);
    ASSERT_EQ(nullptr, p);
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocations)
{
  run_3_times([this]()
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
  run_3_times([this]()
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
  run_3_times([this]()
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

TEST_F(block_allocator_unittest, TestFullAllocationsWithInterleavedDeallocations)
{
  run_3_times([this]()
  {
    void* p = m_allocator.allocate_n(N);

    ASSERT_NE(nullptr, p);

    for (size_t i = 0; i < N; i += 2)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );
      ASSERT_EQ(1, res);
    }

    for (size_t i = 1; i < N; i += 2)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocationsWithBidirectionalInterleavedDeallocations)
{
  run_3_times([this]()
  {
    void* p = m_allocator.allocate_n(N);

    ASSERT_NE(nullptr, p);

    // 1, 3, 5, 7
    for (size_t i = 1; i < N; i += 2)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[i] );
      ASSERT_EQ(1, res);
    }

    // 6, 4, 2, 0
    const size_t upper_bound = N - 1;
    for (size_t i = 0; i < N; i += 2)
    {
      int res = m_allocator.deallocate( &reinterpret_cast<T*>(p)[upper_bound - i - 1] );
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocationsWithCircularInterleavedDeallocations)
{
  /**
   * Deallocate indices with the following order:
   *
   *    0, 7, 2, 5, 4, 3, 6, 1
   */
  run_3_times([this]()
  {
    void* p = m_allocator.allocate_n(N);

    ASSERT_NE(nullptr, p);

    size_t index_1 = 0;
    size_t index_2 = 1;

    bool odd = true;
    while (index_1 < N or index_2 < N)
    {
      if (odd and index_1 < N)
      {
        m_allocator.deallocate( &reinterpret_cast<T*>(p)[index_1] );
        index_1 += 2;
      }
      else if (!odd and index_2 < N)
      {
        m_allocator.deallocate( &reinterpret_cast<T*>(p)[N - index_2] );
        index_2 += 2;
      }
      odd = !odd;
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestFullAllocationsInDifferentChunks)
{
  run_3_times([this]()
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
  run_3_times([this]()
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

TEST_F(block_allocator_unittest, TestBulkAllocations)
{
  run_3_times([this]()
  {
    const size_t M = 4;
    void* p[N / M] = { 0 };

    for (size_t i = 0; i < N / M; ++i)
    {
      p[i] = m_allocator.allocate_n(M);
      ASSERT_NE(nullptr, p[i]);
    }

    for (size_t i = 0; i < N; ++i)
    {
      void* p_ = reinterpret_cast<void*>( reinterpret_cast<uint64_t>(p[0]) + (i * sizeof(T)) );
      int res = m_allocator.deallocate( p_ );
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestBidirectionalOutwardDeallocation)
{
  run_3_times([this]()
  {
    void* p[N] = { 0 };

    for (size_t i = 0; i < N; ++i)
    {
      p[i] = m_allocator.allocate();
      ASSERT_NE(nullptr, p[i]);
    }

    size_t index_1 = N / 2 - 1;
    size_t index_2 = index_1 + 1;

    for (size_t i = 0; i < N / 2; ++i)
    {
      int res = m_allocator.deallocate(p[index_1--]);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(p[index_2++]);
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

TEST_F(block_allocator_unittest, TestBidirectionalInwardDeallocation)
{
  run_3_times([this]()
  {
    void* p[N] = { 0 };

    for (size_t i = 0; i < N; ++i)
    {
      p[i] = m_allocator.allocate();
      ASSERT_NE(nullptr, p[i]);
    }

    size_t index_1 = 0;
    size_t index_2 = N - 1;

    for (size_t i = 0; i < N / 2; ++i)
    {
      int res = m_allocator.deallocate(p[index_1++]);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(p[index_2--]);
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------

/**
 * Tests that block allocator is able to handle scenarios where there could be
 * non-adjacent free lists during deallocations.
 *
 * See:
 * [COREVM-458] Fix assertion failure in block allocator
 *
 * Steps:
 *
 * 1. Initially, all 10 slots are allocated, and in 1 list.
 *
 *  [x] [x] [x] [x] [x] [x] [x] [x] [x] [x]
 * |---------------------------------------|
 *
 * 2. Next, the 8th, 10th, and 9th slots are deallocated, in that order,
 *    leaving 2 lists, with the last one being a free list.
 *
 *  [x] [x] [x] [x] [x] [x] [x] [ ] [ ] [ ]
 * |-------------------------------|-------|
 *
 * 3. Next, the 3rd, 2nd, and 1st slots are deallocated, in that order,
 *    leaving another free list. Now there are two free lists,
 *    but they are not adjacent.
 *
 *  [ ] [ ] [ ] [x] [x] [x] [x] [ ] [ ] [ ]
 * |-----------|-------------------|-------|
 *
 * 4. Next, the 7th, 6th, 5th, and 4th slots are deallocated, in that order,
 *    leaving another free list. Now there are three free lists.
 *
 *  [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
 * |-----------|-------------------|-------|
 *
 * 5. Next, the three adjacent free lists are combined into one.
 *
 *  [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
 * |---------------------------------------|
 */
TEST_F(block_allocator_unittest, TestAllocationAndDeallocationWithNoInitialFreeLists)
{
  const size_t local_N = 10;
  typedef uint32_t local_T;
  corevm::memory::block_allocator<local_T> local_allocator(local_N * sizeof(local_T));

  run_3_times([this, &local_N, &local_allocator]()
  {
    void* p[local_N] = { 0 };

    for (size_t i = 0; i < local_N; ++i)
    {
      p[i] = local_allocator.allocate();
      ASSERT_NE(nullptr, p[i]);
    }

    for (size_t i = 7; i <= 9; ++i)
    {
      int res = local_allocator.deallocate(p[i]);
      ASSERT_EQ(1, res);
    }

    for (ssize_t i = 2; i >= 0; --i)
    {
      int res = local_allocator.deallocate(p[i]);
      ASSERT_EQ(1, res);
    }

    for (size_t i = 6; i >= 3; --i)
    {
      int res = local_allocator.deallocate(p[i]);
      ASSERT_EQ(1, res);
    }
  });
}

// -----------------------------------------------------------------------------
