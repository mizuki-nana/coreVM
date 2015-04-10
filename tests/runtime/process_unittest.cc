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
#include "runtime/gc_rule.h"
#include "runtime/process.h"
#include "runtime/process_runner.h"
#include "runtime/sighandler_registrar.h"
#include "runtime/vector.h"
#include "types/native_type_handle.h"
#include "types/types.h"

#include <sneaker/testing/_unittest.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <sstream>


class process_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestInitialization)
{
  corevm::runtime::process process;
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestStart)
{
  // TODO: [COREVM-117] Process start causes seg fault


  /******************************** DISABLED ***********************************

  corevm::runtime::process process;
  corevm::runtime::process_runner runner(process);
  runner.start();

  ******************************** DISABLED ***********************************/
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestDefaultAndMaxSizes)
{
  corevm::runtime::process process;

  ASSERT_EQ(0, process.heap_size());
  ASSERT_EQ(0, process.ntvhndl_pool_size());

  ASSERT_LT(0, process.max_heap_size());
  ASSERT_LT(0, process.max_ntvhndl_pool_size());
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestInstantiateWithParameters)
{
  uint64_t heap_alloc_size = 2048;
  uint64_t pool_alloc_size = 1024;

  corevm::runtime::process process(heap_alloc_size, pool_alloc_size);

  ASSERT_EQ(0, process.heap_size());
  ASSERT_EQ(0, process.ntvhndl_pool_size());

  ASSERT_LT(0, process.max_heap_size());
  ASSERT_LT(0, process.max_ntvhndl_pool_size());
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestAppendVector)
{
  // The process's vector is inaccessible to the outside world, so we need to
  // simulate its implementation here.
  corevm::runtime::vector target {
    { .code=6, .oprd1=421, .oprd2=52 },
    { .code=5, .oprd1=532, .oprd2=0  },
    { .code=2, .oprd1=72,  .oprd2=0  },
  };

  corevm::runtime::vector src {
    { .code=36, .oprd1=41, .oprd2=81 },
    { .code=37, .oprd1=27, .oprd2=0  },
    { .code=83, .oprd1=93, .oprd2=0  },
  };

  std::copy(src.begin(), src.end(), std::back_inserter(target));

  corevm::runtime::vector expected {
    { .code=6, .oprd1=421, .oprd2=52 },
    { .code=5, .oprd1=532, .oprd2=0  },
    { .code=2, .oprd1=72,  .oprd2=0  },
    { .code=36, .oprd1=41, .oprd2=81 },
    { .code=37, .oprd1=27, .oprd2=0  },
    { .code=83, .oprd1=93, .oprd2=0  },
  };

  ASSERT_EQ(expected, target);
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestInsertVector)
{
  // The process's vector is inaccessible to the outside world, so we need to
  // simulate its implementation here.
  corevm::runtime::vector target {
    { .code=6,  .oprd1=421, .oprd2=52 },
    { .code=5,  .oprd1=532, .oprd2=0  },
    { .code=2,  .oprd1=72,  .oprd2=0  },
    { .code=71, .oprd1=25,  .oprd2=0  },
    { .code=18, .oprd1=51,  .oprd2=43 },
    { .code=17, .oprd1=11,  .oprd2=99 },
    { .code=33, .oprd1=88,  .oprd2=55 },
    { .code=88, .oprd1=11,  .oprd2=91 },
    { .code=17, .oprd1=71,  .oprd2=23 },
    { .code=91, .oprd1=64,  .oprd2=67 },
  };

  corevm::runtime::vector src {
    { .code=36, .oprd1=41,  .oprd2=81 },
    { .code=37, .oprd1=27,  .oprd2=0  },
    { .code=83, .oprd1=93,  .oprd2=0  },
  };

  int64_t fake_pc = 5;

  target.insert(target.begin() + fake_pc + 1, src.begin(), src.end());

  corevm::runtime::vector expected {
    { .code=6,  .oprd1=421, .oprd2=52 },
    { .code=5,  .oprd1=532, .oprd2=0  },
    { .code=2,  .oprd1=72,  .oprd2=0  },
    { .code=71, .oprd1=25,  .oprd2=0  },
    { .code=18, .oprd1=51,  .oprd2=43 },
    { .code=17, .oprd1=11,  .oprd2=99 },
    { .code=36, .oprd1=41,  .oprd2=81 },
    { .code=37, .oprd1=27,  .oprd2=0  },
    { .code=83, .oprd1=93,  .oprd2=0  },
    { .code=33, .oprd1=88,  .oprd2=55 },
    { .code=88, .oprd1=11,  .oprd2=91 },
    { .code=17, .oprd1=71,  .oprd2=23 },
    { .code=91, .oprd1=64,  .oprd2=67 },
  };

  ASSERT_EQ(expected, target);
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestPushAndPopStack)
{
  corevm::runtime::process process;

  corevm::dyobj::dyobj_id id1 = 1;
  corevm::dyobj::dyobj_id id2 = 2;

  ASSERT_THROW(
    {
      process.top_stack();
    },
    corevm::runtime::object_stack_empty_error
  );

  ASSERT_THROW(
    {
      process.pop_stack();
    },
    corevm::runtime::object_stack_empty_error
  );

  process.push_stack(id1);

  auto actual_id1 = process.top_stack();

  ASSERT_EQ(id1, actual_id1);

  process.push_stack(id2);

  auto actual_id2 = process.top_stack();

  ASSERT_EQ(id2, actual_id2);

  ASSERT_NO_THROW(
    {
      process.pop_stack();
    }
  );

  ASSERT_EQ(id1, process.top_stack());

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
    corevm::runtime::object_stack_empty_error
  );

  ASSERT_THROW(
    {
      process.pop_stack();
    },
    corevm::runtime::object_stack_empty_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestSwapStack)
{
  corevm::runtime::process process;

  corevm::dyobj::dyobj_id id1 = 1;
  corevm::dyobj::dyobj_id id2 = 2;

  process.push_stack(id2);
  process.push_stack(id1);

  ASSERT_EQ(id1, process.top_stack());

  process.swap_stack();

  ASSERT_EQ(id2, process.top_stack());

  ASSERT_EQ(id2, process.pop_stack());
  ASSERT_EQ(id1, process.pop_stack());
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestPushAndPopFrames)
{
  corevm::runtime::process process;

  ASSERT_EQ(false, process.has_frame());
  ASSERT_EQ(0, process.call_stack_size());

  corevm::runtime::compartment compartment("./example.core");

  corevm::runtime::vector vector {
    { .code=6, .oprd1=421, .oprd2=523 },
    { .code=5, .oprd1=532, .oprd2=0   },
    { .code=2, .oprd1=72,  .oprd2=0   },
  };

  corevm::runtime::closure closure {
    .id=1,
    .parent_id=0,
    .vector=vector
  };

  corevm::runtime::closure_table closure_table {
    closure
  };

  compartment.set_closure_table(closure_table);

  // simulate `process::pre_start()`.
  auto compartment_id = process.insert_compartment(compartment);
  process.append_vector(vector);
  process.set_pc(0);

  corevm::runtime::closure_ctx ctx {
    .compartment_id = compartment_id,
    .closure_id = closure.id,
  };

  process.emplace_invocation_ctx(ctx);

  corevm::runtime::frame frame(ctx);
  frame.set_return_addr(process.pc());
  process.push_frame(frame);
  process.insert_vector(closure.vector);

  ASSERT_EQ(true, process.has_frame());
  ASSERT_EQ(1, process.call_stack_size());

  auto original_pc = process.pc();

  ASSERT_NO_THROW(
    {
      process.pop_frame();
    }
  );

  ASSERT_EQ(original_pc, process.pc());

  ASSERT_EQ(false, process.has_frame());
  ASSERT_EQ(0, process.call_stack_size());

  ASSERT_THROW(
    {
      process.pop_frame();
    },
    corevm::runtime::frame_not_found_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestEmplaceFrame)
{
  corevm::runtime::process process;

  ASSERT_EQ(false, process.has_frame());
  ASSERT_EQ(0, process.call_stack_size());

  corevm::runtime::closure_ctx ctx {
    .compartment_id = 0,
    .closure_id = 0,
  };

  process.emplace_frame(ctx);

  ASSERT_EQ(true, process.has_frame());
  ASSERT_EQ(1, process.call_stack_size());

  auto& frame = process.top_frame();

  ASSERT_EQ(ctx.compartment_id, frame.closure_ctx().compartment_id);
  ASSERT_EQ(ctx.closure_id, frame.closure_ctx().closure_id);
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestInsertAndAccessNativeTypeHandle)
{
  corevm::runtime::process process;
  int value = 8;

  corevm::types::native_type_handle hndl = corevm::types::int8(value);

  auto key = process.insert_ntvhndl(hndl);

  ASSERT_EQ(true, process.has_ntvhndl(key));

  auto actual_hndl = process.get_ntvhndl(key);

  int8_t actual_value = corevm::types::get_value_from_handle<int8_t>(actual_hndl);

  ASSERT_EQ(value, actual_value);
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestInsertAndEraseNativeTypeHandle)
{
  corevm::runtime::process process;
  int value = 8;

  corevm::types::native_type_handle hndl = corevm::types::int8(value);

  auto key = process.insert_ntvhndl(hndl);

  ASSERT_EQ(true, process.has_ntvhndl(key));

  ASSERT_NO_THROW(
    {
      process.erase_ntvhndl(key);
    }
  );

  ASSERT_EQ(false, process.has_ntvhndl(key));

  // Try erase again
  ASSERT_THROW(
    {
      process.erase_ntvhndl(key);
    },
    corevm::runtime::native_type_handle_deletion_error
  );

  // Try access it
  ASSERT_THROW(
    {
      process.get_ntvhndl(key);
    },
    corevm::runtime::native_type_handle_not_found_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestGetFrameByClosureCtx)
{
  corevm::runtime::process process;

  corevm::runtime::compartment_id compartment_id = 0;

  corevm::runtime::closure_id closure_id1 = 100;
  corevm::runtime::closure_id closure_id2 = 200;
  corevm::runtime::closure_id closure_id3 = 300;

  corevm::runtime::closure_ctx ctx1 {
    .compartment_id = compartment_id,
    .closure_id = closure_id1
  };

  corevm::runtime::closure_ctx ctx2 {
    .compartment_id = compartment_id,
    .closure_id = closure_id2
  };

  corevm::runtime::closure_ctx ctx3 {
    .compartment_id = compartment_id,
    .closure_id = closure_id3
  };

  corevm::runtime::frame frame1(ctx3);
  corevm::runtime::frame frame2(ctx2);

  corevm::runtime::frame* ptr = nullptr;

  process.push_frame(frame1);
  process.push_frame(frame2);

  process.get_frame_by_closure_ctx(ctx1, &ptr);

  ASSERT_EQ(nullptr, ptr);

  process.get_frame_by_closure_ctx(ctx2, &ptr);

  ASSERT_NE(nullptr, ptr);
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestInsertCompartment)
{
  corevm::runtime::process process;

  corevm::runtime::compartment compartment("./example.core");

  auto compartment_id = process.insert_compartment(compartment);

  ASSERT_NE(corevm::runtime::NONESET_COMPARTMENT_ID, compartment_id);
}

// -----------------------------------------------------------------------------

TEST_F(process_unittest, TestOutputStream)
{
  corevm::runtime::process process;

  // Compartment 1

  corevm::runtime::compartment compartment1("./example.core");

  corevm::runtime::vector vector1 {
    { .code=6, .oprd1=421, .oprd2=523 },
    { .code=5, .oprd1=532, .oprd2=0   },
    { .code=2, .oprd1=72,  .oprd2=0   },
  };

  corevm::runtime::closure closure1 {
    .id=2,
    .parent_id=1,
    .vector=vector1
  };

  corevm::runtime::closure_table closure_table1 {
    closure1
  };

  compartment1.set_closure_table(closure_table1);

  process.insert_compartment(compartment1);

  // Compartment 2

  corevm::runtime::compartment compartment2("./tests/sample01.core");

  corevm::runtime::vector vector2 {
    { .code=17, .oprd1=957, .oprd2=0  },
    { .code=59, .oprd1=0,   .oprd2=0  },
    { .code=22, .oprd1=5,   .oprd2=0  },
  };

  corevm::runtime::closure closure2 {
    .id=1,
    .parent_id=corevm::runtime::NONESET_CLOSURE_ID,
    .vector=vector2
  };

  corevm::runtime::closure_table closure_table2 {
    closure2
  };

  compartment2.set_closure_table(closure_table2);

  process.insert_compartment(compartment2);

  auto id = corevm::runtime::process::adapter(process).help_create_dyobj();
  auto &obj = corevm::runtime::process::adapter(process).help_get_dyobj(id);
  obj.set_ntvhndl_key(1);
  obj.set_closure_ctx(
    corevm::runtime::closure_ctx {
      .compartment_id=1,
      .closure_id=2
    }
  );

  corevm::types::native_type_handle hndl = corevm::types::uint32(32);
  process.insert_ntvhndl(hndl);

  std::stringstream ss;
  ss << process;

  ASSERT_NE(0, ss.str().size());
}

// -----------------------------------------------------------------------------

class process_gc_rule_unittest : public process_unittest
{
protected:
  corevm::runtime::process _process;
};

// -----------------------------------------------------------------------------

TEST_F(process_gc_rule_unittest, Test_gc_rule_always)
{
  corevm::runtime::gc_rule_always gc_rule;
  ASSERT_EQ(true, gc_rule.should_gc(_process));
}

// -----------------------------------------------------------------------------

TEST_F(process_gc_rule_unittest, Test_gc_rule_by_heap_size)
{
  corevm::runtime::gc_rule_by_heap_size gc_rule;
  ASSERT_EQ(false, gc_rule.should_gc(_process));
}

// -----------------------------------------------------------------------------

TEST_F(process_gc_rule_unittest, Test_gc_rule_by_ntvhndl_pool_size)
{
  corevm::runtime::gc_rule_by_ntvhndl_pool_size gc_rule;
  ASSERT_EQ(false, gc_rule.should_gc(_process));
}

// -----------------------------------------------------------------------------

class process_signal_handling_unittest : public process_unittest {};

// -----------------------------------------------------------------------------

TEST_F(process_signal_handling_unittest, TestHandleSignalWithUserAction)
{
  /* This test currently does not work because our signal handling is only
   * for signals that gets generated from individual instructions.
   * Signal handling on the entire VM is currently not supported yet.
   * The signal explicitly raised in this test will cause a crash and a
   * core dump.
   **/

  /*
  sig_atomic_t sig = SIGINT;

  corevm::runtime::process process;
  corevm::runtime::sighandler_registrar::init(&process);

  ASSERT_EQ(0, process.stack_size());

  corevm::runtime::vector vector {
    { .code=corevm::runtime::instr_enum::NEW, .oprd1=0, .oprd2=0 },
  };
  process.set_sig_vector(sig, vector);

  process.start();
  raise(sig);

  ASSERT_EQ(1, process.stack_size());
  */
}

// -----------------------------------------------------------------------------

TEST_F(process_signal_handling_unittest, TestHandleSIGFPE)
{
  /* Tests that we are able to capture signals caused by individual
   * instructions. In this case we are capturing the floating point error signal
   * from the `div` instruction when the divisor is zero, and also
   * we are able to catch it more than once.
   **/

  // TODO: [COREVM-117] Process start causes seg fault

  /******************************** DISABLED ***********************************

  sig_atomic_t sig = SIGFPE;

  corevm::runtime::process process;
  corevm::runtime::frame frame;
  corevm::runtime::sighandler_registrar::init(&process);
  corevm::types::native_type_handle oprd4 = corevm::types::int8(0);
  corevm::types::native_type_handle oprd3 = corevm::types::int8(10);
  corevm::types::native_type_handle oprd2 = corevm::types::int8(0);
  corevm::types::native_type_handle oprd1 = corevm::types::int8(10);

  frame.push_eval_stack(oprd4);
  frame.push_eval_stack(oprd3);
  frame.push_eval_stack(oprd2);
  frame.push_eval_stack(oprd1);
  process.push_frame(frame);

  ASSERT_EQ(0, process.stack_size());

  std::vector<corevm::runtime::instr> instrs {
    { .code=corevm::runtime::instr_enum::DIV, .oprd1=0, .oprd2=0 },
    { .code=corevm::runtime::instr_enum::DIV, .oprd1=0, .oprd2=0 },
  };

  process.append_instrs(instrs);

  corevm::runtime::vector vector {
    { .code=corevm::runtime::instr_enum::NEW, .oprd1=0, .oprd2=0 },
  };
  process.set_sig_vector(sig, vector);

  process.start();

  ASSERT_EQ(2, process.stack_size());

  ******************************** DISABLED ***********************************/
}

// -----------------------------------------------------------------------------
