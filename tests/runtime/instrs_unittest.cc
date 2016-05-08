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
#include "corevm/macros.h"
#include "dyobj/flags.h"
#include "dyobj/util.h"
#include "runtime/closure.h"
#include "runtime/common.h"
#include "runtime/invocation_ctx.h"
#include "runtime/process.h"
#include "types/interfaces.h"
#include "types/native_type_handle.h"
#include "types/types.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <list>


// -----------------------------------------------------------------------------

using corevm::runtime::Process;

// -----------------------------------------------------------------------------

class InstrsUnitTest : public ::testing::Test
{
public:
  static const std::string DUMMY_PATH;

  InstrsUnitTest()
    :
    ::testing::Test(),
    m_compartment(new corevm::runtime::Compartment("")),
    m_ctx(corevm::runtime::ClosureCtx(corevm::runtime::NONESET_COMPARTMENT_ID, corevm::runtime::NONESET_CLOSURE_ID)),
    m_closure(),
    m_frame(new corevm::runtime::Frame(m_ctx, m_compartment, &m_closure)),
    m_invk_ctx(new corevm::runtime::InvocationCtx(m_ctx, m_compartment, &m_closure)),
    m_process()
  {
  }

  virtual void SetUp()
  {
    m_process.push_frame(*m_frame);
    m_process.push_invocation_ctx(*m_invk_ctx);
  }

  void execute_instr(corevm::runtime::InstrHandler handler,
    const corevm::runtime::Instr& instr)
  {
    corevm::runtime::Frame* frame = &m_process.top_frame();
    corevm::runtime::InvocationCtx* invk_ctx = &m_process.top_invocation_ctx();

    handler(instr, m_process, &frame, &invk_ctx);
  }

protected:
  corevm::runtime::Compartment* m_compartment;
  corevm::runtime::ClosureCtx m_ctx;
  corevm::runtime::Closure m_closure;
  corevm::runtime::Frame* m_frame;
  corevm::runtime::InvocationCtx* m_invk_ctx;
  corevm::runtime::Process m_process;
};

// -----------------------------------------------------------------------------

const std::string InstrsUnitTest::DUMMY_PATH = "./my/path/file";

// -----------------------------------------------------------------------------

