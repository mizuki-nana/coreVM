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


class process_native_type_instrs_test : public process_instr_integration_test {};


class process_native_type_creation_instrs_test : public process_native_type_instrs_test {
public:
  virtual void SetUp () {
    _process.push_frame(_frame);
  }

  template<typename InstrHandlerCls, typename IntrinsicType>
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
};


TEST_F(process_native_type_creation_instrs_test, TestInstrINT8)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_int8, int8_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrUINT8)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_uint8, uint8_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrINT16)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_int16, int16_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrUINT16)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_uint16, uint16_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrINT32)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_int32, int32_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrUINT32)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_uint32, uint32_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrINT64)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_int64, int64_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrUINT64)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_uint64, uint64_t>(0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrBOOL)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_bool, bool>(true);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrDEC1)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_dec1, float>(0.0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrDEC2)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_dec2, double>(0.0);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrSTR)
{
  corevm::types::native_string expected_result;
  execute_instr_and_assert_result<corevm::runtime::instr_handler_str, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrARY)
{
  corevm::types::native_array expected_result;
  execute_instr_and_assert_result<corevm::runtime::instr_handler_ary, corevm::types::native_array>(expected_result);
}

TEST_F(process_native_type_creation_instrs_test, TestInstrMAP)
{
  corevm::types::native_map expected_result;
  execute_instr_and_assert_result<corevm::runtime::instr_handler_map, corevm::types::native_map>(expected_result);
}


class process_native_type_conversion_instrs_test : public process_native_type_instrs_test {
public:
  virtual void SetUp () {
    corevm::types::native_type_handle hndl = corevm::types::uint8(_oprd);
    _frame.push_eval_stack(hndl);
    _process.push_frame(_frame);
  }

  template<typename InstrHandlerCls>
  void execute_instr_and_assert_result() {
    // TODO: should have more concrete testing on the conversion.
    InstrHandlerCls instr_handler;

    corevm::runtime::instr instr;

    instr_handler.execute(instr, _process);

    corevm::runtime::frame& frame = _process.top_frame();
    ASSERT_EQ(1, frame.eval_stack_size());
  }

private:
  corevm::types::uint8 _oprd = 100;
};


