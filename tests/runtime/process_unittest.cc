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
#include "runtime/gc_rule.h"
#include "runtime/process.h"
#include "runtime/process_runner.h"
#include "runtime/vector.h"
#include "types/native_type_handle.h"
#include "types/types.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <iterator>


class ProcessUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestInitialization)
{
  corevm::runtime::Process process;
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestStart)
{
  // TODO: [COREVM-117] Process start causes seg fault


  /******************************** DISABLED ***********************************

  corevm::runtime::Process process;
  corevm::runtime::process_runner runner(process);
  runner.start();

  ******************************** DISABLED ***********************************/
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestDefaultAndMaxSizes)
{
  corevm::runtime::Process process;

  ASSERT_EQ(0, process.heap_size());
  ASSERT_EQ(0, process.ntvhndl_pool_size());

  ASSERT_LT(0, process.max_heap_size());
  ASSERT_LT(0, process.max_ntvhndl_pool_size());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestInstantiateWithParameters)
{
  uint64_t heap_alloc_size = 2048;
  uint64_t pool_alloc_size = 1024;

  corevm::runtime::Process process(heap_alloc_size, pool_alloc_size);

  ASSERT_EQ(0, process.heap_size());
  ASSERT_EQ(0, process.ntvhndl_pool_size());

  ASSERT_LT(0, process.max_heap_size());
  ASSERT_LT(0, process.max_ntvhndl_pool_size());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestPushAndPopStack)
{
  corevm::runtime::Process process;

  auto obj1 = process.create_dyobj();
  auto obj2 = process.create_dyobj();

  ASSERT_THROW(
    {
      process.top_stack();
    },
    corevm::runtime::ObjectStackEmptyError
  );

  ASSERT_THROW(
    {
      process.pop_stack();
    },
    corevm::runtime::ObjectStackEmptyError
  );

  process.push_stack(obj1);

  auto actual_obj1 = process.top_stack();

  ASSERT_EQ(obj1, actual_obj1);

  process.push_stack(obj2);

  auto actual_obj2 = process.top_stack();

  ASSERT_EQ(obj2, actual_obj2);

  ASSERT_NO_THROW(
    {
      process.pop_stack();
    }
  );

  ASSERT_EQ(obj1, process.top_stack());

  ASSERT_NO_THROW(
    {
      process.pop_stack();
    }
  );

  // Stack is empty by this point.
  ASSERT_THROW(
    {
      process.top_stack();
    },
    corevm::runtime::ObjectStackEmptyError
  );

  ASSERT_THROW(
    {
      process.pop_stack();
    },
    corevm::runtime::ObjectStackEmptyError
  );
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestSwapStack)
{
  corevm::runtime::Process process;

  auto obj1 = process.create_dyobj();
  auto obj2 = process.create_dyobj();

  process.push_stack(obj2);
  process.push_stack(obj1);

  ASSERT_EQ(obj1, process.top_stack());

  process.swap_stack();

  ASSERT_EQ(obj2, process.top_stack());

  ASSERT_EQ(obj2, process.pop_stack());
  ASSERT_EQ(obj1, process.pop_stack());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestPushAndPopFrames)
{
  corevm::runtime::Process process;

  ASSERT_EQ(false, process.has_frame());
  ASSERT_EQ(0, process.call_stack_size());

  corevm::runtime::Compartment compartment("./example.core");

  corevm::runtime::Vector vector {
    corevm::runtime::Instr(100, 100, 100),
    corevm::runtime::Instr(100, 100, 100),
    corevm::runtime::Instr(100, 100, 100),
  };

  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure
  };

  compartment.set_closure_table(std::move(closure_table));

  // simulate `process::pre_start()`.
  auto compartment_id = process.insert_compartment(compartment);

  corevm::runtime::ClosureCtx ctx(compartment_id, closure.id);

  process.emplace_invocation_ctx(ctx, &compartment, &closure_table[0]);

  corevm::runtime::Frame frame(ctx, &compartment, &closure_table[0]);
  process.push_frame(frame);
  process.set_pc(0);
  frame.set_return_addr(process.pc());

  ASSERT_EQ(true, process.has_frame());
  ASSERT_EQ(1, process.call_stack_size());

  ASSERT_NO_THROW(
    {
      process.pop_frame();
    }
  );

  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, process.pc());

  ASSERT_EQ(false, process.has_frame());
  ASSERT_EQ(0, process.call_stack_size());

  ASSERT_THROW(
    {
      process.pop_frame();
    },
    corevm::runtime::FrameNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestEmplaceFrame)
{
  corevm::runtime::Process process;

  ASSERT_EQ(false, process.has_frame());
  ASSERT_EQ(0, process.call_stack_size());

  corevm::runtime::ClosureCtx ctx(0, 0);

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "",
    ctx.closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };

  corevm::runtime::Compartment compartment("");

  compartment.set_closure_table(std::move(closure_table));
  process.insert_compartment(compartment);

  process.emplace_frame(ctx, &compartment, &closure_table[0]);

  ASSERT_EQ(true, process.has_frame());
  ASSERT_EQ(1, process.call_stack_size());

  auto& frame = process.top_frame();

  ASSERT_EQ(&compartment, frame.compartment_ptr());
  ASSERT_EQ(&closure_table[0], frame.closure_ptr());
  ASSERT_EQ(ctx.compartment_id, frame.closure_ctx().compartment_id);
  ASSERT_EQ(ctx.closure_id, frame.closure_ctx().closure_id);
  ASSERT_EQ(corevm::runtime::NONESET_INSTR_ADDR, frame.return_addr());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestTopNthFrame)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::ClosureCtx ctx1(0, 1);
  corevm::runtime::ClosureCtx ctx2(0, 2);
  corevm::runtime::ClosureCtx ctx3(0, 3);

  corevm::runtime::Closure closure(
    "",
    ctx1.closure_id,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };

  corevm::runtime::Compartment compartment("");

  compartment.set_closure_table(std::move(closure_table));
  process.insert_compartment(compartment);

  corevm::runtime::Frame frame1(ctx1, &compartment, &closure_table[0]);
  corevm::runtime::Frame frame2(ctx2, &compartment, &closure_table[0]);
  corevm::runtime::Frame frame3(ctx3, &compartment, &closure_table[0]);

  process.push_frame(frame1);
  process.push_frame(frame2);
  process.push_frame(frame3);

  ASSERT_EQ(3, process.call_stack_size());

  // Top frame.
  auto& frame_1st = process.top_nth_frame(0);
  ASSERT_EQ(frame3.closure_ctx().closure_id, frame_1st.closure_ctx().closure_id);

  // 2nd top frame.
  auto& frame_2nd = process.top_nth_frame(1);
  ASSERT_EQ(frame2.closure_ctx().closure_id, frame_2nd.closure_ctx().closure_id);

  // 3rd top frame.
  auto& frame_3rd = process.top_nth_frame(2);
  ASSERT_EQ(frame1.closure_ctx().closure_id, frame_3rd.closure_ctx().closure_id);

  // Invalid `n`.
  ASSERT_THROW(
    {
      process.top_nth_frame(3);
    },
    corevm::runtime::FrameNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestInsertAndAccessNativeTypeHandle)
{
  corevm::runtime::Process process;
  int value = 8;

  corevm::types::NativeTypeHandle hndl = corevm::types::int8(value);

  auto saved_hndl = process.insert_ntvhndl(hndl);

  auto actual_hndl = process.get_ntvhndl(saved_hndl);

  int8_t actual_value = corevm::types::get_value_from_handle<int8_t>(actual_hndl);

  ASSERT_EQ(value, actual_value);
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestInsertAndEraseNativeTypeHandle)
{
  corevm::runtime::Process process;
  int value = 8;

  corevm::types::NativeTypeHandle hndl = corevm::types::int8(value);

  auto saved_hndl = process.insert_ntvhndl(hndl);

  ASSERT_NO_THROW(
    {
      process.erase_ntvhndl(saved_hndl);
    }
  );

  // Try erase again
  ASSERT_THROW(
    {
      process.erase_ntvhndl(saved_hndl);
    },
    corevm::runtime::NativeTypeHandleDeletionError
  );

  // Try access it
  ASSERT_THROW(
    {
      process.get_ntvhndl(saved_hndl);
    },
    corevm::runtime::NativeTypeHandleNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestGetFrameByClosureCtx)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::compartment_id_t compartment_id = 0;

  corevm::runtime::closure_id_t closure_id1 = 0;
  corevm::runtime::closure_id_t closure_id2 = 1;
  corevm::runtime::closure_id_t closure_id3 = 2;

  corevm::runtime::ClosureCtx ctx1(compartment_id, closure_id1);
  corevm::runtime::ClosureCtx ctx2(compartment_id, closure_id2);
  corevm::runtime::ClosureCtx ctx3(compartment_id, closure_id3);

  corevm::runtime::Closure closure1(
    "",
    closure_id1,
    closure_id2,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure2(
    "",
    closure_id2,
    closure_id3,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure3(
    "",
    closure_id3,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure1, closure2, closure3 };

  corevm::runtime::Compartment compartment("");
  compartment.set_closure_table(std::move(closure_table));
  process.insert_compartment(compartment);

  corevm::runtime::Frame frame1(ctx3, &compartment, &closure1);
  corevm::runtime::Frame frame2(ctx2, &compartment, &closure2);

  corevm::runtime::Frame* ptr = nullptr;

  process.push_frame(frame1);
  process.push_frame(frame2);

  process.get_frame_by_closure_ctx(ctx1, &ptr);

  ASSERT_EQ(nullptr, ptr);

  process.get_frame_by_closure_ctx(ctx2, &ptr);

  ASSERT_NE(nullptr, ptr);
}

// -----------------------------------------------------------------------------

TEST_F(ProcessUnitTest, TestInsertCompartment)
{
  corevm::runtime::Process process;

  corevm::runtime::Compartment compartment("./example.core");

  auto compartment_id = process.insert_compartment(compartment);

  ASSERT_NE(corevm::runtime::NONESET_COMPARTMENT_ID, compartment_id);
}

// -----------------------------------------------------------------------------

class ProcessGCRuleUnitTest : public ProcessUnitTest
{
protected:
  corevm::runtime::Process _process;
};

// -----------------------------------------------------------------------------

TEST_F(ProcessGCRuleUnitTest, TestGCRuleAlways)
{
  ASSERT_EQ(true, corevm::runtime::gc_rule_always(_process));
}

// -----------------------------------------------------------------------------

TEST_F(ProcessGCRuleUnitTest, TestGCRuleByHeapSize)
{
  ASSERT_EQ(false, corevm::runtime::gc_rule_by_heap_size(_process));
}

// -----------------------------------------------------------------------------

TEST_F(ProcessGCRuleUnitTest, TestGCRuleByNtvhndlPoolSize)
{
  ASSERT_EQ(false, corevm::runtime::gc_rule_by_ntvhndl_pool_size(_process));
}

// -----------------------------------------------------------------------------

class ProcessFindFrameByCtxUnitTest : public ProcessUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(ProcessFindFrameByCtxUnitTest, TestFindFrameWithAssociatedCtx)
{
  corevm::runtime::Process process;

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

  corevm::runtime::Compartment compartment("dummy-path");

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::ClosureCtx ctx(0, closure.id);

  process.insert_compartment(compartment);

  process.emplace_frame(ctx, &compartment, &closure_table[0]);

  corevm::runtime::Frame* res = corevm::runtime::Process::find_frame_by_ctx(
    ctx, &compartment, process);

  ASSERT_NE(nullptr, res);
  ASSERT_TRUE(ctx == res->closure_ctx());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessFindFrameByCtxUnitTest, TestFindFrameByTraverseClosureTree)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure1(
    "__main__",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure2(
    "run",
    1,
    0,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure3(
    "",
    2,
    1,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure1, closure2, closure3 };

  corevm::runtime::Compartment compartment("dummy-path");

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::ClosureCtx ctx1(0, closure1.id);

  corevm::runtime::ClosureCtx ctx3(0, closure3.id);

  process.insert_compartment(compartment);

  process.emplace_frame(ctx1, &compartment, &closure_table[0]);

  corevm::runtime::Frame* res = corevm::runtime::Process::find_frame_by_ctx(
    ctx3, &compartment, process);

  ASSERT_NE(nullptr, res);
  ASSERT_TRUE(ctx1 == res->closure_ctx());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessFindFrameByCtxUnitTest, TestFindMissingFrame)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure1(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure2(
    "",
    1,
    0,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure3(
    "",
    2,
    1,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure1, closure2, closure3 };

  corevm::runtime::Compartment compartment("dummy-path");

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::ClosureCtx ctx1(0, closure1.id);
  corevm::runtime::ClosureCtx ctx2(0, closure2.id);
  corevm::runtime::ClosureCtx ctx3(0, closure3.id);

  process.insert_compartment(compartment);

  process.emplace_frame(ctx2, &compartment, &closure_table[1], 0);
  process.emplace_frame(ctx3, &compartment, &closure_table[2], 0);

  corevm::runtime::Frame* res = corevm::runtime::Process::find_frame_by_ctx(
    ctx1, &compartment, process);

  ASSERT_EQ(nullptr, res);
}

// -----------------------------------------------------------------------------

class ProcessFindParentFrameInProcessUnitTest : public ProcessUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(ProcessFindParentFrameInProcessUnitTest, TestFindParentFrameSuccessful)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure1(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure2(
    "",
    1,
    0,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure3(
    "",
    2,
    1,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure1, closure2, closure3 };

  corevm::runtime::Compartment compartment("dummy-path");

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::ClosureCtx ctx1(0, closure1.id);
  corevm::runtime::ClosureCtx ctx2(0, closure2.id);
  corevm::runtime::ClosureCtx ctx3(0, closure3.id);

  process.insert_compartment(compartment);

  process.emplace_frame(ctx1, &compartment, &closure_table[0], 0);
  process.emplace_frame(ctx2, &compartment, &closure_table[1], 0);
  process.emplace_frame(ctx3, &compartment, &closure_table[2], 0);

  corevm::runtime::Frame* frame_ptr = &process.top_frame();

  corevm::runtime::Frame* res = corevm::runtime::Process::find_parent_frame_in_process(
    frame_ptr, process);

  ASSERT_NE(nullptr, res);
  ASSERT_TRUE(ctx2 == res->closure_ctx());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessFindParentFrameInProcessUnitTest, TestFindParentFrameWithMissingIntermediateFrame)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure1(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure2(
    "",
    1,
    0,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure3(
    "",
    2,
    1,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure1, closure2, closure3 };

  corevm::runtime::Compartment compartment("dummy-path");

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::ClosureCtx ctx1(0, closure1.id);

  corevm::runtime::ClosureCtx ctx3(0, closure3.id);

  process.insert_compartment(compartment);

  process.emplace_frame(ctx1, &compartment, &closure_table[0], 0);
  process.emplace_frame(ctx3, &compartment, &closure_table[2], 0);

  corevm::runtime::Frame* frame_ptr = &process.top_frame();

  corevm::runtime::Frame* res = corevm::runtime::Process::find_parent_frame_in_process(
    frame_ptr, process);

  ASSERT_NE(nullptr, res);
  ASSERT_TRUE(ctx1 == res->closure_ctx());
}

// -----------------------------------------------------------------------------

TEST_F(ProcessFindParentFrameInProcessUnitTest, TestFindParentFrameFails)
{
  corevm::runtime::Process process;

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure1(
    "",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure2(
    "",
    1,
    0,
    vector,
    locs,
    catch_sites);

  corevm::runtime::Closure closure3(
    "",
    2,
    1,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure1, closure2, closure3 };

  corevm::runtime::Compartment compartment("dummy-path");

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::ClosureCtx ctx3(0, closure3.id);

  process.insert_compartment(compartment);

  process.emplace_frame(ctx3, &compartment, &closure_table[2], 0);

  corevm::runtime::Frame* frame_ptr = &process.top_frame();

  corevm::runtime::Frame* res = corevm::runtime::Process::find_parent_frame_in_process(
    frame_ptr, process);

  ASSERT_EQ(nullptr, res);
}

// -----------------------------------------------------------------------------
