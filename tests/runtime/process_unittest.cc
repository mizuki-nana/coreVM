#include <sneaker/testing/_unittest.h>
#include "../../include/runtime/process.h"
#include "../../include/runtime/sighandler_registrar.h"


class process_test : public ::testing::Test {};


class process_unittest : public process_test {};


TEST_F(process_unittest, TestInitialization)
{
  corevm::runtime::process process;
}

TEST_F(process_unittest, TestInitializationWithArgs)
{
  corevm::runtime::process process(1);
}

TEST_F(process_unittest, TestStart)
{
  corevm::runtime::process process(1);
  process.start();
}


class process_gc_rule_unittest : public process_test {
protected:
  corevm::runtime::process _process;
};


TEST_F(process_gc_rule_unittest, Test_gc_rule_always)
{
  corevm::runtime::gc_rule_always gc_rule;
  ASSERT_EQ(true, gc_rule.should_gc(_process));
}

TEST_F(process_gc_rule_unittest, Test_gc_rule_by_heap_size)
{
  corevm::runtime::gc_rule_by_heap_size gc_rule;
  ASSERT_EQ(false, gc_rule.should_gc(_process));
}

TEST_F(process_gc_rule_unittest, Test_gc_rule_by_ntvhndl_pool_size)
{
  corevm::runtime::gc_rule_by_ntvhndl_pool_size gc_rule;
  ASSERT_EQ(false, gc_rule.should_gc(_process));
}


class process_signal_handling_unittest : public process_unittest {};


TEST_F(process_signal_handling_unittest, TestHandleSignalWithUserAction)
{
  // This test currently does not work because our signal handling is only
  // for signals that gets generated from individual instructions.
  // Signal handling on the entire VM is currently not supported yet.
  // The signal explicitly raised in this test will cause a crash and a
  // core dump.
  return;
  sig_atomic_t sig = SIGINT;

  corevm::runtime::process process;
  corevm::runtime::sighandler_registrar::init(&process);

  ASSERT_EQ(0, process.stack_size());

  corevm::runtime::instr_block block_on_signal {
    {.code=corevm::runtime::instr_enum::NEW, .oprd1=0, .oprd2=0},
  };
  process.set_sig_instr_block(sig, block_on_signal);

  process.start();
  raise(sig);

  ASSERT_EQ(1, process.stack_size());
}

TEST_F(process_signal_handling_unittest, TestHandleSIGFPE)
{
  // Tests that we are able to capture signals caused by individual
  // instructions. In this case we are capturing the floating point error signal
  // from the `div` instruction when the divisor is zero, and also
  // we are able to catch it more than once.
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

  std::vector<corevm::runtime::instr> instrs = {
    {.code=corevm::runtime::instr_enum::DIV, .oprd1=0, .oprd2=0},
    {.code=corevm::runtime::instr_enum::DIV, .oprd1=0, .oprd2=0},
  };

  process.append_instrs(instrs);

  corevm::runtime::instr_block block_on_signal {
    {.code=corevm::runtime::instr_enum::NEW, .oprd1=0, .oprd2=0},
  };
  process.set_sig_instr_block(sig, block_on_signal);

  process.start();

  ASSERT_EQ(2, process.stack_size());
}