class InstrsObjUnitTest : public InstrsUnitTest
{
protected:
  void execute_instr(corevm::runtime::InstrHandler handler,
    const corevm::runtime::Instr& instr, uint64_t expected_stack_size=1)
  {
    InstrsUnitTest::execute_instr(handler, instr);

    uint64_t actual_stack_size = m_process.stack_size();

    ASSERT_EQ(expected_stack_size, actual_stack_size);
  }
};

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrNEW)
{
  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_new, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrLDOBJ)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 10;
  corevm::runtime::closure_id_t parent_closure_id = 100;
  corevm::runtime::Compartment compartment(DUMMY_PATH);
  corevm::runtime::Vector vector;

  corevm::runtime::ClosureCtx ctx1(compartment_id, closure_id);
  corevm::runtime::ClosureCtx ctx2(compartment_id, parent_closure_id);

  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    closure_id,
    parent_closure_id,
    vector,
    locs,
    catch_sites);


  corevm::runtime::Closure parent_closure(
    "",
    parent_closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure,
    parent_closure
  };

  corevm::runtime::Frame frame(ctx1, m_compartment, &closure_table[0], 0);
  corevm::runtime::Frame parent_frame(ctx2, m_compartment, &closure_table[1], 0);
  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  corevm::runtime::variable_key_t key = 123;
  auto obj = m_process.create_dyobj();

  parent_frame.set_visible_var(key, obj);

  m_process.push_frame(parent_frame);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  execute_instr(corevm::runtime::instr_handler_ldobj, instr, 1);

  ASSERT_EQ(obj, m_process.top_stack());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSTOBJ)
{
  corevm::runtime::Instr instr(0, 1, 0);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  execute_instr(corevm::runtime::instr_handler_stobj, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();

  ASSERT_TRUE(actual_frame.has_visible_var(1));

  auto actual_obj = actual_frame.get_visible_var(1);

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSTOBJN)
{
  corevm::runtime::variable_key_t key = 1;
  size_t n = 1;

  corevm::runtime::Instr instr(
    0,
    static_cast<corevm::runtime::instr_oprd_t>(key),
    static_cast<corevm::runtime::instr_oprd_t>(n));

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  corevm::runtime::Frame frame2(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);
  m_process.push_frame(frame2);

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  execute_instr(corevm::runtime::instr_handler_stobjn, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_nth_frame(n);

  ASSERT_TRUE(actual_frame.has_visible_var(key));

  auto actual_obj = actual_frame.get_visible_var(key);

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrGETATTR)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::Compartment compartment(DUMMY_PATH);

  uint64_t attr_str_key = 0;
  const std::string attr_str = "Hello world";

  corevm::runtime::StringLiteralTable str_literal_table { attr_str };

  compartment.set_string_literal_table(str_literal_table);

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };

  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  corevm::runtime::ClosureCtx ctx(compartment_id, closure.id);

  m_process.emplace_frame(ctx, &compartment, &closure_table[0]);

  corevm::runtime::Instr instr(0, attr_str_key, 0);

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);
  obj1->putattr(attr_key, obj2);
  m_process.push_stack(obj1);

  execute_instr(corevm::runtime::instr_handler_getattr, instr, 1);

  auto expected_obj = obj2;
  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(expected_obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSETATTR)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::Compartment compartment(DUMMY_PATH);

  uint64_t attr_str_key = 0;
  const std::string attr_str = "Hello world";

  corevm::runtime::StringLiteralTable str_literal_table { attr_str };

  compartment.set_string_literal_table(str_literal_table);
  m_process.insert_compartment(compartment);

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };

  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  corevm::runtime::ClosureCtx ctx(compartment_id, closure.id);

  m_process.emplace_frame(ctx, &compartment, &closure_table[0]);

  corevm::runtime::Instr instr(0, attr_str_key, 0);

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  m_process.push_stack(obj1);
  m_process.push_stack(obj2);

  execute_instr(corevm::runtime::instr_handler_setattr, instr, 1);

  auto expected_obj = obj1;
  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(expected_obj, actual_obj);

  corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  ASSERT_TRUE(actual_obj->hasattr(attr_key));

  ASSERT_EQ(obj2, actual_obj->getattr(attr_key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrDELATTR)
{
  corevm::dyobj::attr_key_t attr_key = 777;
  corevm::runtime::Instr instr(0, attr_key, 0);

  auto obj = m_process.create_dyobj();
  auto attr_obj = m_process.create_dyobj();

  obj->putattr(attr_key, attr_obj);

  ASSERT_TRUE(obj->hasattr(attr_key));

  m_process.push_stack(obj);

  execute_instr(corevm::runtime::instr_handler_delattr, instr, 1);

  auto expected_obj = obj;
  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(expected_obj, actual_obj);

  ASSERT_FALSE(actual_obj->hasattr(attr_key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrHASATTR2)
{
  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  corevm::runtime::Frame& frame = m_process.top_frame();
  frame.push_eval_stack(hndl);

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  obj1->putattr(attr_key, obj2);
  m_process.push_stack(obj1);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_hasattr2, instr, 1);

  corevm::runtime::Frame& top_frame = m_process.top_frame();

  corevm::types::NativeTypeHandle res_hndl = top_frame.top_eval_stack();

  const bool actual_res = corevm::types::get_value_from_handle<bool>(res_hndl);

  ASSERT_EQ(true, actual_res);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrGETATTR2)
{
  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  corevm::runtime::Frame& frame = m_process.top_frame();
  frame.push_eval_stack(hndl);

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  obj1->putattr(attr_key, obj2);
  m_process.push_stack(obj1);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_getattr2, instr, 1);

  auto attr_obj = m_process.top_stack();

  ASSERT_EQ(obj2, attr_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSETATTR2)
{
  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  corevm::runtime::Frame& frame = m_process.top_frame();
  frame.push_eval_stack(hndl);

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  m_process.push_stack(obj1);
  m_process.push_stack(obj2);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_setattr2, instr, 1);

  auto expected_obj = obj1;
  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(expected_obj, actual_obj);

  ASSERT_EQ(true, actual_obj->hasattr(attr_key));
  ASSERT_EQ(obj2, actual_obj->getattr(attr_key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrDELATTR2)
{
  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  corevm::runtime::Frame& frame = m_process.top_frame();
  frame.push_eval_stack(hndl);

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  obj1->putattr(attr_key, obj2);
  m_process.push_stack(obj1);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_delattr2, instr, 1);

  auto expected_obj = obj1;
  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(expected_obj, actual_obj);

  ASSERT_EQ(false, actual_obj->hasattr(attr_key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrPOP)
{
  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_pop, instr, 0);

  ASSERT_THROW(
    {
      m_process.top_stack();
    },
    corevm::runtime::ObjectStackEmptyError
  );
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrLDOBJ2)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 10;
  corevm::runtime::closure_id_t parent_closure_id = 100;
  corevm::runtime::Compartment compartment(DUMMY_PATH);
  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::ClosureCtx ctx1(compartment_id, closure_id);
  corevm::runtime::ClosureCtx ctx2(compartment_id, parent_closure_id);

  corevm::runtime::Closure closure(
    "",
    closure_id,
    parent_closure_id,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure parent_closure(
    "",
    parent_closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure,
    parent_closure
  };

  corevm::runtime::Frame frame(ctx1, m_compartment, &closure_table[0], 0);
  corevm::runtime::Frame parent_frame(ctx2, m_compartment, &closure_table[1], 0);

  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  corevm::runtime::variable_key_t key = 123;
  auto obj = m_process.create_dyobj();

  parent_frame.set_invisible_var(key, obj);

  m_process.push_frame(parent_frame);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  execute_instr(corevm::runtime::instr_handler_ldobj2, instr, 1);

  ASSERT_EQ(obj, m_process.top_stack());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSTOBJ2)
{
  corevm::runtime::Instr instr(0, 1, 0);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  execute_instr(corevm::runtime::instr_handler_stobj2, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();

  ASSERT_TRUE(actual_frame.has_invisible_var(1));

  auto actual_obj = actual_frame.get_invisible_var(1);

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrDELOBJ)
{
  corevm::runtime::variable_key_t key = 1;
  auto obj = m_process.create_dyobj();

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  frame.set_visible_var(key, obj);
  m_process.push_frame(frame);

  execute_instr(corevm::runtime::instr_handler_delobj, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();

  ASSERT_FALSE(actual_frame.has_visible_var(key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrDELOBJ2)
{
  corevm::runtime::variable_key_t key = 1;
  auto obj = m_process.create_dyobj();

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  frame.set_invisible_var(key, obj);
  m_process.push_frame(frame);

  execute_instr(corevm::runtime::instr_handler_delobj2, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();

  ASSERT_FALSE(actual_frame.has_invisible_var(key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrGETHNDL)
{
  uint32_t expected_value = 123;

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(expected_value);
  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  obj->set_ntvhndl(saved_hndl);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_gethndl, instr, 1);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle actual_handle = actual_frame.pop_eval_stack();

  uint32_t actual_value = corevm::types::get_value_from_handle<uint32_t>(
    actual_handle
  );

  ASSERT_EQ(expected_value, actual_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSETHNDL)
{
  uint32_t expected_value = 123;

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(expected_value);
  frame.push_eval_stack(hndl);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_sethndl, instr, 1);

  ASSERT_TRUE(obj->has_ntvhndl());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrGETHNDL2)
{
  uint32_t expected_value = 123;

  auto obj = m_process.create_dyobj();

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  corevm::runtime::variable_key_t key = 1;
  frame.set_visible_var(key, obj);
  m_process.push_frame(frame);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(expected_value);
  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  obj->set_ntvhndl(saved_hndl);

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  execute_instr(corevm::runtime::instr_handler_gethndl2, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle actual_handle = actual_frame.pop_eval_stack();

  uint32_t actual_value = corevm::types::get_value_from_handle<uint32_t>(
    actual_handle
  );

  ASSERT_EQ(expected_value, actual_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrCLRHNDL)
{
  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(123);
  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  obj->set_ntvhndl(saved_hndl);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_clrhndl, instr, 1);

  ASSERT_FALSE(obj->has_ntvhndl());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrCPYHNDL)
{
  auto target_obj = m_process.create_dyobj();
  m_process.push_stack(target_obj);

  auto src_obj = m_process.create_dyobj();
  m_process.push_stack(src_obj);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(123);
  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  src_obj->set_ntvhndl(saved_hndl);

  corevm::runtime::Instr instr(0, 6, 0);
  execute_instr(corevm::runtime::instr_handler_cpyhndl, instr, 0);

  ASSERT_TRUE(target_obj->has_ntvhndl());

  auto& res_hndl = m_process.get_ntvhndl(&target_obj->ntvhndl());

  uint32_t res_value = corevm::types::get_value_from_handle<uint32_t>(res_hndl);

  ASSERT_EQ(123, res_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrCPYREPR)
{
  auto target_obj = m_process.create_dyobj();
  m_process.push_stack(target_obj);

  auto src_obj = m_process.create_dyobj();
  m_process.push_stack(src_obj);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(123);
  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  src_obj->set_ntvhndl(saved_hndl);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_cpyrepr, instr, 0);

  ASSERT_TRUE(target_obj->has_ntvhndl());

  auto& res_hndl = m_process.get_ntvhndl(&target_obj->ntvhndl());

  const corevm::types::native_string res_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(res_hndl);

  ASSERT_STREQ("123", res_value.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrISTRUTHY)
{
  auto src_obj = m_process.create_dyobj();
  m_process.push_stack(src_obj);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(123);
  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  src_obj->set_ntvhndl(saved_hndl);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_istruthy, instr, 1);

  auto& frame = m_process.top_frame();
  corevm::types::NativeTypeHandle& res_hndl = frame.top_eval_stack();

  const bool res_value =
    corevm::types::get_value_from_handle<bool>(res_hndl);

  ASSERT_EQ(true, res_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrOBJEQ)
{
  auto obj1 = m_process.create_dyobj();
  auto obj2 = obj1;

  m_process.push_stack(obj1);
  m_process.push_stack(obj2);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_objeq, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle result_handle = actual_frame.pop_eval_stack();

  bool expected_result = true;
  bool actual_result = corevm::types::get_value_from_handle<bool>(
    result_handle
  );

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrOBJNEQ)
{
  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  m_process.push_stack(obj1);
  m_process.push_stack(obj2);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_objneq, instr, 0);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle result_handle = actual_frame.pop_eval_stack();

  bool expected_result = true;
  bool actual_result = corevm::types::get_value_from_handle<bool>(
    result_handle
  );

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSETCXT)
{
  auto obj = m_process.create_dyobj();

  m_process.push_stack(obj);

  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::SETCTX,
    static_cast<corevm::runtime::instr_oprd_t>(m_ctx.closure_id),
    0);

  execute_instr(corevm::runtime::instr_handler_setctx, instr);

  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(obj, actual_obj);

  const corevm::runtime::ClosureCtx& ctx = actual_obj->closure_ctx();

  ASSERT_EQ(m_ctx.compartment_id, ctx.compartment_id);
  ASSERT_EQ(m_ctx.closure_id, ctx.closure_id);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrCLDOBJ)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 10;
  corevm::runtime::closure_id_t parent_closure_id = 100;
  corevm::runtime::Compartment compartment(DUMMY_PATH);
  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::ClosureCtx ctx1(compartment_id, closure_id);
  corevm::runtime::ClosureCtx ctx2(compartment_id, parent_closure_id);

  corevm::runtime::Closure closure(
    "",
    closure_id,
    parent_closure_id,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure parent_closure(
    "",
    parent_closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure,
    parent_closure
  };

  corevm::runtime::Frame frame(ctx1, m_compartment, &closure_table[0], 0);
  corevm::runtime::Frame parent_frame(ctx2, m_compartment, &closure_table[1], 0);
  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  corevm::runtime::variable_key_t key1 = 123;
  auto obj1 = m_process.create_dyobj();
  frame.set_visible_var(key1, obj1);

  corevm::runtime::variable_key_t key2 = 321;
  auto obj2 = m_process.create_dyobj();
  parent_frame.set_visible_var(key2, obj2);

  corevm::types::NativeTypeHandle hndl = corevm::types::boolean(false);
  frame.push_eval_stack(hndl);

  m_process.push_frame(parent_frame);
  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0,
    static_cast<corevm::runtime::instr_oprd_t>(key1),
    static_cast<corevm::runtime::instr_oprd_t>(key2));

  execute_instr(corevm::runtime::instr_handler_cldobj, instr, 1);

  ASSERT_EQ(obj2, m_process.top_stack());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSETATTRS)
{
  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::types::NativeTypeHandle hndl = corevm::types::native_map {
    { 0, static_cast<corevm::types::native_map_mapped_type>(obj1->id()) },
    { 1, static_cast<corevm::types::native_map_mapped_type>(obj2->id()) },
    { 2, static_cast<corevm::types::native_map_mapped_type>(obj3->id()) },
  };

  auto saved_hndl = m_process.insert_ntvhndl(hndl);

  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 10;

  corevm::runtime::Compartment compartment(DUMMY_PATH);

  const std::string attr_str1 = "__init__";
  const std::string attr_str2 = "__len__";
  const std::string attr_str3 = "__iter__";

  corevm::runtime::StringLiteralTable encoding_map {
    attr_str1,
    attr_str2,
    attr_str3,
  };

  compartment.set_string_literal_table(encoding_map);
  m_process.insert_compartment(compartment);

  corevm::dyobj::attr_key_t attr_key1 = corevm::dyobj::hash_attr_str(attr_str1);
  corevm::dyobj::attr_key_t attr_key2 = corevm::dyobj::hash_attr_str(attr_str2);
  corevm::dyobj::attr_key_t attr_key3 = corevm::dyobj::hash_attr_str(attr_str3);

  corevm::runtime::ClosureCtx ctx(compartment_id, closure_id);

  auto dst_obj = m_process.create_dyobj();
  m_process.push_stack(dst_obj);

  auto src_obj = m_process.create_dyobj();
  src_obj->set_ntvhndl(saved_hndl);
  src_obj->set_closure_ctx(ctx);

  m_process.push_stack(src_obj);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_setattrs, instr, 1);

  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(dst_obj, actual_obj);

  ASSERT_EQ(obj1, actual_obj->getattr(attr_key1));
  ASSERT_EQ(obj2, actual_obj->getattr(attr_key2));
  ASSERT_EQ(obj3, actual_obj->getattr(attr_key3));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrRSETATTRS)
{
  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::types::NativeTypeHandle hndl = corevm::types::native_map {
    { 1, static_cast<corevm::types::native_map_mapped_type>(obj1->id()) },
    { 2, static_cast<corevm::types::native_map_mapped_type>(obj2->id()) },
    { 3, static_cast<corevm::types::native_map_mapped_type>(obj3->id()) },
  };

  corevm::runtime::compartment_id_t compartment_id = 0;

  uint64_t attr_str_key = 0;
  const std::string attr_str = "Hello world";

  corevm::runtime::StringLiteralTable str_literal_table { attr_str };

  corevm::runtime::Compartment compartment(DUMMY_PATH);
  compartment.set_string_literal_table(str_literal_table);
  m_process.insert_compartment(compartment);

  corevm::runtime::ClosureCtx ctx(
    compartment_id, corevm::runtime::NONESET_CLOSURE_ID);

  corevm::runtime::Frame frame(ctx, &compartment, &m_closure);
  frame.push_eval_stack(hndl);

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  m_process.push_frame(frame);

  corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(attr_str_key), 0);

  execute_instr(corevm::runtime::instr_handler_rsetattrs, instr, 1);

  ASSERT_EQ(obj, obj1->getattr(attr_key));
  ASSERT_EQ(obj, obj2->getattr(attr_key));
  ASSERT_EQ(obj, obj3->getattr(attr_key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSETATTRS2)
{
  const std::string attr_str1 = "__init__";
  const std::string attr_str2 = "__len__";
  const std::string attr_str3 = "__iter__";

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::dyobj::attr_key_t attr_key1 = corevm::dyobj::hash_attr_str(attr_str1);
  corevm::dyobj::attr_key_t attr_key2 = corevm::dyobj::hash_attr_str(attr_str2);
  corevm::dyobj::attr_key_t attr_key3 = corevm::dyobj::hash_attr_str(attr_str3);

  auto dst_obj = m_process.create_dyobj();
  m_process.push_stack(dst_obj);

  auto src_obj = m_process.create_dyobj();
  src_obj->putattr(attr_key1, obj1);
  src_obj->putattr(attr_key2, obj2);
  src_obj->putattr(attr_key3, obj3);

  corevm::runtime::compartment_id_t compartment_id = 0;

  uint64_t attr_str_key = 0;
  const std::string attr_str = "im_self";

  auto attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::runtime::StringLiteralTable str_literal_table { attr_str };

  corevm::runtime::Compartment compartment(DUMMY_PATH);
  compartment.set_string_literal_table(str_literal_table);
  m_process.insert_compartment(compartment);

  corevm::runtime::ClosureCtx ctx(compartment_id, 0);

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  m_process.emplace_frame(ctx, &compartment, &closure);

  m_process.push_stack(dst_obj);
  m_process.push_stack(src_obj);

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(attr_str_key), 0);

  execute_instr(corevm::runtime::instr_handler_setattrs2, instr, 2);

  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(dst_obj, actual_obj);

  ASSERT_EQ(true, actual_obj->hasattr(attr_key1));
  ASSERT_EQ(true, actual_obj->hasattr(attr_key2));
  ASSERT_EQ(true, actual_obj->hasattr(attr_key3));

  auto attr_obj1 = actual_obj->getattr(attr_key1);
  auto attr_obj2 = actual_obj->getattr(attr_key1);
  auto attr_obj3 = actual_obj->getattr(attr_key1);

  ASSERT_EQ(dst_obj, attr_obj1->getattr(attr_key));
  ASSERT_EQ(dst_obj, attr_obj2->getattr(attr_key));
  ASSERT_EQ(dst_obj, attr_obj3->getattr(attr_key));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrPUTOBJ)
{
  auto obj = m_process.create_dyobj();

  m_process.push_stack(obj);

  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 0;

  corevm::runtime::ClosureCtx ctx(compartment_id, closure_id);

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };

  corevm::runtime::Compartment compartment(DUMMY_PATH);
  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  m_process.emplace_frame(ctx, &compartment, &closure_table[0]);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_putobj, instr, 1);

  corevm::runtime::Frame& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle& hndl = frame.top_eval_stack();

  corevm::dyobj::dyobj_id_t actual_id =
    corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(hndl);

  ASSERT_EQ(obj->id(), actual_id);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrGETOBJ)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 10;

  corevm::runtime::ClosureCtx ctx(compartment_id, closure_id);

  corevm::runtime::Frame frame(ctx, m_compartment, &m_closure);

  auto obj = m_process.create_dyobj();
  corevm::types::NativeTypeHandle hndl = corevm::types::uint64(obj->id());

  frame.push_eval_stack(hndl);

  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_getobj, instr, 1);

  auto actual_obj = m_process.top_stack();

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrSWAP)
{
  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();

  corevm::runtime::Instr instr(0, 0, 0);

  ASSERT_THROW(
    {
      execute_instr(corevm::runtime::instr_handler_swap, instr, 2);
    },
    corevm::runtime::InvalidOperationError
  );

  m_process.push_stack(obj2);
  m_process.push_stack(obj1);

  ASSERT_EQ(obj1, m_process.top_stack());

  execute_instr(corevm::runtime::instr_handler_swap, instr, 2);

  auto top_obj = m_process.top_stack();

  ASSERT_EQ(obj2, top_obj);
}

// -----------------------------------------------------------------------------

class InstrsObjFlagUnitTest : public InstrsObjUnitTest
{
protected:
  virtual void SetUp()
  {
    InstrsObjUnitTest::SetUp();

    auto obj = m_process.create_dyobj();
    m_process.push_stack(obj);
  }

  virtual void TearDown()
  {
    InstrsObjUnitTest::TearDown();
    m_process.pop_stack();
  }

  void execute_instr_and_assert_result(corevm::runtime::InstrHandler handler,
    const corevm::dyobj::DynamicObjectFlagBits flag)
  {
    execute_instr_with_toggle_on(handler, flag);
    execute_instr_with_toggle_off(handler, flag);
  }

private:
  void execute_instr_with_toggle_on(corevm::runtime::InstrHandler handler,
    const corevm::dyobj::DynamicObjectFlagBits flag)
  {
    corevm::runtime::Instr instr(0, 1, 0);
    _execute_and_assert_result(handler, instr, flag);
  }

  void execute_instr_with_toggle_off(corevm::runtime::InstrHandler handler,
    const corevm::dyobj::DynamicObjectFlagBits flag)
  {
    corevm::runtime::Instr instr(0, 0, 0);
    _execute_and_assert_result(handler, instr, flag);
  }

  void _execute_and_assert_result(corevm::runtime::InstrHandler handler,
    const corevm::runtime::Instr& instr,
    const corevm::dyobj::DynamicObjectFlagBits flag)
  {
    execute_instr(handler, instr);

    auto actual_obj = m_process.top_stack();

    bool on_off = static_cast<bool>(instr.oprd1);

    ASSERT_EQ(on_off, actual_obj->get_flag(flag));
  }
};

// -----------------------------------------------------------------------------

TEST_F(InstrsObjFlagUnitTest, TestInstrSETFLGC)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_setflgc,
    corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjFlagUnitTest, TestInstrSETFLDEL)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_setfldel,
    corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_INDELIBLE);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjFlagUnitTest, TestInstrSETFLCALL)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_setflcall,
    corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_NON_CALLABLE);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjFlagUnitTest, TestInstrSETFLMUTE)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_setflmute,
    corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE);
}

