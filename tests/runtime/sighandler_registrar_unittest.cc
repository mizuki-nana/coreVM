#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/runtime/process.h"
#include "../../include/runtime/sighandler_registrar.h"


class sighandler_registrar_test : public ::testing::Test {};


TEST_F(sighandler_registrar_test, TestInit)
{
  corevm::runtime::process process;
  corevm::runtime::sighandler_registrar::init(&process);
}
