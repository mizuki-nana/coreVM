#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/runtime/process.h"


class process_test : public ::testing::Test {};


class process_unittest : public process_test {};


TEST_F(process_unittest, TestInitialization)
{
  corevm::runtime::process process;
}

TEST_F(process_unittest, TestInitializationWithArgs)
{
  corevm::runtime::process process(10);
}


class process_instr_integration_test : public process_test {
public:
  typedef uint32_t IntrinsicType;
  typedef corevm::types::uint32 NativeType;

protected:
  template<class InstrHandlerCls>
  void execute_instr_and_assert_result(IntrinsicType expected_result) {
    InstrHandlerCls instr_handler;

    corevm::runtime::instr instr;

    instr_handler.execute(instr, _process);

    corevm::runtime::frame& frame = _process.top_frame();
    corevm::types::native_type_handle result_handle = frame.pop_eval_stack();

    IntrinsicType actual_result = corevm::types::get_value_from_handle<IntrinsicType>(
      result_handle
    );

    ASSERT_EQ(expected_result, actual_result);
  }

  corevm::runtime::frame _frame;
  corevm::runtime::process _process;
};


class process_arithmetic_instrs_test : public process_instr_integration_test {};


class process_unary_arithmetic_instrs_test : public process_arithmetic_instrs_test {
public:
  virtual void SetUp () {
    corevm::types::native_type_handle hndl = NativeType(_oprd);
    _frame.push_eval_stack(hndl);
    _process.push_frame(_frame);
  }

protected:
  IntrinsicType _oprd = 5;
};


TEST_F(process_unary_arithmetic_instrs_test, TestInstrPOS)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_pos>(+_oprd);
}

TEST_F(process_unary_arithmetic_instrs_test, TestInstrNEG)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_neg>(-_oprd);
}

TEST_F(process_unary_arithmetic_instrs_test, TestInstrINC)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_inc>(++_oprd);
}

TEST_F(process_unary_arithmetic_instrs_test, TestInstrDEC)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_dec>(--_oprd);
}

TEST_F(process_unary_arithmetic_instrs_test, TestInstrBNOT)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_bnot>(~_oprd);
}

TEST_F(process_unary_arithmetic_instrs_test, TestInstrLNOT)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_lnot>(!_oprd);
}


class process_binary_arithmetic_instrs_test : public process_arithmetic_instrs_test {
public:
  virtual void SetUp () {
    corevm::types::native_type_handle hndl1 = NativeType(_oprd1);
    corevm::types::native_type_handle hndl2 = NativeType(_oprd2);
    _frame.push_eval_stack(hndl2);
    _frame.push_eval_stack(hndl1);
    _process.push_frame(_frame);
  }

protected:
  IntrinsicType _oprd1 = 10;
  IntrinsicType _oprd2 = 5;
};


TEST_F(process_binary_arithmetic_instrs_test, TestInstrADD)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_add>(_oprd1 + _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrSUB)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_sub>(_oprd1 - _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrMUL)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_mul>(_oprd1 * _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrDIV)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_div>(_oprd1 / _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrMOD)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_mod>(_oprd1 % _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrPOW)
{
  // TODO: to be implemented...
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrBAND)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_band>(_oprd1 & _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrBOR)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_bor>(_oprd1 | _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrBXOR)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_bxor>(_oprd1 ^ _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrBLS)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_bls>(_oprd1 << _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrBRS)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_brs>(_oprd1 >> _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrEQ)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_eq>(_oprd1 == _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrNEQ)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_neq>(_oprd1 != _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrGT)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_gt>(_oprd1 > _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrLT)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_lt>(_oprd1 < _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrGTE)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_gte>(_oprd1 >= _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrLTE)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_lte>(_oprd1 <= _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrLAND)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_land>(_oprd1 && _oprd2);
}

TEST_F(process_binary_arithmetic_instrs_test, TestInstrLOR)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_lor>(_oprd1 || _oprd2);
}