// -----------------------------------------------------------------------------

class InstrsFunctionsInstrsTest : public InstrsUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrPUTARG)
{
  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  corevm::runtime::InvocationCtx& actual_invk_ctx = m_process.top_invocation_ctx();

  ASSERT_THROW(
    {
      actual_invk_ctx.pop_param();
    },
    corevm::runtime::MissingParameterError
  );

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_putarg, instr);

  auto actual_obj = actual_invk_ctx.pop_param();

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrPUTKWARG)
{
  corevm::runtime::variable_key_t key = 3;
  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  corevm::runtime::InvocationCtx& actual_invk_ctx = m_process.top_invocation_ctx();

  ASSERT_THROW(
    {
      actual_invk_ctx.pop_param_value_pair(key);
    },
    corevm::runtime::MissingParameterError
  );

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 2);

  execute_instr(corevm::runtime::instr_handler_putkwarg, instr);

  auto actual_obj = actual_invk_ctx.pop_param_value_pair(key);

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrPUTARGS)
{
  auto obj = m_process.create_dyobj();

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::types::NativeTypeHandle hndl = corevm::types::native_array {
    obj1->id(),
    obj2->id(),
    obj3->id()
  };

  auto saved_hndl = m_process.insert_ntvhndl(hndl);
  obj->set_ntvhndl(saved_hndl);

  m_process.push_stack(obj);

  corevm::runtime::InvocationCtx& actual_invk_ctx = m_process.top_invocation_ctx();

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_putargs, instr);

  auto arg_obj1 = actual_invk_ctx.pop_param();
  auto arg_obj2 = actual_invk_ctx.pop_param();
  auto arg_obj3 = actual_invk_ctx.pop_param();

  ASSERT_EQ(obj1, arg_obj1);
  ASSERT_EQ(obj2, arg_obj2);
  ASSERT_EQ(obj3, arg_obj3);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrPUTKWARGS)
{
  auto obj = m_process.create_dyobj();

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::runtime::variable_key_t key1 = 1;
  corevm::runtime::variable_key_t key2 = 2;
  corevm::runtime::variable_key_t key3 = 3;

  corevm::types::NativeTypeHandle hndl = corevm::types::native_map {
    { key1, obj1->id() },
    { key2, obj2->id() },
    { key3, obj3->id() }
  };

  auto saved_hndl = m_process.insert_ntvhndl(hndl);
  obj->set_ntvhndl(saved_hndl);

  m_process.push_stack(obj);

  corevm::runtime::InvocationCtx& actual_invk_ctx = m_process.top_invocation_ctx();

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_putkwargs, instr);

  auto arg_obj1 = actual_invk_ctx.pop_param_value_pair(key1);
  ASSERT_EQ(obj1, arg_obj1);

  auto arg_obj2 = actual_invk_ctx.pop_param_value_pair(key2);
  ASSERT_EQ(obj2, arg_obj2);

  auto arg_obj3 = actual_invk_ctx.pop_param_value_pair(key3);
  ASSERT_EQ(obj3, arg_obj3);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrGETARG)
{
  corevm::runtime::StringLiteralTable str_literal_table { "self" };
  m_compartment->set_string_literal_table(std::move(str_literal_table));

  auto obj = m_process.create_dyobj();
  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);
  invk_ctx.put_param(obj);

  m_process.push_invocation_ctx(invk_ctx);

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_getarg, instr);

  auto& frame = m_process.top_frame();
  auto actual_obj = frame.get_visible_var(0);

  ASSERT_EQ(obj, actual_obj);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrGETKWARG)
{
  corevm::runtime::variable_key_t key = 22;
  auto obj = m_process.create_dyobj();

  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);
  invk_ctx.put_param_value_pair(key, obj);
  m_process.push_invocation_ctx(invk_ctx);

  corevm::runtime::Vector vector {
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
  };

  m_closure.vector = std::move(vector);

  m_process.set_pc(0);

  uint32_t relative_addr = 2;

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), relative_addr);

  execute_instr(corevm::runtime::instr_handler_getkwarg, instr);

  const corevm::runtime::Frame& frame = m_process.top_frame();

  ASSERT_EQ(true, frame.has_visible_var(key));

  auto actual_obj = frame.get_visible_var(key);

  ASSERT_EQ(obj, actual_obj);

  ASSERT_EQ(relative_addr, m_process.pc());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrGETARGS)
{
  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);
  invk_ctx.put_param(obj1);
  invk_ctx.put_param(obj2);
  invk_ctx.put_param(obj3);
  m_process.push_invocation_ctx(invk_ctx);
  m_process.emplace_frame(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(true, invk_ctx.has_params());

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_getargs, instr);

  corevm::runtime::InvocationCtx& actual_invk_ctx = m_process.top_invocation_ctx();

  ASSERT_EQ(false, actual_invk_ctx.has_params());

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle hndl = actual_frame.pop_eval_stack();

  auto result_handle3 = corevm::types::interface_array_back(hndl);
  hndl = corevm::types::interface_array_pop(hndl);

  auto result_handle2 = corevm::types::interface_array_back(hndl);
  hndl = corevm::types::interface_array_pop(hndl);

  auto result_handle1 = corevm::types::interface_array_back(hndl);
  hndl = corevm::types::interface_array_pop(hndl);

  auto actual_id1 = corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(result_handle1);
  auto actual_id2 = corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(result_handle2);
  auto actual_id3 = corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(result_handle3);

  ASSERT_EQ(obj1->id(), actual_id1);
  ASSERT_EQ(obj2->id(), actual_id2);
  ASSERT_EQ(obj3->id(), actual_id3);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrGETKWARGS)
{
  corevm::runtime::variable_key_t key1 = 1;
  corevm::runtime::variable_key_t key2 = 22;
  corevm::runtime::variable_key_t key3 = 333;

  auto obj1 = m_process.create_dyobj();
  auto obj2 = m_process.create_dyobj();
  auto obj3 = m_process.create_dyobj();

  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);
  invk_ctx.put_param_value_pair(key1, obj1);
  invk_ctx.put_param_value_pair(key2, obj2);
  invk_ctx.put_param_value_pair(key3, obj3);
  m_process.push_invocation_ctx(invk_ctx);
  m_process.emplace_frame(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(true, invk_ctx.has_param_value_pairs());

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_getkwargs, instr);

  corevm::runtime::InvocationCtx& actual_invk_ctx = m_process.top_invocation_ctx();

  ASSERT_EQ(false, actual_invk_ctx.has_param_value_pairs());

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle hndl = actual_frame.pop_eval_stack();

  corevm::types::NativeTypeHandle key_handle1 = corevm::types::uint64(key1);
  corevm::types::NativeTypeHandle key_handle2 = corevm::types::uint64(key2);
  corevm::types::NativeTypeHandle key_handle3 = corevm::types::uint64(key3);

  auto result_handle1 = corevm::types::interface_map_at(hndl, key_handle1);
  auto result_handle2 = corevm::types::interface_map_at(hndl, key_handle2);
  auto result_handle3 = corevm::types::interface_map_at(hndl, key_handle3);

  corevm::dyobj::dyobj_id_t actual_id1 = corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(result_handle1);
  corevm::dyobj::dyobj_id_t actual_id2 = corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(result_handle2);
  corevm::dyobj::dyobj_id_t actual_id3 = corevm::types::get_value_from_handle<corevm::dyobj::dyobj_id_t>(result_handle3);

  ASSERT_EQ(obj1->id(), actual_id1);
  ASSERT_EQ(obj2->id(), actual_id2);
  ASSERT_EQ(obj3->id(), actual_id3);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsFunctionsInstrsTest, TestInstrHASARGS)
{
  auto obj = m_process.create_dyobj();

  corevm::runtime::InvocationCtx invk_ctx(m_ctx, m_compartment, &m_closure);
  invk_ctx.put_param(obj);
  m_process.push_invocation_ctx(invk_ctx);
  m_process.emplace_frame(m_ctx, m_compartment, &m_closure);

  ASSERT_EQ(true, invk_ctx.has_params());

  corevm::runtime::Instr instr(0, 0, 0);
  execute_instr(corevm::runtime::instr_handler_hasargs, instr);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  corevm::types::NativeTypeHandle& hndl = actual_frame.top_eval_stack();

  const bool actual_result = corevm::types::get_value_from_handle<bool>(hndl);

  ASSERT_EQ(true, actual_result);
}

