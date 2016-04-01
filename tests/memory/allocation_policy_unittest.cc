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
#include "corevm/macros.h"
#include "memory/allocator.h"
#include "memory/allocation_policy.h"
#include "memory/sequential_allocation_scheme.h"

#include <gtest/gtest.h>

#include <climits>


#if COREVM_463

class AllocationPolicyUnitTest : public ::testing::Test
{
public:
  class DummyPolicy {};

  typedef corevm::memory::FirstFitAllocationScheme AllocationScheme;
  typedef corevm::memory::BestFitAllocationScheme OtherAllocationScheme;

  typedef corevm::memory::allocator<AllocationScheme> CoreAllocatorType;
  typedef corevm::memory::allocator<OtherAllocationScheme> OtherCoreAllocatorType;

  typedef corevm::memory::AllocationPolicy<int, CoreAllocatorType> _AllocPolicyType;

  static const uint64_t ALLOCATION_SIZE;
};

// -----------------------------------------------------------------------------

const uint64_t AllocationPolicyUnitTest::ALLOCATION_SIZE = 1024;

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestMaxSize)
{
  _AllocPolicyType allocation_policy(ALLOCATION_SIZE);

  ASSERT_GT(allocation_policy.max_size(), 0);
  ASSERT_LE(allocation_policy.max_size(), ULONG_MAX);
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestAllocateAndDeallocate)
{
  _AllocPolicyType allocation_policy(ALLOCATION_SIZE);
  const int N = 5;

  int *nums = allocation_policy.allocate(N);
  assert(nums);

  for (int i = 0; i < N; ++i)
  {
    nums[i] = i;
  }

  for (int i = 0; i < N; ++i)
  {
    ASSERT_EQ(i, nums[i]);
  }

  allocation_policy.deallocate(nums, N);
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestAllocationOverMaxSize)
{
  _AllocPolicyType allocation_policy(ALLOCATION_SIZE);
  uint64_t max_size = allocation_policy.max_size();

  std::vector<int*> ptrs(max_size);

  for (size_t i = 0; i < max_size; ++i)
  {
    int* ptr = allocation_policy.allocate(1);
    ASSERT_NE(nullptr, ptr);
    ptrs[i] = ptr;
  }

  ASSERT_EQ(nullptr, allocation_policy.allocate(1));

  // Clean up.
  for (size_t i = 0; i < max_size; ++i)
  {
    allocation_policy.deallocate(ptrs[i], sizeof(int));
  }
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestEquality)
{
  corevm::memory::AllocationPolicy<int, CoreAllocatorType> allocation_policy_1(ALLOCATION_SIZE);
  corevm::memory::AllocationPolicy<int, CoreAllocatorType> allocation_policy_2(ALLOCATION_SIZE);

  ASSERT_TRUE(allocation_policy_1 == allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 != allocation_policy_2);
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestEquality2)
{
  corevm::memory::AllocationPolicy<int, CoreAllocatorType> allocation_policy_1(20);
  corevm::memory::AllocationPolicy<int, CoreAllocatorType> allocation_policy_2(40);

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestEquality3)
{
  corevm::memory::AllocationPolicy<int, CoreAllocatorType>  allocation_policy_1(20);
  corevm::memory::AllocationPolicy<float, CoreAllocatorType> allocation_policy_2(40);

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestEquality4)
{
  corevm::memory::AllocationPolicy<int, CoreAllocatorType> allocation_policy_1(20);
  corevm::memory::AllocationPolicy<int, OtherCoreAllocatorType> allocation_policy_2(20);

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

// -----------------------------------------------------------------------------

TEST_F(AllocationPolicyUnitTest, TestEquality5)
{
  corevm::memory::AllocationPolicy<int, CoreAllocatorType>  allocation_policy_1(20);
  corevm::memory::AllocationPolicy<float, OtherCoreAllocatorType> allocation_policy_2(40);

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

// -----------------------------------------------------------------------------

#endif /* if COREVM_463 */
