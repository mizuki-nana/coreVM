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
#include "runtime/frame.h"
#include "runtime/common.h"
#include "runtime/closure.h"
#include "runtime/compartment.h"
#include "runtime/runtime_types.h"
#include "types/native_type_handle.h"

#include <gtest/gtest.h>


class FrameUnitTest : public ::testing::Test
{
protected:
  FrameUnitTest()
    :
    m_compartment(new corevm::runtime::Compartment("")),
    m_closure_ctx(corevm::runtime::NONESET_COMPARTMENT_ID, corevm::runtime::NONESET_CLOSURE_ID),
    m_closure()
  {
  }

  ~FrameUnitTest()
  {
    delete m_compartment;
  }

  corevm::runtime::Compartment* m_compartment;
  corevm::runtime::ClosureCtx m_closure_ctx;
  corevm::runtime::Closure m_closure;
};

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestInitialization)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  ASSERT_EQ(m_compartment, frame.compartment());
  ASSERT_EQ(&m_closure, frame.closure());
  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, frame.return_addr());
  ASSERT_EQ(0, frame.exc_obj());
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestInitializationWithReturnAddr)
{
  corevm::runtime::instr_addr_t return_addr = 100;
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure, return_addr);

  ASSERT_EQ(return_addr, frame.return_addr());
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestGetAndSetReturnAddr)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, frame.return_addr());

  corevm::runtime::instr_addr_t expected_return_addr = 555;
  frame.set_return_addr(expected_return_addr);
  ASSERT_EQ(expected_return_addr, frame.return_addr());
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestPushAndPopEvalStack)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::types::NativeTypeHandle handle = corevm::types::uint8(5);

  frame.push_eval_stack(handle);
  corevm::types::NativeTypeHandle popped_handle = frame.pop_eval_stack();

  ASSERT_THROW(
    {
      frame.pop_eval_stack();
    },
    corevm::runtime::EvaluationStackEmptyError
  );
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestMovePushAndPopEvalStack)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::types::NativeTypeHandle handle = corevm::types::uint8(5);

  frame.push_eval_stack(std::move(handle));
  corevm::types::NativeTypeHandle popped_handle = frame.pop_eval_stack();

  ASSERT_THROW(
    {
      frame.pop_eval_stack();
    },
    corevm::runtime::EvaluationStackEmptyError
  );
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestVisibleVars)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::runtime::variable_key_t var_key = 1111;
  corevm::runtime::RuntimeTypes::dynamic_object_type obj;

  ASSERT_THROW(
    {
      frame.get_visible_var(var_key);
    },
    corevm::runtime::NameNotFoundError
  );

  frame.set_visible_var(var_key, &obj);

  ASSERT_EQ(&obj, frame.get_visible_var(var_key));
  ASSERT_EQ(&obj, frame.pop_visible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_visible_var(var_key);
    },
    corevm::runtime::NameNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestInvisibleVars)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::runtime::variable_key_t var_key = 1111;
  corevm::runtime::RuntimeTypes::dynamic_object_type obj;

  ASSERT_THROW(
    {
      frame.get_invisible_var(var_key);
    },
    corevm::runtime::NameNotFoundError
  );

  frame.set_invisible_var(var_key, &obj);

  ASSERT_EQ(&obj, frame.get_invisible_var(var_key));
  ASSERT_EQ(&obj, frame.pop_invisible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_invisible_var(var_key);
    },
    corevm::runtime::NameNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(FrameUnitTest, TestGetAndSetExcObj)
{
  corevm::runtime::Frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::runtime::RuntimeTypes::dynamic_object_type obj;

  ASSERT_NE(&obj, frame.exc_obj());

  frame.set_exc_obj(&obj);

  ASSERT_EQ(&obj, frame.exc_obj());

  frame.clear_exc_obj();

  ASSERT_EQ(NULL, frame.exc_obj());
}

// -----------------------------------------------------------------------------