// -----------------------------------------------------------------------------

class InstrsRuntimeInstrsTest : public InstrsUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsRuntimeInstrsTest, TestInstrGC)
{
  auto obj = m_process.create_dyobj();

  ASSERT_EQ(1, m_process.heap_size());

  // Make object attached and referenced by zero objects.
  // A.k.a. ready to be garbage collected.
  obj->manager().on_setattr();
  obj->manager().dec_ref_count();

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_gc(instr, m_process, &m_frame, &m_invk_ctx);

  ASSERT_EQ(0, m_process.heap_size());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsRuntimeInstrsTest, TestInstrDEBUG)
{
  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_debug(instr, m_process, &m_frame, &m_invk_ctx);

  std::cout << "(output above expected)" << std::endl;
}

// -----------------------------------------------------------------------------

class InstrsControlInstrsTest : public InstrsUnitTest
{
protected:
  virtual void SetUp()
  {
    InstrsUnitTest::SetUp();

    corevm::runtime::Vector vector {
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
      corevm::runtime::Instr(0, 0, 0),
    };
    m_closure.vector = std::move(vector);
  }
};

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrPINVK)
{
  auto obj = m_process.create_dyobj();

  m_ctx.compartment_id = 0;
  m_ctx.closure_id = 0;
  obj->set_closure_ctx(m_ctx);

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    m_ctx.closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };

  corevm::runtime::Compartment compartment(DUMMY_PATH);
  compartment.set_closure_table(std::move(closure_table));

  m_process.insert_compartment(compartment);

  m_process.push_stack(obj);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_pinvk, instr);

  corevm::runtime::InvocationCtx& invk_ctx = m_process.top_invocation_ctx();

  corevm::runtime::ClosureCtx ctx = invk_ctx.closure_ctx();

  ASSERT_TRUE(m_ctx == ctx);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrINVK)
{
  corevm::runtime::closure_id_t closure_id = 1;
  corevm::runtime::compartment_id_t compartment_id = 0;

  m_ctx.compartment_id = compartment_id;
  m_ctx.closure_id = closure_id;

  corevm::runtime::Vector vector {
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0)
  };
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Compartment compartment(DUMMY_PATH);
  corevm::runtime::ClosureTable closure_table {
    closure
  };

  compartment.set_closure_table(std::move(closure_table));
  m_process.insert_compartment(compartment);

  m_process.emplace_frame(m_ctx, &compartment, &closure_table[0], 0);
  m_process.emplace_invocation_ctx(m_ctx, &compartment, &closure_table[0]);

  corevm::runtime::Instr instr(0, 0, 0);

  const corevm::runtime::instr_addr_t expected_return_addr = 2;
  m_process.set_pc(expected_return_addr);

  execute_instr(corevm::runtime::instr_handler_invk, instr);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();

  ASSERT_EQ(expected_return_addr, actual_frame.return_addr());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrRTRN)
{
  // TODO: [COREVM-49] Complete instruction set and implementations
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrJMP)
{
  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);
  m_process.push_frame(frame);

  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, m_process.pc());

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(8), 0);

  execute_instr(corevm::runtime::instr_handler_jmp, instr);

  ASSERT_EQ(7, m_process.pc());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrJMPIF)
{
  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);

  corevm::types::NativeTypeHandle hndl = corevm::types::boolean(true);
  frame.push_eval_stack(hndl);
  m_process.push_frame(frame);

  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, m_process.pc());

  corevm::runtime::Instr instr(0, static_cast<corevm::runtime::instr_oprd_t>(8), 0);

  execute_instr(corevm::runtime::instr_handler_jmpif, instr);

  ASSERT_EQ(7, m_process.pc());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrJMPIF_OnFalseCondition)
{
  corevm::runtime::Frame frame(m_ctx, m_compartment, &m_closure);

  corevm::types::NativeTypeHandle hndl = corevm::types::boolean(false);
  frame.push_eval_stack(hndl);
  m_process.push_frame(frame);

  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, m_process.pc());

  corevm::runtime::Instr instr(0, static_cast<corevm::runtime::instr_oprd_t>(8), 0);

  execute_instr(corevm::runtime::instr_handler_jmpif, instr);

  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, m_process.pc());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrJMPR)
{
  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, m_process.pc());

  corevm::runtime::Instr instr(0, static_cast<corevm::runtime::instr_oprd_t>(5), 0);

  execute_instr(corevm::runtime::instr_handler_jmpr, instr);

  ASSERT_EQ(5, m_process.pc());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrEXC)
{
  const corevm::runtime::closure_id_t closure_id = 0;
  const corevm::runtime::compartment_id_t compartment_id = 0;

  corevm::runtime::Vector vector {
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
  };

  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSite catch_site(0, 5, 7);

  corevm::runtime::CatchSiteList catch_sites { catch_site };

  corevm::runtime::Closure closure(
    "",
    closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureCtx ctx(closure_id, compartment_id);

  corevm::runtime::ClosureTable closure_table { closure };
  corevm::runtime::Compartment compartment(DUMMY_PATH);
  compartment.set_closure_table(std::move(closure_table));
  m_process.insert_compartment(compartment);

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  m_process.emplace_frame(ctx, &compartment, &closure_table[0], corevm::runtime::NONESET_INSTR_ADDR);
  m_process.emplace_invocation_ctx(ctx, &compartment, &closure_table[0]);

  // Emulate process starting condition.
  closure.vector = std::move(vector);
  m_process.set_pc(0);

  corevm::runtime::Instr instr(0, 1, 0);

  execute_instr(corevm::runtime::instr_handler_exc, instr);

  // Checks that the program counter is set to be one less than the destination
  // specified in the catch site.
  ASSERT_EQ(catch_site.dst - 1, m_process.pc());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsObjUnitTest, TestInstrEXCOBJ)
{
  corevm::runtime::ClosureCtx ctx(0, 0);

  corevm::runtime::Frame frame(ctx, m_compartment, &m_closure);

  ASSERT_EQ(0, frame.exc_obj());

  auto obj = m_process.create_dyobj();

  frame.set_exc_obj(obj);

  ASSERT_EQ(obj, frame.exc_obj());

  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_excobj, instr);

  ASSERT_EQ(obj, m_process.top_stack());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrCLREXC)
{
  corevm::runtime::ClosureCtx ctx(0, 0);

  corevm::runtime::Frame frame(ctx, m_compartment, &m_closure);

  ASSERT_EQ(NULL, frame.exc_obj());

  auto obj = m_process.create_dyobj();

  frame.set_exc_obj(obj);

  ASSERT_EQ(obj, frame.exc_obj());

  m_process.push_frame(frame);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_clrexc, instr);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();

  ASSERT_EQ(NULL, actual_frame.exc_obj());
}

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsTest, TestInstrJMPEXC)
{
  const corevm::runtime::closure_id_t closure_id = 0;
  const corevm::runtime::compartment_id_t compartment_id = 0;

  corevm::runtime::Vector vector {
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
  };

  corevm::runtime::ClosureCtx ctx(closure_id, compartment_id);

  auto obj = m_process.create_dyobj();

  corevm::runtime::Frame frame(ctx, m_compartment, &m_closure);
  frame.set_exc_obj(obj);
  m_process.push_frame(frame);

  // Emulate process starting condition.
  m_closure.vector = std::move(vector);
  m_process.set_pc(0);

  corevm::runtime::Instr instr(0, 6, 1);

  execute_instr(corevm::runtime::instr_handler_jmpexc, instr);

  ASSERT_EQ(6, m_process.pc());
}

