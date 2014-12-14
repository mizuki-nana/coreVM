/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#include <cassert>
#include <sneaker/testing/_unittest.h>
#include "../../include/memory/allocator.h"
#include "../../include/memory/sequential_allocation_scheme.h"


const int HEAP_STORAGE_FOR_TEST = 1024;


template<typename AllocationSchemeType>
class allocator_unittest : public ::testing::Test {
protected:
  void* allocate(size_t size) noexcept {
    return m_allocator.allocate(size);
  }

  int deallocate(void* ptr) noexcept {
    return m_allocator.deallocate(ptr);
  }

  corevm::memory::allocator<
    HEAP_STORAGE_FOR_TEST, AllocationSchemeType> m_allocator;
};


typedef ::testing::Types<
  corevm::memory::first_fit_allocation_scheme,
  corevm::memory::best_fit_allocation_scheme,
  corevm::memory::worst_fit_allocation_scheme,
  corevm::memory::next_fit_allocation_scheme,
  corevm::memory::buddy_allocation_scheme
> SequentialAllocationSchemeTypes;


TYPED_TEST_CASE(allocator_unittest, SequentialAllocationSchemeTypes);


TYPED_TEST(allocator_unittest, TestFirstMallocSuccessful)
{
  void* p = this->allocate(10);
  ASSERT_NE(nullptr, p);
}

TYPED_TEST(allocator_unittest, TestMallocWithSizeZeroFails)
{
  void* p = this->allocate(0);
  ASSERT_EQ(nullptr, p);
}

TYPED_TEST(allocator_unittest, TestMallocWithExcessiveSizeFails)
{
  void* p = this->allocate(HEAP_STORAGE_FOR_TEST + 1);
  ASSERT_EQ(nullptr, p);
}

TYPED_TEST(allocator_unittest, TestFreeFailsOnInvalidPtr)
{
  int c = 5;
  void* ptr = &c;
  int res = this->deallocate(ptr);
  ASSERT_EQ(-1, res);
}

TYPED_TEST(allocator_unittest, TestSingleMallocAndFree)
{
  void* p = this->allocate(HEAP_STORAGE_FOR_TEST / 2);
  ASSERT_NE(nullptr, p);

  int res = 0;
  res = this->deallocate(p);
  ASSERT_EQ(1, res);
}

TYPED_TEST(allocator_unittest, TestMallocFreeOnFullSpaceCycleSuccessful)
{
  const int CYCLES = 3;

  for(int i = 0; i < CYCLES; ++i) {
    void* p = nullptr;
    p = this->allocate(HEAP_STORAGE_FOR_TEST);
    ASSERT_NE(nullptr, p);

    int res = this->deallocate(p);
    ASSERT_EQ(1, res);
  }
}


template<typename AllocationSchemeType>
class sequential_allocation_schemes_unittest :
  public allocator_unittest<AllocationSchemeType>
{
};


typedef ::testing::Types<
  corevm::memory::first_fit_allocation_scheme,
  corevm::memory::best_fit_allocation_scheme,
  corevm::memory::worst_fit_allocation_scheme,
  corevm::memory::next_fit_allocation_scheme
> ExtraAllocationSchemeTypes;


TYPED_TEST_CASE(sequential_allocation_schemes_unittest, ExtraAllocationSchemeTypes);


TYPED_TEST(sequential_allocation_schemes_unittest, TestDoubleMallocAndFree)
{
  size_t size1 = HEAP_STORAGE_FOR_TEST / 2;
  size_t size2 = HEAP_STORAGE_FOR_TEST - size1;

  ASSERT_EQ(HEAP_STORAGE_FOR_TEST, size1 + size2);

  void* p1 = this->allocate(size1);
  assert(p1);

  void* p2 = this->allocate(size2);
  assert(p2);

  ASSERT_NE(p1, p2);

  // Test that we can do whatever we want with our pointers.
  memset(p1, 1, size1);
  memset(p2, 2, size2);

  // Test that additional allocation is not possible.
  void* p3 = this->allocate(1);
  ASSERT_EQ(nullptr, p3);

  int res1 = this->deallocate(p1);
  ASSERT_EQ(1, res1);

  int res2 = this->deallocate(p2);
  ASSERT_EQ(1, res2);
}

