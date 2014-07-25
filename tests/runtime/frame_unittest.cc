#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/runtime/frame.h"
#include "../../include/types/native_type_handle.h"


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

  _ASSERT_THROW(
    { frame.pop_eval_stack(); },
    corevm::runtime::evaluation_stack_empty_error
  );
}

TEST_F(frame_unittest, TestVisibleVars)
{
  corevm::runtime::frame frame;
  corevm::runtime::variable_key var_key = 1111;
  corevm::dyobj::dyobj_id obj_id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  ASSERT_EQ(false, frame.has_visible_var(var_key));

  _ASSERT_THROW(
    { frame.get_visible_var(var_key); },
    corevm::runtime::local_variable_not_found_error
  );

  frame.set_visible_var(var_key, obj_id);

  ASSERT_EQ(true, frame.has_visible_var(var_key));
  ASSERT_EQ(obj_id, frame.get_visible_var(var_key));
  ASSERT_EQ(obj_id, frame.pop_visible_var(var_key));

  _ASSERT_THROW(
    { frame.get_visible_var(var_key); },
    corevm::runtime::local_variable_not_found_error
  );
}

TEST_F(frame_unittest, TestInvisibleVars)
{
  corevm::runtime::frame frame;
  corevm::runtime::variable_key var_key = 1111;
  corevm::dyobj::dyobj_id obj_id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  ASSERT_EQ(false, frame.has_invisible_var(var_key));

  _ASSERT_THROW(
    { frame.get_invisible_var(var_key); },
    corevm::runtime::local_variable_not_found_error
  );

  frame.set_invisible_var(var_key, obj_id);

  ASSERT_EQ(true, frame.has_invisible_var(var_key));
  ASSERT_EQ(obj_id, frame.get_invisible_var(var_key));
  ASSERT_EQ(obj_id, frame.pop_invisible_var(var_key));

  _ASSERT_THROW(
    { frame.get_invisible_var(var_key); },
    corevm::runtime::local_variable_not_found_error
  );
}

TEST_F(frame_unittest, TestStateCheckOnInvalidState)
{
  corevm::runtime::frame frame;

  corevm::types::native_type_handle handle = corevm::types::string("Hello world!");
  frame.push_eval_stack(handle);

  _ASSERT_THROW(
    { frame.check_state_before_destruction(); },
    corevm::runtime::evaluation_stack_not_empty_error
  );
}