// -----------------------------------------------------------------------------

class InstrsControlInstrsWithSignalTest : public InstrsControlInstrsTest
{
public:
  static bool signal_fired()
  {
    return m_signal_fired;
  }

  static void set_signal_fired()
  {
    m_signal_fired = true;
  }

protected:
  virtual void TearDown()
  {
    InstrsControlInstrsWithSignalTest::m_signal_fired = false;
  }

private:
  static bool m_signal_fired;
};

// -----------------------------------------------------------------------------

bool InstrsControlInstrsWithSignalTest::m_signal_fired = false;

// -----------------------------------------------------------------------------

TEST_F(InstrsControlInstrsWithSignalTest, TestInstrEXIT)
{
  auto sig_handler = [](int /* signum */) {
    InstrsControlInstrsWithSignalTest::set_signal_fired();
    signal(SIGTERM, SIG_IGN);
  };

  signal(SIGTERM, sig_handler);

  ASSERT_EQ(false, InstrsControlInstrsWithSignalTest::signal_fired());

  corevm::runtime::Instr instr(0, EXIT_SUCCESS, 0);

  corevm::runtime::instr_handler_exit(instr, m_process, &m_frame, &m_invk_ctx);

  ASSERT_EQ(true, InstrsControlInstrsWithSignalTest::signal_fired());
}

// -----------------------------------------------------------------------------

class InstrsEvalStackInstrsTest : public InstrsUnitTest
{
public:
  typedef uint32_t IntrinsicType;

  typedef corevm::types::uint32 NativeType;

  typedef std::list<corevm::types::NativeTypeHandle> eval_oprds_list;

protected:
  void push_eval_stack(const eval_oprds_list& elements)
  {
    corevm::runtime::Frame& frame = m_process.top_frame();

    std::for_each(
      elements.begin(),
      elements.end(),
      [&frame](corevm::types::NativeTypeHandle oprd) {
        frame.push_eval_stack(oprd);
      }
    );
  }

  template<typename IntrinsicType=uint32_t>
  void execute_instr_and_assert_result(corevm::runtime::InstrHandler handler,
    corevm::runtime::Instr instr, IntrinsicType expected_result)
  {
    execute_instr(handler, instr);

    corevm::runtime::Frame& frame = m_process.top_frame();

    ASSERT_GT(frame.eval_stack_size(), 0);

    corevm::types::NativeTypeHandle result_handle = frame.pop_eval_stack();

    IntrinsicType actual_result = corevm::types::get_value_from_handle<IntrinsicType>(
      result_handle
    );

    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename IntrinsicType=uint32_t>
  void execute_instr_and_assert_result(corevm::runtime::InstrHandler handler,
    IntrinsicType expected_result)
  {
    corevm::runtime::Instr instr(0, 0, 0);
    execute_instr_and_assert_result<IntrinsicType>(handler, instr, expected_result);
  }

  template<typename IntrinsicType=uint32_t>
  void execute_instr_and_assert_result_equals_operand(
    corevm::runtime::InstrHandler handler,
    const corevm::runtime::Instr& instr)
  {
    execute_instr(handler, instr);

    corevm::runtime::Frame& frame = m_process.top_frame();

    ASSERT_GT(frame.eval_stack_size(), 0);

    corevm::types::NativeTypeHandle result_handle = frame.pop_eval_stack();

    IntrinsicType expected_result = static_cast<IntrinsicType>(instr.oprd1);

    IntrinsicType actual_result = corevm::types::get_value_from_handle<IntrinsicType>(
      result_handle
    );

    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename IntrinsicType>
  void execute_native_floating_type_creation_instr_and_assert_result(
    corevm::runtime::InstrHandler handler, const corevm::runtime::Instr& instr,
    IntrinsicType expected_result)
  {
    execute_instr(handler, instr);

    corevm::runtime::Frame& frame = m_process.top_frame();

    ASSERT_GT(frame.eval_stack_size(), 0);

    corevm::types::NativeTypeHandle result_handle = frame.pop_eval_stack();

    IntrinsicType actual_result = corevm::types::get_value_from_handle<IntrinsicType>(
      result_handle
    );

    ASSERT_DOUBLE_EQ(expected_result, actual_result);
  }
};

// -----------------------------------------------------------------------------

class InstrsArithmeticInstrsTest : public InstrsEvalStackInstrsTest {};

// -----------------------------------------------------------------------------

class InstrsUnaryArithmeticInstrsTest : public InstrsArithmeticInstrsTest
{
public:
  InstrsUnaryArithmeticInstrsTest()
    :
    InstrsArithmeticInstrsTest(),
    m_oprd(5)
  {
  }

  virtual void SetUp()
  {
    InstrsUnitTest::SetUp();

    corevm::types::NativeTypeHandle hndl = NativeType(m_oprd);
    push_eval_stack(eval_oprds_list{hndl});
  }

protected:
  IntrinsicType m_oprd;
};

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrPOS)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_pos, +m_oprd);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrNEG)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_neg, -m_oprd);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrINC)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_inc, ++m_oprd);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrDEC)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_dec, --m_oprd);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrBNOT)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_bnot, ~m_oprd);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrLNOT)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_lnot, !m_oprd);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrABS)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_abs,
    std::abs(static_cast<long int>(m_oprd)));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsUnaryArithmeticInstrsTest, TestInstrSQRT)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_sqrt,
    static_cast<uint32_t>(std::sqrt(m_oprd)));
}

