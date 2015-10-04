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
#include "runtime/frame.h"
#include "runtime/common.h"
#include "runtime/closure.h"
#include "runtime/compartment.h"
#include "types/native_type_handle.h"

#include <sneaker/testing/_unittest.h>


class frame_unittest : public ::testing::Test
{
protected:
  corevm::runtime::closure_ctx m_closure_ctx {
    .compartment_id = corevm::runtime::NONESET_COMPARTMENT_ID,
    .closure_id = corevm::runtime::NONESET_CLOSURE_ID
  };

  corevm::runtime::compartment *m_compartment = new corevm::runtime::compartment("");
  corevm::runtime::closure m_closure;
};

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestInitialization)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  ASSERT_EQ(m_compartment, frame.compartment_ptr());
  ASSERT_EQ(&m_closure, frame.closure_ptr());
  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, frame.return_addr());
  ASSERT_EQ(0, frame.exc_obj());
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestInitializationWithReturnAddr)
{
  corevm::runtime::instr_addr return_addr = 100;
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure, return_addr);

  ASSERT_EQ(return_addr, frame.return_addr());
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestGetAndSetReturnAddr)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, frame.return_addr());

  corevm::runtime::instr_addr expected_return_addr = 555;
  frame.set_return_addr(expected_return_addr);
  ASSERT_EQ(expected_return_addr, frame.return_addr());
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestPushAndPopEvalStack)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::types::native_type_handle handle = corevm::types::uint8(5);

  frame.push_eval_stack(handle);
  corevm::types::native_type_handle popped_handle = frame.pop_eval_stack();

  ASSERT_THROW(
    {
      frame.pop_eval_stack();
    },
    corevm::runtime::evaluation_stack_empty_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestMovePushAndPopEvalStack)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::types::native_type_handle handle = corevm::types::uint8(5);

  frame.push_eval_stack(std::move(handle));
  corevm::types::native_type_handle popped_handle = frame.pop_eval_stack();

  ASSERT_THROW(
    {
      frame.pop_eval_stack();
    },
    corevm::runtime::evaluation_stack_empty_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestVisibleVars)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::runtime::variable_key var_key = 1111;
  corevm::dyobj::dyobj_id obj_id = 1;

  ASSERT_EQ(false, frame.has_visible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_visible_var(var_key);
    },
    corevm::runtime::name_not_found_error
  );

  frame.set_visible_var(var_key, obj_id);

  ASSERT_EQ(true, frame.has_visible_var(var_key));
  ASSERT_EQ(obj_id, frame.get_visible_var(var_key));
  ASSERT_EQ(obj_id, frame.pop_visible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_visible_var(var_key);
    },
    corevm::runtime::name_not_found_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestInvisibleVars)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::runtime::variable_key var_key = 1111;
  corevm::dyobj::dyobj_id obj_id = 812;

  ASSERT_EQ(false, frame.has_invisible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_invisible_var(var_key);
    },
    corevm::runtime::name_not_found_error
  );

  frame.set_invisible_var(var_key, obj_id);

  ASSERT_EQ(true, frame.has_invisible_var(var_key));
  ASSERT_EQ(obj_id, frame.get_invisible_var(var_key));
  ASSERT_EQ(obj_id, frame.pop_invisible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_invisible_var(var_key);
    },
    corevm::runtime::name_not_found_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(frame_unittest, TestGetAndSetExcObj)
{
  corevm::runtime::frame frame(m_closure_ctx, m_compartment, &m_closure);
  corevm::dyobj::dyobj_id id = 1;

  ASSERT_NE(id, frame.exc_obj());

  frame.set_exc_obj(id);

  ASSERT_EQ(id, frame.exc_obj());

  frame.clear_exc_obj();

  ASSERT_EQ(0, frame.exc_obj());
}

// -----------------------------------------------------------------------------