TYPED_TEST(sequential_allocation_schemes_unittest, TestMallocAndFreeNTimes)
{
  const size_t N = 8;
  size_t chunk_size = HEAP_STORAGE_FOR_TEST / N;

  void* ptrs[N];

  const char* letters[N] = {
    "a", "b", "c", "d", "e", "f", "g", "h"
  };

  for(int i = 0; i < N; ++i) {
    void* p = this->allocate(chunk_size);
    assert(p);
    p = static_cast<void*>(strcpy(static_cast<char*>(p), letters[i]));
    ptrs[i] = p;
  }

  // Test that additional allocation is not possible.
  void* failed_ptr = this->allocate(1);
  ASSERT_EQ(nullptr, failed_ptr);

  for(int i = 0; i < N; ++i) {
    ASSERT_STREQ(letters[i], static_cast<char*>(ptrs[i]));
  }

  for(int i = 0; i < N; ++i) {
    int res = this->deallocate(ptrs[i]);
    ASSERT_EQ(1, res);
  }
}

TYPED_TEST(sequential_allocation_schemes_unittest, TestMallocAfterFree)
{
  size_t chunk_size_1 = HEAP_STORAGE_FOR_TEST / 3;
  size_t chunk_size_2 = chunk_size_1;
  size_t chunk_size_3 = HEAP_STORAGE_FOR_TEST - (chunk_size_1 + chunk_size_2);

  void* p1 = this->allocate(chunk_size_1);
  ASSERT_NE(nullptr, p1);

  void* p2 = this->allocate(chunk_size_2);
  ASSERT_NE(nullptr, p2);

  void* p3 = this->allocate(chunk_size_3);
  ASSERT_NE(nullptr, p3);

  size_t new_chunk_size = HEAP_STORAGE_FOR_TEST / 2;

  ASSERT_LT(new_chunk_size, (chunk_size_2 + chunk_size_3));

  void* p = nullptr;
  p = this->allocate(new_chunk_size);
  ASSERT_EQ(nullptr, p);

  int res;
  res = this->deallocate(p2);
  ASSERT_EQ(1, res);

  res = this->deallocate(p3);
  ASSERT_EQ(1, res);

  p = this->allocate(new_chunk_size);
  ASSERT_NE(nullptr, p);
}


const int BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE = 1024;


class buddy_allocation_scheme_unittest : public ::testing::Test {
protected:
  corevm::memory::allocator<
    BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE,
    corevm::memory::buddy_allocation_scheme
  > m_allocator;

  template<typename F>
  void run_twice(F func) {
    for(int i = 0; i < 2; ++i) {
      func();
    }
  }

  void validate() {
    void* ptr = m_allocator.allocate(BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE);
    ASSERT_NE(nullptr, ptr);

    int res = m_allocator.deallocate(ptr);
    ASSERT_EQ(1, res);
  }
};


TEST_F(buddy_allocation_scheme_unittest, TestAllocHalfAndHalf)
{
  this->run_twice(
    [this]() {
      void* p1 = nullptr;
      void* p2 = nullptr;

      p1 = m_allocator.allocate(BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / 2);
      ASSERT_NE(nullptr, p1);

      p2 = m_allocator.allocate(BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / 2);
      ASSERT_NE(nullptr, p2);

      int res1 = m_allocator.deallocate(p1);
      int res2 = m_allocator.deallocate(p2);

      ASSERT_EQ(1, res1);
      ASSERT_EQ(1, res2);

      this->validate();
    }
  );
}

TEST_F(buddy_allocation_scheme_unittest, TestSequentialAllocAndFree)
{
  this->run_twice(
    [this]() {
      const int N = 8;
      const size_t size = BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / N;

      void* ptrs[N] = { nullptr };

      for(int i = 0; i < N; ++i) {
        ptrs[i] = m_allocator.allocate(size);
        ASSERT_NE(nullptr, ptrs[i]);
      }

      int res;

      for(int i = N - 1; i >= 0; --i) {
        res = m_allocator.deallocate(ptrs[i]);
        ASSERT_EQ(1, res);
      }

      this->validate();
    }
  );
}