// -----------------------------------------------------------------------------

class InstrsBinaryArithmeticInstrsTest : public InstrsArithmeticInstrsTest
{
public:
  InstrsBinaryArithmeticInstrsTest()
    :
    InstrsArithmeticInstrsTest(),
    m_oprd1(10),
    m_oprd2(5)
  {
  }

  virtual void SetUp()
  {
    InstrsUnitTest::SetUp();

    corevm::types::NativeTypeHandle hndl1 = NativeType(m_oprd1);
    corevm::types::NativeTypeHandle hndl2 = NativeType(m_oprd2);
    push_eval_stack({hndl2, hndl1});
  }

protected:
  IntrinsicType m_oprd1;
  IntrinsicType m_oprd2;
};

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrADD)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_add,
    m_oprd1 + m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrSUB)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_sub,
    m_oprd1 - m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrMUL)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_mul,
    m_oprd1 * m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrDIV)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_div,
    m_oprd1 / m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrMOD)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_mod,
    m_oprd1 % m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrPOW)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_pow,
    pow(m_oprd1, m_oprd2));
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrBAND)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_band,
    m_oprd1 & m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrBOR)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_bor,
    m_oprd1 | m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrBXOR)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_bxor,
    m_oprd1 ^ m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrBLS)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_bls,
    m_oprd1 << m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrBRS)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_brs,
    m_oprd1 >> m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrEQ)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_eq,
    m_oprd1 == m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrNEQ)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_neq,
    m_oprd1 != m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrGT)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_gt,
    m_oprd1 > m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrLT)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_lt,
    m_oprd1 < m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrGTE)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_gte,
    m_oprd1 >= m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrLTE)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_lte,
    m_oprd1 <= m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrLAND)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_land,
    m_oprd1 && m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrLOR)
{
  execute_instr_and_assert_result(corevm::runtime::instr_handler_lor,
    m_oprd1 || m_oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsBinaryArithmeticInstrsTest, TestInstrCMP)
{
  int32_t expected_result = 0;

  if (m_oprd1 < m_oprd2)
  {
    expected_result = -1;
  }
  else if (m_oprd1 == m_oprd2)
  {
    expected_result = 0;
  }
  else
  {
    expected_result = 1;
  }

  execute_instr_and_assert_result<int32_t>(corevm::runtime::instr_handler_cmp,
    expected_result);
}

// -----------------------------------------------------------------------------

class InstrsNativeTypesInstrsTest : public InstrsEvalStackInstrsTest {};

// -----------------------------------------------------------------------------

class InstrsNativeIntegerTypeCreationInstrsTest : public InstrsNativeTypesInstrsTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrINT8)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::INT8, std::numeric_limits<int8_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<int8_t>(
    corevm::runtime::instr_handler_int8, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrUINT8)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::UINT8, std::numeric_limits<uint8_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<uint8_t>(
    corevm::runtime::instr_handler_uint8, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrINT16)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::INT16, std::numeric_limits<int16_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<int16_t>(
    corevm::runtime::instr_handler_int16, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrUINT16)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::UINT16, std::numeric_limits<uint16_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<uint16_t>(
    corevm::runtime::instr_handler_uint16, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrINT32)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::INT32, std::numeric_limits<int32_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<int32_t>(
    corevm::runtime::instr_handler_int32, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrUINT32)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::UINT32, std::numeric_limits<uint32_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<uint32_t>(
    corevm::runtime::instr_handler_uint32, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrINT64)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::INT64, std::numeric_limits<int64_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<int64_t>(
    corevm::runtime::instr_handler_int64, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrUINT64)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::UINT64, std::numeric_limits<uint64_t>::max(), 0);

  execute_instr_and_assert_result_equals_operand<uint64_t>(
    corevm::runtime::instr_handler_uint64, instr);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeIntegerTypeCreationInstrsTest, TestInstrBOOL)
{
  corevm::runtime::Instr instr(
    corevm::runtime::InstrEnum::BOOL, 1, 0);

  execute_instr_and_assert_result_equals_operand<bool>(
    corevm::runtime::instr_handler_bool, instr);
}

// -----------------------------------------------------------------------------

class InstrsNativeTypeCreationInstrsTest : public InstrsNativeTypesInstrsTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeCreationInstrsTest, TestInstrDEC1)
{
  const float fpt_literal = 12345.98765f;
  corevm::runtime::FptLiteralTable fpt_literal_table { fpt_literal };
  m_compartment->set_fpt_literal_table(fpt_literal_table);

  corevm::runtime::Instr instr(corevm::runtime::InstrEnum::DEC1, 0, 0);

  execute_native_floating_type_creation_instr_and_assert_result<float>(
    corevm::runtime::instr_handler_dec1, instr, fpt_literal);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeCreationInstrsTest, TestInstrDEC2)
{
  const double fpt_literal = 1234567890.0123456789;
  corevm::runtime::FptLiteralTable fpt_literal_table { fpt_literal };
  m_compartment->set_fpt_literal_table(fpt_literal_table);

  corevm::runtime::Instr instr(corevm::runtime::InstrEnum::DEC2, 0, 0);

  execute_native_floating_type_creation_instr_and_assert_result<double>(
    corevm::runtime::instr_handler_dec2, instr, fpt_literal);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeCreationInstrsTest, TestInstrSTR)
{
  corevm::types::native_string expected_result;
  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_str, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeCreationInstrsTest, TestInstrARY)
{
  corevm::types::native_array expected_result;
  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_ary, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeCreationInstrsTest, TestInstrMAP)
{
  corevm::types::native_map expected_result;
  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_map, expected_result);
}

// -----------------------------------------------------------------------------

class InstrsNativeTypeConversionInstrsTest : public InstrsNativeTypesInstrsTest
{
public:
  InstrsNativeTypeConversionInstrsTest()
    :
    InstrsNativeTypesInstrsTest(),
    m_oprd(std::numeric_limits<corevm::types::uint8>::max())
  {
  }

  virtual void SetUp()
  {
    InstrsUnitTest::SetUp();

    corevm::types::NativeTypeHandle hndl = corevm::types::uint8(m_oprd);
    push_eval_stack(eval_oprds_list{hndl});
  }