TEST_F(process_native_type_conversion_instrs_test, TestInstr2INT8)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2int8>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2INT16)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2int16>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2UINT16)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2uint16>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2INT32)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2int32>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2UINT64)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2uint64>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2INT64)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2int64>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2BOOL)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2bool>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2DEC1)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2dec1>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2DEC2)
{
  execute_instr_and_assert_result<corevm::runtime::instr_handler_2dec2>();
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2STR)
{
  _ASSERT_THROW(
    { execute_instr_and_assert_result<corevm::runtime::instr_handler_2str>(); },
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2ARY)
{
  _ASSERT_THROW(
    { execute_instr_and_assert_result<corevm::runtime::instr_handler_2ary>(); },
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(process_native_type_conversion_instrs_test, TestInstr2MAP)
{
  _ASSERT_THROW(
    { execute_instr_and_assert_result<corevm::runtime::instr_handler_2map>(); },
    corevm::types::corevm_native_type_conversion_error
  );
}


class process_native_type_complex_instrs_test : public process_native_type_instrs_test {
public:
  virtual void SetUp() {
    // Do nothing here.
  }

  template<typename InstrHandlerCls, typename IntrinsicType>
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
};


class process_native_type_complex_instrs_for_str_type_test : public process_native_type_complex_instrs_test {};


TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRLEN)
{
  corevm::types::native_string hello_world = "Hello world!";
  corevm::types::native_string::size_type expected_result = hello_world.size();
  corevm::types::native_type_handle oprd = corevm::types::string(hello_world);
  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strlen, corevm::types::native_string::size_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRCLR)
{
  corevm::types::native_string hello_world = "Hello world!";
  corevm::types::native_string expected_result = "";
  corevm::types::native_type_handle oprd = hello_world;
  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strclr, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRAPD)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string exclamation = "!";
  corevm::types::native_string expected_result = "Hello world!";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = exclamation;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strapd, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRPSH)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 exclamation = static_cast<corevm::types::int8::value_type>('!');
  corevm::types::native_string expected_result = "Hello world!";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = exclamation;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strpsh, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRIST)
{
  corevm::types::native_string hello_world = "Helloworld";
  corevm::types::int8 pos = static_cast<corevm::types::int8::value_type>(5);
  corevm::types::native_string space = " ";
  corevm::types::native_string expected_result = "Hello world";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;
  corevm::types::native_type_handle oprd3 = space;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strist, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRIST2)
{
  corevm::types::native_string hello_world = "Helloworld";
  corevm::types::int8 pos = static_cast<corevm::types::int8::value_type>(5);
  corevm::types::int8 space = static_cast<corevm::types::int8::value_type>(' ');
  corevm::types::native_string expected_result = "Hello world";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;
  corevm::types::native_type_handle oprd3 = space;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strist2, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRERS)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = static_cast<corevm::types::int8::value_type>(5);
  corevm::types::native_string expected_result = "Hello";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strers, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRERS2)
{
  corevm::types::native_string hello_world = "Hello  world";
  corevm::types::int8 pos = static_cast<corevm::types::int8::value_type>(5);
  corevm::types::int8 len = static_cast<corevm::types::int8::value_type>(1);
  corevm::types::native_string expected_result = "Hello world";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;
  corevm::types::native_type_handle oprd3 = len;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strers2, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRRPLC)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = static_cast<corevm::types::int8::value_type>(5);
  corevm::types::int8 len = static_cast<corevm::types::int8::value_type>(6);
  corevm::types::native_string exclamation = "!!!!!!";
  corevm::types::native_string expected_result = "Hello!!!!!!";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;
  corevm::types::native_type_handle oprd3 = len;
  corevm::types::native_type_handle oprd4 = exclamation;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _frame.push_eval_stack(oprd4);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strrplc, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRSWP)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string okla = "OKLA!";
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = okla;
  corevm::types::native_string expected_result = okla;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strswp, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRSUB)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = 6;
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;
  corevm::types::native_string expected_result = "world";

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strsub, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrSTRSUB2)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::int8 pos = 6;
  corevm::types::int8 len = 2;
  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = pos;
  corevm::types::native_type_handle oprd3 = len;
  corevm::types::native_string expected_result = "wo";

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strsub2, corevm::types::native_string>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrFND)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = word;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strfnd, corevm::types::native_string::size_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrFND2)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::uint32 pos = 6;
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = word;
  corevm::types::native_type_handle oprd3 = pos;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strfnd2, corevm::types::native_string::size_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrRFND)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = word;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strrfnd, corevm::types::native_string::size_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_str_type_test, TestInstrRFND2)
{
  corevm::types::native_string hello_world = "Hello world";
  corevm::types::native_string word = "world";
  corevm::types::uint32 pos = 6;
  corevm::types::native_string::size_type expected_result = 6;

  corevm::types::native_type_handle oprd1 = hello_world;
  corevm::types::native_type_handle oprd2 = word;
  corevm::types::native_type_handle oprd3 = pos;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_strrfnd2, corevm::types::native_string::size_type>(expected_result);
}


class process_native_type_complex_instrs_for_array_type_test : public process_native_type_complex_instrs_test {};


TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYLEN)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::native_array::size_type expected_result = array.size();
  corevm::types::native_type_handle oprd = array;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_arylen, corevm::types::native_array::size_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYEMP)
{
  corevm::types::native_array array = {1, 2, 3};
  bool expected_result = false;
  corevm::types::native_type_handle oprd = array;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_aryemp, bool>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYAT)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::uint32 index = 1;
  corevm::types::native_array::value_type expected_result = 2;

  corevm::types::native_type_handle oprd1 = array;
  corevm::types::native_type_handle oprd2 = index;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_aryat, corevm::types::native_array::value_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYFRT)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::native_array::value_type expected_result = 1;
  corevm::types::native_type_handle oprd = array;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_aryfrt, corevm::types::native_array::value_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYBAK)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::native_array::value_type expected_result = 3;
  corevm::types::native_type_handle oprd = array;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_arybak, corevm::types::native_array::value_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYAPND)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::uint64 data = 4;
  corevm::types::native_array expected_result = {1, 2, 3, 4};

  corevm::types::native_type_handle oprd1 = array;
  corevm::types::native_type_handle oprd2 = data;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_aryapnd, corevm::types::native_array>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYPOP)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::native_array expected_result = {1, 2};
  corevm::types::native_type_handle oprd = array;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_arypop, corevm::types::native_array>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYSWP)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::native_array other = {4, 5, 6};
  corevm::types::native_array expected_result = other;

  corevm::types::native_type_handle oprd1 = array;
  corevm::types::native_type_handle oprd2 = other;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_aryswp, corevm::types::native_array>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_array_type_test, TestInstrARYCLR)
{
  corevm::types::native_array array = {1, 2, 3};
  corevm::types::native_array expected_result;

  corevm::types::native_type_handle oprd = array;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_aryclr, corevm::types::native_array>(expected_result);
}


class process_native_type_complex_instrs_for_map_type_test : public process_native_type_complex_instrs_test {};


TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPLEN)
{
  corevm::types::native_map map;
  map[1] = 1;

  corevm::types::native_map::size_type expected_result = 1;
  corevm::types::native_type_handle oprd = map;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_maplen, corevm::types::native_map::size_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPEMP)
{
  corevm::types::native_map map;
  map[1] = 1;

  bool expected_result = false;
  corevm::types::native_type_handle oprd = map;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_mapemp, bool>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPAT)
{
  corevm::types::native_map map;
  map[1] = 100;

  corevm::types::uint64 key = 1;
  corevm::types::native_map::mapped_type expected_result = 100;
  corevm::types::native_type_handle oprd1 = map;
  corevm::types::native_type_handle oprd2 = key;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_mapat, corevm::types::native_map::mapped_type>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPPUT)
{
  corevm::types::native_map map;
  corevm::types::uint64 key = 1;
  corevm::types::uint64 value = 11;
  corevm::types::native_map expected_result;
  expected_result[1] = 11;

  corevm::types::native_type_handle oprd1 = map;
  corevm::types::native_type_handle oprd2 = key;
  corevm::types::native_type_handle oprd3 = value;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _frame.push_eval_stack(oprd3);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_mapput, corevm::types::native_map>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPERS)
{
  corevm::types::native_map map;
  map[1] = 11;
  map[2] = 22;

  corevm::types::uint64 key = 1;
  corevm::types::native_map expected_result;
  expected_result[2] = 22;

  corevm::types::native_type_handle oprd1 = map;
  corevm::types::native_type_handle oprd2 = key;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_mapers, corevm::types::native_map>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPCLR)
{
  corevm::types::native_map map;
  map[1] = 11;
  map[2] = 222;

  corevm::types::native_map expected_result;
  corevm::types::native_type_handle oprd = map;

  _frame.push_eval_stack(oprd);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_mapclr, corevm::types::native_map>(expected_result);
}

TEST_F(process_native_type_complex_instrs_for_map_type_test, TestInstrMAPSWP)
{
  corevm::types::native_map map;
  map[1] = 11;
  map[2] = 22;

  corevm::types::native_map other;
  other[1] = 22;
  other[2] = 11;

  corevm::types::native_map expected_result;
  expected_result[1] = 22;
  expected_result[2] = 11;

  corevm::types::native_type_handle oprd1 = map;
  corevm::types::native_type_handle oprd2 = other;

  _frame.push_eval_stack(oprd1);
  _frame.push_eval_stack(oprd2);
  _process.push_frame(_frame);

  execute_instr_and_assert_result<corevm::runtime::instr_handler_mapswp, corevm::types::native_map>(expected_result);
}