TEST_F(buddy_allocation_scheme_unittest, TestAllocAndFreeInterleaved)
{
  /* This test is based on the example on page 1 in
   * http://www.cs.fsu.edu/~engelen/courses/COP402003/p827.pdf
   **/
  this->run_twice(
    [this]() {
      const size_t sizeA = 70;
      const size_t sizeB = 35;
      const size_t sizeC = 80;
      const size_t sizeD = 60;
      void* ptrA = nullptr;
      void* ptrB = nullptr;
      void* ptrC = nullptr;
      void* ptrD = nullptr;
      int res = 0;

      ptrA = m_allocator.allocate(sizeA);
      ASSERT_NE(nullptr, ptrA);

      ptrB = m_allocator.allocate(sizeB);
      ASSERT_NE(nullptr, ptrB);

      ptrC = m_allocator.allocate(sizeC);
      ASSERT_NE(nullptr, ptrC);

      res = m_allocator.deallocate(ptrA);
      ASSERT_EQ(1, res);

      ptrD = m_allocator.allocate(sizeD);
      ASSERT_NE(nullptr, ptrD);

      res = m_allocator.deallocate(ptrB);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrD);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrC);
      ASSERT_EQ(1, res);

      this->validate();
    }
  );
}

TEST_F(buddy_allocation_scheme_unittest, TestAllocAndFreeInterleaved2)
{
  /* This test is based on the example given in
   * http://en.wikipedia.org/wiki/Buddy_memory_allocation#In_practice
   **/
  this->run_twice(
    [this]() {
      const size_t sizeA = 34;
      const size_t sizeB = 66;
      const size_t sizeC = 35;
      const size_t sizeD = 67;
      void* ptrA = nullptr;
      void* ptrB = nullptr;
      void* ptrC = nullptr;
      void* ptrD = nullptr;
      int res = 0;

      ptrA = m_allocator.allocate(sizeA);
      ASSERT_NE(nullptr, ptrA);

      ptrB = m_allocator.allocate(sizeB);
      ASSERT_NE(nullptr, ptrB);

      ptrC = m_allocator.allocate(sizeC);
      ASSERT_NE(nullptr, ptrC);

      ptrD = m_allocator.allocate(sizeD);
      ASSERT_NE(nullptr, ptrD);

      res = m_allocator.deallocate(ptrB);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrD);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrA);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrC);
      ASSERT_EQ(1, res);

      this->validate();
    }
  );
}

TEST_F(buddy_allocation_scheme_unittest, TestAllocAndFreeInterleaved3)
{
  this->run_twice(
    [this]() {
      const size_t N = 4;
      const size_t size1 = BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / N;
      const size_t size2 = BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / N / 2 + 1;
      void* ptrA = nullptr;
      void* ptrB = nullptr;
      void* ptrC = nullptr;
      void* ptrD = nullptr;
      void* ptrY = nullptr;
      void* ptrZ = nullptr;

      ptrA = m_allocator.allocate(size1);
      ASSERT_NE(nullptr, ptrA);

      ptrY = m_allocator.allocate(size2);
      ASSERT_NE(nullptr, ptrY);

      ptrB = m_allocator.allocate(size1);
      ASSERT_NE(nullptr, ptrB);

      ptrZ = m_allocator.allocate(size2);
      ASSERT_NE(nullptr, ptrZ);

      void* p = m_allocator.allocate(size1);
      ASSERT_EQ(nullptr, p);

      int res;

      res = m_allocator.deallocate(ptrY);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrZ);
      ASSERT_EQ(1, res);

      ptrC = m_allocator.allocate(size1);
      ASSERT_NE(nullptr, ptrC);

      ptrD = m_allocator.allocate(size1);
      ASSERT_NE(nullptr, ptrD);

      res = m_allocator.deallocate(ptrA);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrB);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrC);
      ASSERT_EQ(1, res);

      res = m_allocator.deallocate(ptrD);
      ASSERT_EQ(1, res);

      this->validate();
    }
  );
}