  template<typename TargetNativeType>
  void execute_instr_and_assert_result(corevm::runtime::InstrHandler handler)
  {
    corevm::runtime::Instr instr(0, 0, 0);

    execute_instr(handler, instr);

    corevm::runtime::Frame& frame = m_process.top_frame();

    ASSERT_GT(frame.eval_stack_size(), 0);

    corevm::types::NativeTypeHandle hndl = m_oprd;

    auto expected_value =
      corevm::types::get_value_from_handle<TargetNativeType>(hndl);

    corevm::types::NativeTypeHandle result_handle = frame.pop_eval_stack();

    auto actual_value =
      corevm::types::get_value_from_handle<TargetNativeType>(result_handle);

    ASSERT_EQ(expected_value, actual_value);
  }

private:
  corevm::types::uint8 m_oprd;
};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2INT8)
{
  execute_instr_and_assert_result<corevm::types::int8>(
    corevm::runtime::instr_handler_2int8);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2INT16)
{
  execute_instr_and_assert_result<corevm::types::int16>(
    corevm::runtime::instr_handler_2int16);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2UINT16)
{
  execute_instr_and_assert_result<corevm::types::uint16>(
    corevm::runtime::instr_handler_2uint16);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2INT32)
{
  execute_instr_and_assert_result<corevm::types::int32>(
    corevm::runtime::instr_handler_2int32);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2UINT64)
{
  execute_instr_and_assert_result<corevm::types::uint64>(
    corevm::runtime::instr_handler_2uint64);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2INT64)
{
  execute_instr_and_assert_result<corevm::types::int64>(
    corevm::runtime::instr_handler_2int64);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2BOOL)
{
  execute_instr_and_assert_result<corevm::types::boolean>(
    corevm::runtime::instr_handler_2bool);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2DEC1)
{
  execute_instr_and_assert_result<corevm::types::decimal>(
    corevm::runtime::instr_handler_2dec1);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2DEC2)
{
  execute_instr_and_assert_result<corevm::types::decimal2>(
    corevm::runtime::instr_handler_2dec2);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2STR)
{
  /* Lambda defined to avoid compilation error on the `ASSERT_THROW` macro.
   * (Somehow calling a templated function wrapped in a code block makes
   * `ASSERT_THROW` thinks it's getting an initializer list)
   **/
  auto test = [&]() {
    execute_instr_and_assert_result<corevm::types::string>(
      corevm::runtime::instr_handler_2str);
  };

  ASSERT_THROW(
    {
      test();
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2ARY)
{
  /* Lambda defined to avoid compilation error on the `ASSERT_THROW` macro.
   * (Somehow calling a templated function wrapped in a code block makes
   * `ASSERT_THROW` thinks it's getting an initializer list)
   **/
  auto test = [&]() {
    execute_instr_and_assert_result<corevm::types::array>(
      corevm::runtime::instr_handler_2ary);
  };

  ASSERT_THROW(
    {
      test();
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeConversionInstrsTest, TestInstr2MAP)
{
  /* Lambda defined to avoid compilation error on the `ASSERT_THROW` macro.
   * (Somehow calling a templated function wrapped in a code block makes
   * `ASSERT_THROW` thinks it's getting an initializer list)
   **/
  auto test = [&]() {
    execute_instr_and_assert_result<corevm::types::map>(
      corevm::runtime::instr_handler_2map);
  };

  ASSERT_THROW(
    {
      test();
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

class InstrsNativeTypeManipulationInstrsTest : public InstrsNativeTypesInstrsTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREPR_01)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(5);

  frame.push_eval_stack(hndl);

  corevm::types::native_string expected_value("5");

  execute_instr_and_assert_result<corevm::types::string>(
    corevm::runtime::instr_handler_repr, expected_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREPR_02)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::decimal(3.141592);

  frame.push_eval_stack(hndl);

  corevm::types::native_string expected_value("3.141592");

  execute_instr_and_assert_result<corevm::types::string>(
    corevm::runtime::instr_handler_repr, expected_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREPR_03)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::boolean(false);

  frame.push_eval_stack(hndl);

  corevm::types::native_string expected_value("0");

  execute_instr_and_assert_result<corevm::types::string>(
    corevm::runtime::instr_handler_repr, expected_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREPR_04)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::string("Hello world!");

  frame.push_eval_stack(hndl);

  corevm::types::native_string expected_value("Hello world!");

  execute_instr_and_assert_result<corevm::types::string>(
    corevm::runtime::instr_handler_repr, expected_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREPR_05)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::array({1, 2, 3});

  frame.push_eval_stack(hndl);

  corevm::types::native_string expected_value("<array>");

  execute_instr_and_assert_result<corevm::types::string>(
    corevm::runtime::instr_handler_repr, expected_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREPR_06)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::map({ { 1 , 2 } });

  frame.push_eval_stack(hndl);

  corevm::types::native_string expected_value("<map>");

  execute_instr_and_assert_result<corevm::types::string>(
    corevm::runtime::instr_handler_repr, expected_value);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrHASH)
{
  auto& frame = m_process.top_frame();

  corevm::types::NativeTypeHandle hndl = corevm::types::int32(123);

  frame.push_eval_stack(hndl);

  corevm::runtime::Instr instr(0, 0, 0);

  execute_instr(corevm::runtime::instr_handler_hash, instr);

  corevm::runtime::Frame& actual_frame = m_process.top_frame();
  ASSERT_EQ(2, actual_frame.eval_stack_size());

  corevm::types::NativeTypeHandle result_handle = actual_frame.pop_eval_stack();

  int64_t actual_result = corevm::types::get_value_from_handle<int64_t>(
    result_handle
  );

  ASSERT_GT(actual_result, 0);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrSLICE)
{
  corevm::types::NativeTypeHandle hndl1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle hndl2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle hndl3 = corevm::types::uint32(4);

  push_eval_stack({ hndl3, hndl2, hndl1 });

  corevm::types::array expected_result { 2, 3, 4 };
  execute_instr_and_assert_result<corevm::types::array>(
    corevm::runtime::instr_handler_slice, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrSTRIDE)
{
  corevm::types::NativeTypeHandle hndl1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle hndl2 = corevm::types::uint32(2);

  push_eval_stack({ hndl2, hndl1 });

  corevm::types::array expected_result { 1, 3, 5 };
  execute_instr_and_assert_result<corevm::types::array>(
    corevm::runtime::instr_handler_stride, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrREVERSE)
{
  corevm::types::NativeTypeHandle hndl = corevm::types::array({1, 2, 3, 4, 5});

  push_eval_stack({ hndl });

  corevm::types::array expected_result { 5, 4, 3, 2, 1 };
  execute_instr_and_assert_result<corevm::types::array>(
    corevm::runtime::instr_handler_reverse, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeTypeManipulationInstrsTest, TestInstrROUND)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::decimal2(3.1415));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(2));

  push_eval_stack({ oprd2, oprd });

  corevm::types::decimal2 expected_result(3.14);
  execute_instr_and_assert_result<corevm::types::decimal2>(
    corevm::runtime::instr_handler_round, expected_result);
}

// -----------------------------------------------------------------------------

class InstrsNativeTypeComplexInstrsTest : public InstrsNativeTypesInstrsTest {};

// -----------------------------------------------------------------------------

class InstrsNativeStringTypeComplexInstrsTest : public InstrsNativeTypeComplexInstrsTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRLEN)
{
  corevm::types::native_string hello_world = "Hello world!";
  corevm::types::native_string::size_type expected_result = hello_world.size();
  corevm::types::NativeTypeHandle oprd = corevm::types::string(hello_world);

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_string::size_type>(
    corevm::runtime::instr_handler_strlen, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRAT)
{
  corevm::types::native_string hello_world = "Hello world!";
  corevm::types::native_string expected_result("w");
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string(hello_world);
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(6);

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strat, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRCLR)
{
  corevm::types::native_string hello_world = "Hello world!";
  corevm::types::native_string expected_result = "";
  corevm::types::NativeTypeHandle oprd = hello_world;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strclr, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRAPD)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string exclamation = "!";
  corevm::types::native_string expected_result = "Hello world!";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = exclamation;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strapd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRPSH)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 exclamation = static_cast<corevm::types::int8>('!');
  corevm::types::native_string expected_result = "Hello world!";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = exclamation;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strpsh, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRIST)
{
  corevm::types::native_string hello_world = "Helloworld";
  corevm::types::int8 pos = static_cast<corevm::types::int8>(5);
  corevm::types::native_string space = " ";
  corevm::types::native_string expected_result = "Hello world";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;
  corevm::types::NativeTypeHandle oprd3 = space;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strist, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRIST2)
{
  corevm::types::native_string hello_world = "Helloworld";
  corevm::types::int8 pos = static_cast<corevm::types::int8>(5);
  corevm::types::int8 space = static_cast<corevm::types::int8>(' ');
  corevm::types::native_string expected_result = "Hello world";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;
  corevm::types::NativeTypeHandle oprd3 = space;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strist2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRERS)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = static_cast<corevm::types::int8>(5);
  corevm::types::native_string expected_result = "Hello";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strers, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRERS2)
{
  corevm::types::native_string hello_world = "Hello  world";
  corevm::types::int8 pos = static_cast<corevm::types::int8>(5);
  corevm::types::int8 len = static_cast<corevm::types::int8>(1);
  corevm::types::native_string expected_result = "Hello world";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;
  corevm::types::NativeTypeHandle oprd3 = len;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strers2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRRPLC)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = static_cast<corevm::types::int8>(5);
  corevm::types::int8 len = static_cast<corevm::types::int8>(6);
  corevm::types::native_string exclamation = "!!!!!!";
  corevm::types::native_string expected_result = "Hello!!!!!!";

  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;
  corevm::types::NativeTypeHandle oprd3 = len;
  corevm::types::NativeTypeHandle oprd4 = exclamation;

  push_eval_stack(eval_oprds_list{ oprd4, oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strrplc, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRSWP)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string okla = "OKLA!";
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = okla;
  corevm::types::native_string expected_result = okla;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strswp, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRSUB)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = 6;
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;
  corevm::types::native_string expected_result = "world";

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strsub, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRSUB2)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = 6;
  corevm::types::int8 len = 2;
  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = pos;
  corevm::types::NativeTypeHandle oprd3 = len;
  corevm::types::native_string expected_result = "wo";

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string>(
    corevm::runtime::instr_handler_strsub2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRFND)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = word;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string::size_type>(
    corevm::runtime::instr_handler_strfnd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRFND2)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::uint32 pos = 6;
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = word;
  corevm::types::NativeTypeHandle oprd3 = pos;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string::size_type>(
    corevm::runtime::instr_handler_strfnd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRRFND)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = word;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string::size_type>(
    corevm::runtime::instr_handler_strrfnd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeStringTypeComplexInstrsTest, TestInstrSTRRFND2)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::uint32 pos = 6;
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::NativeTypeHandle oprd1 = hello_world;
  corevm::types::NativeTypeHandle oprd2 = word;
  corevm::types::NativeTypeHandle oprd3 = pos;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_string::size_type>(
    corevm::runtime::instr_handler_strrfnd2, expected_result);
}

// -----------------------------------------------------------------------------

