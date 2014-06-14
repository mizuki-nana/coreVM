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
