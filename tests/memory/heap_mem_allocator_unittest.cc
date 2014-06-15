#include <stdio.h>
#include <cassert>
#include <sneaker/testing/_unittest.h>
#include "../../include/memory/heap_mem_allocator.h"
#include "../../include/memory/sequential_allocation_scheme.h"


#define HEAP_STORAGE_FOR_TEST 1024 * 4


template<typename AllocationSchemeType>
class heap_mem_allocator_unit_test : public ::testing::Test {
protected:
  void* _malloc_mem(size_t size) noexcept {
    return _allocator.malloc_mem(size);
  }

  int _free_mem(void* ptr) noexcept {
    return _allocator.free_mem(ptr);
  }

  corevm::memory::heap_mem_allocator<HEAP_STORAGE_FOR_TEST, AllocationSchemeType> _allocator;
};


typedef ::testing::Types<
  corevm::memory::first_fit_allocation_scheme,
  corevm::memory::best_fit_allocation_scheme,
  corevm::memory::worst_fit_allocation_scheme,
  corevm::memory::next_fit_allocation_scheme,
  corevm::memory::buddy_allocation_scheme
> SequentialAllocationSchemeTypes;


TYPED_TEST_CASE(heap_mem_allocator_unit_test, SequentialAllocationSchemeTypes);


TYPED_TEST(heap_mem_allocator_unit_test, TestFirstMallocSuccessful)
{
  void* p = this->_malloc_mem(10);
  ASSERT_NE(nullptr, p);
}

TYPED_TEST(heap_mem_allocator_unit_test, TestMallocWithSizeZeroFails)
{
  void* p = this->_malloc_mem(0);
  ASSERT_EQ(nullptr, p);
}

TYPED_TEST(heap_mem_allocator_unit_test, TestMallocWithExcessiveSizeFails)
{
  void* p = this->_malloc_mem(HEAP_STORAGE_FOR_TEST + 1);
  ASSERT_EQ(nullptr, p);
}

TYPED_TEST(heap_mem_allocator_unit_test, TestFreeFailsOnInvalidPtr)
{
  int c = 5;
  void* ptr = &c;
  int res = this->_free_mem(ptr);
  ASSERT_EQ(-1, res);
}

TYPED_TEST(heap_mem_allocator_unit_test, TestSingleMallocAndFree)
{
  void* p = this->_malloc_mem(1024);
  ASSERT_NE(nullptr, p);

  int res = 0;
  res = this->_free_mem(p);
  ASSERT_EQ(1, res);
}

TYPED_TEST(heap_mem_allocator_unit_test, TestMallocFreeOnFullSpaceCycleSuccessful)
{
  void* p = nullptr;
  int res = 0;
  int i = 0;
  const int CYCLES = 3;

  while(i < CYCLES) {
    p = this->_malloc_mem(HEAP_STORAGE_FOR_TEST);
    ASSERT_NE(nullptr, p);

    res = this->_free_mem(p);
    ASSERT_EQ(1, res);

    ++i;
  }
}


template<typename AllocationSchemeType>
class heap_mem_allocator_true_sequential_unit_test : 
  public heap_mem_allocator_unit_test<AllocationSchemeType>
{
};


typedef ::testing::Types<
  corevm::memory::first_fit_allocation_scheme,
  corevm::memory::best_fit_allocation_scheme,
  corevm::memory::worst_fit_allocation_scheme,
  corevm::memory::next_fit_allocation_scheme
> ExtraAllocationSchemeTypes;


TYPED_TEST_CASE(heap_mem_allocator_true_sequential_unit_test, ExtraAllocationSchemeTypes);


TYPED_TEST(heap_mem_allocator_true_sequential_unit_test, TestDoubleMallocAndFree)
{
  size_t size1 = HEAP_STORAGE_FOR_TEST / 2;
  size_t size2 = HEAP_STORAGE_FOR_TEST - size1;

  ASSERT_EQ(HEAP_STORAGE_FOR_TEST, size1 + size2);

  void* p1 = this->_malloc_mem(size1);
  assert(p1);

  void* p2 = this->_malloc_mem(size2);
  assert(p2);

  ASSERT_NE(p1, p2);

  // test that we can do whatever we want with our pointers.
  memset(p1, 1, size1);
  memset(p2, 2, size2);

  // test that additional allocation is not possible.
  void* p3 = this->_malloc_mem(1);
  ASSERT_EQ(nullptr, p3);

  int res1 = this->_free_mem(p1);
  ASSERT_EQ(1, res1);

  int res2 = this->_free_mem(p2);
  ASSERT_EQ(1, res2);
}

TYPED_TEST(heap_mem_allocator_true_sequential_unit_test, TestMallocAndFreeNTimes)
{
  const size_t N = 8;
  size_t chunk_size = HEAP_STORAGE_FOR_TEST / N;

  void* ptrs[N];

  const char* letters[N] = {
    "a", "b", "c", "d", "e", "f", "g", "h"
  };

  for(int i = 0; i < N; ++i) {
    void* p = this->_malloc_mem(chunk_size);
    assert(p);
    p = static_cast<void*>(strcpy(static_cast<char*>(p), letters[i]));
    ptrs[i] = p;
  }

  // test that additional allocation is not possible.
  void* failed_ptr = this->_malloc_mem(1);
  ASSERT_EQ(nullptr, failed_ptr);

  for(int i = 0; i < N; ++i) {
    ASSERT_STREQ(letters[i], static_cast<char*>(ptrs[i]));
  }

  for(int i = 0; i < N; ++i) {
    int res = this->_free_mem(ptrs[i]);
    ASSERT_EQ(1, res);
  }
}