class InstrsNativeArrayTypeComplexInstrsTest : public InstrsNativeTypeComplexInstrsTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYLEN)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array::size_type expected_result = array.size();
  corevm::types::NativeTypeHandle oprd = array;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_array::size_type>(
    corevm::runtime::instr_handler_arylen, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYEMP)
{
  corevm::types::native_array array { 1, 2, 3 };
  bool expected_result = false;
  corevm::types::NativeTypeHandle oprd = array;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<bool>(
    corevm::runtime::instr_handler_aryemp, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYAT)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::uint32 index = 1;
  corevm::types::native_array::value_type expected_result = 2;

  corevm::types::NativeTypeHandle oprd1 = array;
  corevm::types::NativeTypeHandle oprd2 = index;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_array::value_type>(
    corevm::runtime::instr_handler_aryat, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYFRT)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array::value_type expected_result = 1;
  corevm::types::NativeTypeHandle oprd = array;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_array::value_type>(
    corevm::runtime::instr_handler_aryfrt, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYBAK)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array::value_type expected_result = 3;
  corevm::types::NativeTypeHandle oprd = array;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_array::value_type>(
    corevm::runtime::instr_handler_arybak, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYPUT)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::uint64 index = 2;
  corevm::types::uint64 data = 4;
  corevm::types::native_array expected_result { 1, 2, 4 };

  corevm::types::NativeTypeHandle oprd1 = array;
  corevm::types::NativeTypeHandle oprd2 = index;
  corevm::types::NativeTypeHandle oprd3 = data;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_aryput, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYAPND)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::uint64 data = 4;
  corevm::types::native_array expected_result { 1, 2, 3, 4 };

  corevm::types::NativeTypeHandle oprd1 = array;
  corevm::types::NativeTypeHandle oprd2 = data;

  push_eval_stack(eval_oprds_list{oprd2, oprd1});

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_aryapnd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYERS)
{
  corevm::types::native_array array { 1, 2, 3, 4 };
  corevm::types::uint64 index = 2;
  corevm::types::native_array expected_result { 1, 2, 4 };

  corevm::types::NativeTypeHandle oprd1(array);
  corevm::types::NativeTypeHandle oprd2(index);

  push_eval_stack(eval_oprds_list{oprd2, oprd1});

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_aryers, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYPOP)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array expected_result { 1, 2 };
  corevm::types::NativeTypeHandle oprd = array;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_arypop, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYSWP)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array other { 4, 5, 6 };
  corevm::types::native_array expected_result = other;

  corevm::types::NativeTypeHandle oprd1 = array;
  corevm::types::NativeTypeHandle oprd2 = other;

  push_eval_stack(eval_oprds_list{oprd2, oprd1});

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_aryswp, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYCLR)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array expected_result;

  corevm::types::NativeTypeHandle oprd = array;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_aryclr, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeArrayTypeComplexInstrsTest, TestInstrARYMRG)
{
  corevm::types::native_array array { 1, 2, 3 };
  corevm::types::native_array other_array { 4, 5, 6 };

  corevm::types::native_array expected_result {1, 2, 3, 4, 5, 6};

  corevm::types::NativeTypeHandle oprd = array;
  corevm::types::NativeTypeHandle other_oprd = other_array;

  push_eval_stack(eval_oprds_list{ other_oprd, oprd });

  execute_instr_and_assert_result<corevm::types::native_array>(
    corevm::runtime::instr_handler_arymrg, expected_result);
}

// -----------------------------------------------------------------------------

class InstrsNativeMapTypeComplexInstrsTest : public InstrsNativeTypeComplexInstrsTest {};

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPLEN)
{
  corevm::types::native_map map {
    { 1, 1 }
  };

  corevm::types::native_map::size_type expected_result = 1;
  corevm::types::NativeTypeHandle oprd = map;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_map::size_type>(
    corevm::runtime::instr_handler_maplen, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPEMP)
{
  corevm::types::native_map map {
    { 1, 1 }
  };

  bool expected_result = false;
  corevm::types::NativeTypeHandle oprd = map;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<bool>(corevm::runtime::instr_handler_mapemp,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPFIND)
{
  corevm::types::native_map map {
    { 1, 100 }
  };

  corevm::types::uint64 key = 1;
  corevm::types::NativeTypeHandle oprd1 = map;
  corevm::types::NativeTypeHandle oprd2 = key;
  bool expected_result = true;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<bool>(corevm::runtime::instr_handler_mapfind,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPAT)
{
  corevm::types::native_map map {
    { 1, 100 }
  };

  corevm::types::uint64 key = 1;
  corevm::types::native_map::mapped_type expected_result = 100;
  corevm::types::NativeTypeHandle oprd1 = map;
  corevm::types::NativeTypeHandle oprd2 = key;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_map::mapped_type>(
    corevm::runtime::instr_handler_mapat, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPPUT)
{
  corevm::types::native_map map;
  corevm::types::uint64 key = 1;
  corevm::types::uint64 value = 11;
  corevm::types::native_map expected_result;
  expected_result[1] = 11;

  corevm::types::NativeTypeHandle oprd1 = map;
  corevm::types::NativeTypeHandle oprd2 = key;
  corevm::types::NativeTypeHandle oprd3 = value;

  push_eval_stack(eval_oprds_list{ oprd3, oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_mapput, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPSET)
{
  corevm::types::NativeTypeHandle hndl = corevm::types::native_map {
    { 1, 100 },
    { 2, 200 },
    { 3, 300 }
  };

  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::closure_id_t closure_id = 10;

  corevm::runtime::ClosureCtx ctx(compartment_id, closure_id);

  corevm::runtime::Frame frame(ctx, m_compartment, &m_closure);
  frame.push_eval_stack(hndl);

  auto obj = m_process.create_dyobj();
  m_process.push_stack(obj);

  m_process.push_frame(frame);

  corevm::types::native_map_key_type key = 4;
  corevm::types::native_map_mapped_type value =
    static_cast<corevm::types::native_map_mapped_type>(obj->id());

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  corevm::types::native_map res {
    { 1, 100 },
    { 2, 200 },
    { 3, 300 },
    { key, value }
  };

  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_mapset, instr, res);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPERS)
{
  corevm::types::native_map map {
    { 1, 11 },
    { 2, 22 },
  };

  corevm::types::uint64 key = 1;
  corevm::types::native_map expected_result;
  expected_result[2] = 22;

  corevm::types::NativeTypeHandle oprd1 = map;
  corevm::types::NativeTypeHandle oprd2 = key;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_mapers, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPCLR)
{
  corevm::types::native_map map {
    { 1, 11 },
    { 2, 222 },
  };

  corevm::types::native_map expected_result;
  corevm::types::NativeTypeHandle oprd = map;

  push_eval_stack(eval_oprds_list{oprd});

  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_mapclr, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPSWP)
{
  corevm::types::native_map map {
    { 1, 11 },
    { 2, 22 },
  };

  corevm::types::native_map other {
    { 1, 22 },
    { 2, 11 },
  };

  corevm::types::native_map expected_result {
    { 1, 22 },
    { 2, 11 },
  };

  corevm::types::NativeTypeHandle oprd1 = map;
  corevm::types::NativeTypeHandle oprd2 = other;

  push_eval_stack(eval_oprds_list{ oprd2, oprd1 });

  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_mapswp, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPKEYS)
{
  corevm::types::native_map map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  // Result order is nondeterministic?
  corevm::types::native_array expected_result = { 3, 1, 2 };

  corevm::types::NativeTypeHandle oprd = map;

  push_eval_stack(eval_oprds_list{oprd});

  corevm::runtime::Frame* frame = &m_process.top_frame();
  corevm::runtime::InvocationCtx* invk_ctx = &m_process.top_invocation_ctx();

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_mapkeys(instr, m_process, &frame, &invk_ctx);

  corevm::runtime::Frame& top_frame = m_process.top_frame();

  ASSERT_GT(top_frame.eval_stack_size(), 0);

  corevm::types::NativeTypeHandle result_handle = top_frame.pop_eval_stack();

  auto actual_result =
    corevm::types::get_value_from_handle<corevm::types::native_array>(
      result_handle);

  ASSERT_EQ(expected_result.size(), actual_result.size());

  for (const auto& expected_key : expected_result)
  {
    ASSERT_NE(actual_result.end(),
      std::find(actual_result.begin(), actual_result.end(), expected_key));
  }
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPVALS)
{
  corevm::types::native_map map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  // Result order is nondeterministic?
  corevm::types::native_array expected_result = { 33, 11, 22 };

  corevm::types::NativeTypeHandle oprd = map;

  push_eval_stack(eval_oprds_list{oprd});

  corevm::runtime::Frame* frame = &m_process.top_frame();
  corevm::runtime::InvocationCtx* invk_ctx = &m_process.top_invocation_ctx();

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_mapvals(instr, m_process, &frame, &invk_ctx);

  corevm::runtime::Frame& top_frame = m_process.top_frame();

  ASSERT_GT(top_frame.eval_stack_size(), 0);

  corevm::types::NativeTypeHandle result_handle = top_frame.pop_eval_stack();

  auto actual_result =
    corevm::types::get_value_from_handle<corevm::types::native_array>(
      result_handle);

  ASSERT_EQ(expected_result.size(), actual_result.size());

  for (const auto& expected_val : expected_result)
  {
    ASSERT_NE(actual_result.end(),
      std::find(actual_result.begin(), actual_result.end(), expected_val));
  }
}

// -----------------------------------------------------------------------------

TEST_F(InstrsNativeMapTypeComplexInstrsTest, TestInstrMAPMRG)
{
  corevm::types::native_map map1 {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_map map2 {
    { 4, 44 },
    { 5, 55 },
    { 6, 66 },
  };

  corevm::types::native_map expected_result {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
    { 4, 44 },
    { 5, 55 },
    { 6, 66 },
  };

  corevm::types::NativeTypeHandle oprd1 = map1;
  corevm::types::NativeTypeHandle oprd2 = map2;

  push_eval_stack(eval_oprds_list{ oprd1, oprd2 });

  execute_instr_and_assert_result<corevm::types::native_map>(
    corevm::runtime::instr_handler_mapmrg, expected_result);
}

// -----------------------------------------------------------------------------
