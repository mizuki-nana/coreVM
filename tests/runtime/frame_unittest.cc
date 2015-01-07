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
#include "../../include/dyobj/dyobj_id_helper.h"
#include "../../include/runtime/frame.h"
#include "../../include/types/native_type_handle.h"

#include <sneaker/testing/_unittest.h>


class frame_unittest : public ::testing::Test {};


TEST_F(frame_unittest, TestInitialization)
{
  corevm::runtime::frame frame;
  ASSERT_EQ(-1, frame.get_return_addr());
  ASSERT_EQ(nullptr, frame.get_parent_scope_frame_ptr());
}

TEST_F(frame_unittest, TestGetAndSetReturnAddr)
{
  corevm::runtime::frame frame;
  ASSERT_EQ(-1, frame.get_return_addr());

  corevm::runtime::instr_addr expected_return_addr = 555;
  frame.set_return_addr(expected_return_addr);
  ASSERT_EQ(expected_return_addr, frame.get_return_addr());
}

TEST_F(frame_unittest, TestGetAndSetParentScopeFramePtr)
{
  corevm::runtime::frame parent_frame;
  corevm::runtime::frame frame;

  ASSERT_EQ(nullptr, frame.get_parent_scope_frame_ptr());
  frame.set_parent_scope_frame_ptr(&parent_frame);
  ASSERT_EQ(&parent_frame, frame.get_parent_scope_frame_ptr());
}

TEST_F(frame_unittest, TestPushAndPopEvalStack)
{
  corevm::runtime::frame frame;
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

TEST_F(frame_unittest, TestVisibleVars)
{
  corevm::runtime::frame frame;
  corevm::runtime::variable_key var_key = 1111;
  corevm::dyobj::dyobj_id obj_id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  ASSERT_EQ(false, frame.has_visible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_visible_var(var_key);
    },
    corevm::runtime::local_variable_not_found_error
  );

  frame.set_visible_var(var_key, obj_id);

  ASSERT_EQ(true, frame.has_visible_var(var_key));
  ASSERT_EQ(obj_id, frame.get_visible_var(var_key));
  ASSERT_EQ(obj_id, frame.pop_visible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_visible_var(var_key);
    },
    corevm::runtime::local_variable_not_found_error
  );
}

TEST_F(frame_unittest, TestInvisibleVars)
{
  corevm::runtime::frame frame;
  corevm::runtime::variable_key var_key = 1111;
  corevm::dyobj::dyobj_id obj_id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  ASSERT_EQ(false, frame.has_invisible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_invisible_var(var_key);
    },
    corevm::runtime::local_variable_not_found_error
  );

  frame.set_invisible_var(var_key, obj_id);

  ASSERT_EQ(true, frame.has_invisible_var(var_key));
  ASSERT_EQ(obj_id, frame.get_invisible_var(var_key));
  ASSERT_EQ(obj_id, frame.pop_invisible_var(var_key));

  ASSERT_THROW(
    {
      frame.get_invisible_var(var_key);
    },
    corevm::runtime::local_variable_not_found_error
  );
}

TEST_F(frame_unittest, TestPutAndGetParams)
{
  corevm::runtime::frame frame;

  ASSERT_EQ(false, frame.has_params());

  corevm::dyobj::dyobj_id id2 = 200;
  corevm::dyobj::dyobj_id id1 = 1000;

  frame.put_param(id2);
  frame.put_param(id1);

  ASSERT_EQ(true, frame.has_params());

  ASSERT_EQ(id1, frame.pop_param());
  ASSERT_EQ(id2, frame.pop_param());

  ASSERT_THROW(
    {
      frame.pop_param();
    },
    corevm::runtime::missing_parameter_error
  );

  ASSERT_EQ(false, frame.has_params());
}

TEST_F(frame_unittest, TestPutAndGetParamValuePairs)
{
  corevm::runtime::frame frame;

  ASSERT_EQ(false, frame.has_param_value_pairs());

  corevm::runtime::variable_key key1 = 100;
  corevm::runtime::variable_key key2 = 2000;
  corevm::dyobj::dyobj_id id1 = 200;
  corevm::dyobj::dyobj_id id2 = 1000;

  frame.put_param_value_pair(key1, id1);
  frame.put_param_value_pair(key2, id2);

  ASSERT_EQ(true, frame.has_param_value_pairs());

  ASSERT_EQ(id1, frame.pop_param_value_pair(key1));
  ASSERT_EQ(id2, frame.pop_param_value_pair(key2));

  ASSERT_THROW(
    {
      frame.pop_param_value_pair(key1);
    },
    corevm::runtime::missing_parameter_error
  );

  ASSERT_EQ(false, frame.has_param_value_pairs());
}

TEST_F(frame_unittest, TestListParamValuePairKeys)
{
  corevm::runtime::frame frame;

  std::list<corevm::runtime::variable_key> expected_keys = {};
  std::list<corevm::runtime::variable_key> actual_keys = frame.param_value_pair_keys();

  ASSERT_EQ(expected_keys, actual_keys);

  corevm::runtime::variable_key key1 = 100;
  corevm::runtime::variable_key key2 = 2000;
  corevm::dyobj::dyobj_id id1 = 200;
  corevm::dyobj::dyobj_id id2 = 1000;

  frame.put_param_value_pair(key1, id1);
  frame.put_param_value_pair(key2, id2);

  expected_keys = {key1, key2};
  actual_keys = frame.param_value_pair_keys();

  expected_keys.sort();
  actual_keys.sort();

  ASSERT_EQ(expected_keys, actual_keys);
}
