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
#include <climits>
#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dyobj_heap_alloc_policy.h"


class dyobj_heap_alloc_policy_unit_test : public ::testing::Test {
public:
  class DummyPolicy {};
};


TEST_F(dyobj_heap_alloc_policy_unit_test, TestMaxSize)
{
  corevm::dyobj::dyobj_heap_alloc_policy<int> alloc_policy;

  ASSERT_GT(alloc_policy.max_size(), 0);
  ASSERT_LE(alloc_policy.max_size(), ULONG_MAX);
}

TEST_F(dyobj_heap_alloc_policy_unit_test, TestAllocateAndDeallocate)
{
  corevm::dyobj::dyobj_heap_alloc_policy<int> alloc_policy;
  const int N = 5;

  int *nums = alloc_policy.allocate(N);
  assert(nums);

  for(int i = 0; i < N; ++i) nums[i] = i;
  for(int i = 0; i < N; ++i) ASSERT_EQ(i, nums[i]);

  alloc_policy.deallocate(nums, N);
}

TEST_F(dyobj_heap_alloc_policy_unit_test, TestEquality)
{
  corevm::dyobj::dyobj_heap_alloc_policy<int> alloc_policy_1;
  corevm::dyobj::dyobj_heap_alloc_policy<double> alloc_policy_2;

  ASSERT_TRUE(alloc_policy_1 == alloc_policy_2);
  ASSERT_FALSE(alloc_policy_1 != alloc_policy_2);
}

TEST_F(dyobj_heap_alloc_policy_unit_test, TestInequality)
{
  typedef dyobj_heap_alloc_policy_unit_test::DummyPolicy _DummyPolicy;

  corevm::dyobj::dyobj_heap_alloc_policy<int> alloc_policy_1;
  _DummyPolicy alloc_policy_2;

  ASSERT_FALSE(alloc_policy_1 == alloc_policy_2);
  ASSERT_TRUE(alloc_policy_1 != alloc_policy_2);
}
