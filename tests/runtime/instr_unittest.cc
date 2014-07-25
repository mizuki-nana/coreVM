#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/runtime/instr.h"


class instr_unittest : public ::testing::Test {};


TEST_F(instr_unittest, TestInitialization)
{
  corevm::runtime::instr_handler_new handler;
}
