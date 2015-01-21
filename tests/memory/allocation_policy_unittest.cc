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
#include "../../include/memory/allocation_policy.h"
#include "../../include/memory/sequential_allocation_scheme.h"

#include <sneaker/testing/_unittest.h>

#include <climits>


class allocation_policy_unit_test : public ::testing::Test {
public:
  class DummyPolicy {};
  typedef corevm::memory::first_fit_allocation_scheme AllocationScheme;
  typedef corevm::memory::best_fit_allocation_scheme OtherAllocationScheme;
  typedef corevm::memory::allocation_policy<int, AllocationScheme, 1024> _AllocPolicyType;
};


TEST_F(allocation_policy_unit_test, TestMaxSize)
{
  _AllocPolicyType allocation_policy;

  ASSERT_GT(allocation_policy.max_size(), 0);
  ASSERT_LE(allocation_policy.max_size(), ULONG_MAX);
}

TEST_F(allocation_policy_unit_test, TestAllocateAndDeallocate)
{
  _AllocPolicyType allocation_policy;
  const int N = 5;

  int *nums = allocation_policy.allocate(N);
  assert(nums);

  for (int i = 0; i < N; ++i) nums[i] = i;
  for (int i = 0; i < N; ++i) ASSERT_EQ(i, nums[i]);

  allocation_policy.deallocate(nums, N);
}

TEST_F(allocation_policy_unit_test, TestEquality)
{
  corevm::memory::allocation_policy<int, AllocationScheme, 20> allocation_policy_1;
  corevm::memory::allocation_policy<int, AllocationScheme, 20> allocation_policy_2;

  ASSERT_TRUE(allocation_policy_1 == allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 != allocation_policy_2);
}

TEST_F(allocation_policy_unit_test, TestEquality2)
{
  corevm::memory::allocation_policy<int, AllocationScheme, 20> allocation_policy_1;
  corevm::memory::allocation_policy<int, AllocationScheme, 40> allocation_policy_2;

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

TEST_F(allocation_policy_unit_test, TestEquality3)
{
  corevm::memory::allocation_policy<int, AllocationScheme, 20> allocation_policy_1;
  corevm::memory::allocation_policy<float, AllocationScheme, 20> allocation_policy_2;

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

TEST_F(allocation_policy_unit_test, TestEquality4)
{
  corevm::memory::allocation_policy<int, AllocationScheme, 20> allocation_policy_1;
  corevm::memory::allocation_policy<int, OtherAllocationScheme, 20> allocation_policy_2;

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}

TEST_F(allocation_policy_unit_test, TestEquality5)
{
  corevm::memory::allocation_policy<int, AllocationScheme, 20> allocation_policy_1;
  corevm::memory::allocation_policy<float, OtherAllocationScheme, 40> allocation_policy_2;

  ASSERT_TRUE(allocation_policy_1 != allocation_policy_2);
  ASSERT_FALSE(allocation_policy_1 == allocation_policy_2);
}
