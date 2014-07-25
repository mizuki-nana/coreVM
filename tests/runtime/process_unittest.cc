#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/runtime/process.h"


class process_unittest : public ::testing::Test {};


TEST_F(process_unittest, TestInitialization)
{
  corevm::runtime::process process;
}

TEST_F(process_unittest, TestInitializationWithArgs)
{
  corevm::runtime::process process(10);
}
