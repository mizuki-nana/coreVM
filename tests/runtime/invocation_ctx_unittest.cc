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
#include "runtime/closure.h"
#include "runtime/closure_ctx.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/errors.h"
#include "runtime/invocation_ctx.h"
#include "dyobj/dyobj_id.h"

#include <sneaker/testing/_unittest.h>

#include <algorithm>


// -----------------------------------------------------------------------------


class invocation_ctx_unittest: public ::testing::Test
{
protected:
  corevm::runtime::closure_ctx m_ctx {
    .compartment_id = corevm::runtime::NONESET_COMPARTMENT_ID,
    .closure_id = corevm::runtime::NONESET_CLOSURE_ID
  };

  corevm::runtime::compartment* m_compartment = new corevm::runtime::compartment("");
  corevm::runtime::closure m_closure;
};


// -----------------------------------------------------------------------------

TEST_F(invocation_ctx_unittest, TestInitialization)
{
  corevm::runtime::invocation_ctx invk_ctx(m_ctx, m_compartment, &m_closure);
  ASSERT_TRUE(m_ctx == invk_ctx.closure_ctx());
  ASSERT_EQ(m_compartment, invk_ctx.compartment_ptr());
  ASSERT_EQ(&m_closure, invk_ctx.closure_ptr());
}

// -----------------------------------------------------------------------------

TEST_F(invocation_ctx_unittest, TestPutAndGetParams)
{
  corevm::runtime::invocation_ctx invk_ctx(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(false, invk_ctx.has_params());

  corevm::dyobj::dyobj_id id1 = 1000;
  corevm::dyobj::dyobj_id id2 = 200;

  invk_ctx.put_param(id1);
  invk_ctx.put_param(id2);

  ASSERT_EQ(true, invk_ctx.has_params());

  ASSERT_EQ(id1, invk_ctx.pop_param());
  ASSERT_EQ(id2, invk_ctx.pop_param());

  ASSERT_EQ(false, invk_ctx.has_params());

  ASSERT_THROW(
    {
      invk_ctx.pop_param();
    },
    corevm::runtime::missing_parameter_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(invocation_ctx_unittest, TestPutAndGetParamValuePairs)
{
  corevm::runtime::invocation_ctx invk_ctx(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(false, invk_ctx.has_param_value_pairs());

  corevm::runtime::variable_key key1 = 100;
  corevm::runtime::variable_key key2 = 2000;
  corevm::dyobj::dyobj_id id1 = 200;
  corevm::dyobj::dyobj_id id2 = 1000;

  invk_ctx.put_param_value_pair(key1, id1);
  invk_ctx.put_param_value_pair(key2, id2);

  ASSERT_EQ(true, invk_ctx.has_param_value_pairs());

  ASSERT_EQ(id1, invk_ctx.pop_param_value_pair(key1));
  ASSERT_EQ(id2, invk_ctx.pop_param_value_pair(key2));

  ASSERT_THROW(
    {
      invk_ctx.pop_param_value_pair(key1);
    },
    corevm::runtime::missing_parameter_error
  );

  ASSERT_EQ(false, invk_ctx.has_param_value_pairs());
}

// -----------------------------------------------------------------------------

TEST_F(invocation_ctx_unittest, TestListParamValuePairKeys)
{
  corevm::runtime::invocation_ctx invk_ctx(m_ctx, m_compartment, &m_closure);

  std::vector<corevm::runtime::variable_key> expected_keys {};
  std::vector<corevm::runtime::variable_key> actual_keys = invk_ctx.param_value_pair_keys();

  ASSERT_EQ(expected_keys, actual_keys);

  corevm::runtime::variable_key key1 = 100;
  corevm::runtime::variable_key key2 = 2000;
  corevm::dyobj::dyobj_id id1 = 200;
  corevm::dyobj::dyobj_id id2 = 1000;

  invk_ctx.put_param_value_pair(key1, id1);
  invk_ctx.put_param_value_pair(key2, id2);

  expected_keys = { key1, key2 };
  actual_keys = invk_ctx.param_value_pair_keys();

  std::sort(expected_keys.begin(), expected_keys.end());
  std::sort(actual_keys.begin(), actual_keys.end());

  ASSERT_EQ(expected_keys, actual_keys);
}

// -----------------------------------------------------------------------------