TYPED_TEST(heap_mem_allocator_true_sequential_unit_test, TestMallocAfterFree)
{
  size_t chunk_size_1 = HEAP_STORAGE_FOR_TEST / 3;
  size_t chunk_size_2 = chunk_size_1;
  size_t chunk_size_3 = HEAP_STORAGE_FOR_TEST - (chunk_size_1 + chunk_size_2);

  void* p1 = this->_malloc_mem(chunk_size_1);
  ASSERT_NE(nullptr, p1);

  void* p2 = this->_malloc_mem(chunk_size_2);
  ASSERT_NE(nullptr, p2);

  void* p3 = this->_malloc_mem(chunk_size_3);
  ASSERT_NE(nullptr, p3);

  size_t new_chunk_size = HEAP_STORAGE_FOR_TEST / 2;

  ASSERT_LT(new_chunk_size, (chunk_size_2 + chunk_size_3));

  void* p = nullptr;
  p = this->_malloc_mem(new_chunk_size);
  ASSERT_EQ(nullptr, p);

  int res;
  res = this->_free_mem(p2);
  ASSERT_EQ(1, res);

  res = this->_free_mem(p3);
  ASSERT_EQ(1, res);

  p = this->_malloc_mem(new_chunk_size);
  ASSERT_NE(nullptr, p);
}


#define BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE 1024

class buddy_allocation_scheme_unittest : public ::testing::Test {
protected:
  corevm::memory::heap_mem_allocator<
    BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE,
    corevm::memory::buddy_allocation_scheme
  > _allocator;

  void validate() {
    void* ptr = _allocator.malloc_mem(BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE);
    ASSERT_NE(nullptr, ptr);

    int res = _allocator.free_mem(ptr);
    ASSERT_EQ(1, res);
  }
};


TEST_F(buddy_allocation_scheme_unittest, TestAllocHalfAndHalf)
{
  void* p1 = nullptr;
  void* p2 = nullptr;

  p1 = _allocator.malloc_mem(BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / 2);
  ASSERT_NE(nullptr, p1);

  p2 = _allocator.malloc_mem(BUDDY_ALLOCATION_SCHEME_TEST_HEAP_SIZE / 2);
  ASSERT_NE(nullptr, p2);

  int res1 = _allocator.free_mem(p1);
  int res2 = _allocator.free_mem(p2);

  ASSERT_EQ(1, res1);
  ASSERT_EQ(1, res2);

  // Validation
  this->validate();
}

TEST_F(buddy_allocation_scheme_unittest, TestAllocAndFreeInterleaved)
{
  /* This test is based on the example on page 1 in
   * http://www.cs.fsu.edu/~engelen/courses/COP402003/p827.pdf
   * */
  void* ptrA = nullptr;
  void* ptrB = nullptr;
  void* ptrC = nullptr;
  void* ptrD = nullptr;
  int res = 0;

  ptrA = _allocator.malloc_mem(70);
  ASSERT_NE(nullptr, ptrA);

  ptrB = _allocator.malloc_mem(35);
  ASSERT_NE(nullptr, ptrB);

  ptrC = _allocator.malloc_mem(80);
  ASSERT_NE(nullptr, ptrC);

  res = _allocator.free_mem(ptrA);
  ASSERT_EQ(1, res);

  ptrD = _allocator.malloc_mem(60);
  ASSERT_NE(nullptr, ptrD);

  res = _allocator.free_mem(ptrB);
  ASSERT_EQ(1, res);

  res = _allocator.free_mem(ptrD);
  ASSERT_EQ(1, res);

  res = _allocator.free_mem(ptrC);
  ASSERT_EQ(1, res);

  // Validation
  this->validate();
}

TEST_F(buddy_allocation_scheme_unittest, TestAllocAndFreeInterleaved2)
{
  /* This test is based on the example given in
   * http://en.wikipedia.org/wiki/Buddy_memory_allocation#In_practice
   */
  void* ptrA = nullptr;
  void* ptrB = nullptr;
  void* ptrC = nullptr;
  void* ptrD = nullptr;
  int res = 0;

  ptrA = _allocator.malloc_mem(34);
  ASSERT_NE(nullptr, ptrA);

  ptrB = _allocator.malloc_mem(66);
  ASSERT_NE(nullptr, ptrB);

  ptrC = _allocator.malloc_mem(35);
  ASSERT_NE(nullptr, ptrC);

  ptrD = _allocator.malloc_mem(67);
  ASSERT_NE(nullptr, ptrD);

  res = _allocator.free_mem(ptrB);
  ASSERT_EQ(1, res);

  res = _allocator.free_mem(ptrD);
  ASSERT_EQ(1, res);

  res = _allocator.free_mem(ptrA);
  ASSERT_EQ(1, res);

  res = _allocator.free_mem(ptrC);
  ASSERT_EQ(1, res);

  // Validation
  this->validate();
}
