/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#include "runtime/closure.h"
#include "runtime/closure_ctx.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/errors.h"
#include "runtime/invocation_ctx.h"
#include "runtime/runtime_types.h"
#include "dyobj/common.h"

#include <gtest/gtest.h>

#include <algorithm>


// -----------------------------------------------------------------------------


class InvocationCtxUnitTest: public ::testing::Test
{
protected:
  InvocationCtxUnitTest()
    :
    m_compartment(new corevm::runtime::Compartment("")),
    m_ctx(corevm::runtime::NONESET_COMPARTMENT_ID, corevm::runtime::NONESET_CLOSURE_ID),
    m_closure()
  {
  }

  ~InvocationCtxUnitTest()
  {
    delete m_compartment;
  }

  corevm::runtime::Compartment* m_compartment;
  corevm::runtime::ClosureCtx m_ctx;
  corevm::runtime::Closure m_closure;
};


// -----------------------------------------------------------------------------

TEST_F(InvocationCtxUnitTest, TestInitialization)
{
  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);
  ASSERT_TRUE(m_ctx == invk_ctx.closure_ctx());
  ASSERT_EQ(m_compartment, invk_ctx.compartment_ptr());
  ASSERT_EQ(&m_closure, invk_ctx.closure_ptr());
}

// -----------------------------------------------------------------------------

TEST_F(InvocationCtxUnitTest, TestPutAndGetParams)
{
  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(false, invk_ctx.has_params());

  corevm::runtime::RuntimeTypes::dynamic_object_type obj1;
  corevm::runtime::RuntimeTypes::dynamic_object_type obj2;

  invk_ctx.put_param(&obj1);
  invk_ctx.put_param(&obj2);

  ASSERT_EQ(true, invk_ctx.has_params());

  ASSERT_EQ(&obj1, invk_ctx.pop_param());
  ASSERT_EQ(&obj2, invk_ctx.pop_param());

  ASSERT_EQ(false, invk_ctx.has_params());

  ASSERT_THROW(
    {
      invk_ctx.pop_param();
    },
    corevm::runtime::MissingParameterError
  );
}

// -----------------------------------------------------------------------------

TEST_F(InvocationCtxUnitTest, TestPutAndGetParamValuePairs)
{
  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(false, invk_ctx.has_param_value_pairs());

  corevm::runtime::variable_key_t key1 = 100;
  corevm::runtime::variable_key_t key2 = 2000;

  corevm::runtime::RuntimeTypes::dynamic_object_type obj1;
  corevm::runtime::RuntimeTypes::dynamic_object_type obj2;

  invk_ctx.put_param_value_pair(key1, &obj1);
  invk_ctx.put_param_value_pair(key2, &obj2);

  ASSERT_EQ(true, invk_ctx.has_param_value_pairs());

  ASSERT_EQ(&obj1, invk_ctx.pop_param_value_pair(key1));
  ASSERT_EQ(&obj2, invk_ctx.pop_param_value_pair(key2));

  ASSERT_THROW(
    {
      invk_ctx.pop_param_value_pair(key1);
    },
    corevm::runtime::MissingParameterError
  );

  ASSERT_EQ(false, invk_ctx.has_param_value_pairs());
}

// -----------------------------------------------------------------------------

TEST_F(InvocationCtxUnitTest, TestListParamValuePairKeys)
{
  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);

  std::vector<corevm::runtime::variable_key_t> expected_keys {};
  std::vector<corevm::runtime::variable_key_t> actual_keys = invk_ctx.param_value_pair_keys();

  ASSERT_EQ(expected_keys, actual_keys);

  corevm::runtime::variable_key_t key1 = 100;
  corevm::runtime::variable_key_t key2 = 2000;

  corevm::runtime::RuntimeTypes::dynamic_object_type obj1;
  corevm::runtime::RuntimeTypes::dynamic_object_type obj2;

  invk_ctx.put_param_value_pair(key1, &obj1);
  invk_ctx.put_param_value_pair(key2, &obj2);

  expected_keys = { key1, key2 };
  actual_keys = invk_ctx.param_value_pair_keys();

  std::sort(expected_keys.begin(), expected_keys.end());
  std::sort(actual_keys.begin(), actual_keys.end());

  ASSERT_EQ(expected_keys, actual_keys);
}

// -----------------------------------------------------------------------------
