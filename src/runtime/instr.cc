#include <stdexcept>
#include <boost/format.hpp>
#include "../../include/runtime/instr.h"
#include "../../include/runtime/process.h"
#include "../../include/types/interfaces.h"


corevm::runtime::instr_info
corevm::runtime::instr_handler_meta::_validate_instr(
  const corevm::runtime::instr& instr) throw(corevm::runtime::invalid_instr_error)
{
  corevm::runtime::instr_enum instr_code = static_cast<corevm::runtime::instr_enum>(instr.code);
  try {
    return this->_instr_info_map.at(instr_code);
  } catch (...) { // TODO: suppose to catch std::out_of_range
    throw corevm::runtime::invalid_instr_error();
  }
}

std::string
corevm::runtime::instr_handler_meta::instr_to_string(const corevm::runtime::instr& instr)
{
  corevm::runtime::instr_info instr_info = this->_validate_instr(instr);

  if(instr_info.num_oprd == 1) {
    return str(boost::format("<%d %d __>") % instr.code % instr.oprd1);
  } else {
    return str(boost::format("<%d %d %d>") % instr.code % instr.oprd1 % instr.oprd2);
  }
}


/*************************** INSTRUCTION HANDLERS *****************************/

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_unary_operator_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle oprd = frame.pop_eval_stack();

  corevm::types::native_type_handle result;

  interface_func(oprd, result);

  frame.push_eval_stack(result);
}

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_binary_operator_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle lhs = frame.pop_eval_stack();
  corevm::types::native_type_handle rhs = frame.pop_eval_stack();

  corevm::types::native_type_handle result;

  interface_func(lhs, rhs, result);

  frame.push_eval_stack(result);
}

template<typename NativeType>
void
corevm::runtime::instr_handler::_execute_native_type_creation_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = NativeType();

  frame.push_eval_stack(hndl);
}

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle oprd = frame.pop_eval_stack();
  corevm::types::native_type_handle result;

  interface_func(oprd, result);

  frame.push_eval_stack(result);
}

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_native_type_complex_instr_1(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle oprd = frame.pop_eval_stack();
  corevm::types::native_type_handle result;

  interface_func(oprd, result);

  frame.push_eval_stack(result);
}

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle oprd1 = frame.pop_eval_stack();
  corevm::types::native_type_handle oprd2 = frame.pop_eval_stack();
  corevm::types::native_type_handle result;

  interface_func(oprd2, oprd1, result);

  frame.push_eval_stack(result);
}

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle oprd1 = frame.pop_eval_stack();
  corevm::types::native_type_handle oprd2 = frame.pop_eval_stack();
  corevm::types::native_type_handle oprd3 = frame.pop_eval_stack();
  corevm::types::native_type_handle result;

  interface_func(oprd3, oprd2, oprd1, result);

  frame.push_eval_stack(result);
}

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::_execute_native_type_complex_instr_4(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle oprd1 = frame.pop_eval_stack();
  corevm::types::native_type_handle oprd2 = frame.pop_eval_stack();
  corevm::types::native_type_handle oprd3 = frame.pop_eval_stack();
  corevm::types::native_type_handle oprd4 = frame.pop_eval_stack();
  corevm::types::native_type_handle result;

  interface_func(oprd4, oprd3, oprd2, oprd1, result);

  frame.push_eval_stack(result);
}

void
corevm::runtime::instr_handler_new::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_lbobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_stobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_getattr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_setattr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_pop::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_ldobj2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_stobj2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_delobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_delobj2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_gethndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_sethndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_clrhndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_objeq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_objneq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_rtrn::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_jmp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_jmpif::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_exc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_exc2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_exit::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_call::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_putarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_putkwarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_getarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_getkwarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_getargs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_getkwargs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_pos::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_positive_operator
  );
}

void
corevm::runtime::instr_handler_neg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_negation_operator
  );
}

void
corevm::runtime::instr_handler_inc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_increment_operator
  );
}

void
corevm::runtime::instr_handler_dec::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_decrement_operator
  );
}

void
corevm::runtime::instr_handler_add::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_addition_operator
  );
}

void
corevm::runtime::instr_handler_sub::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_subtraction_operator
  );
}

void
corevm::runtime::instr_handler_mul::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_multiplication_operator
  );
}

void
corevm::runtime::instr_handler_div::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_division_operator
  );
}

void
corevm::runtime::instr_handler_mod::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_modulus_operator
  );
}

void
corevm::runtime::instr_handler_pow::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_bnot::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_not_operator
  );
}

void
corevm::runtime::instr_handler_band::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_and_operator
  );
}

void
corevm::runtime::instr_handler_bor::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_or_operator
  );
}

void
corevm::runtime::instr_handler_bxor::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_xor_operator
  );
}

void
corevm::runtime::instr_handler_bls::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_left_shift_operator
  );
}

void
corevm::runtime::instr_handler_brs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_right_shift_operator
  );
}

void
corevm::runtime::instr_handler_eq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_eq_operator
  );
}

void
corevm::runtime::instr_handler_neq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_neq_operator
  );
}

void
corevm::runtime::instr_handler_gt::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_gt_operator
  );
}

void
corevm::runtime::instr_handler_lt::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_lt_operator
  );
}

void
corevm::runtime::instr_handler_gte::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_gte_operator
  );
}

void
corevm::runtime::instr_handler_lte::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_lte_operator
  );
}

void
corevm::runtime::instr_handler_lnot::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_logical_not_operator
  );
}

void
corevm::runtime::instr_handler_land::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_logical_and_operator
  );
}

void
corevm::runtime::instr_handler_lor::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_logical_or_operator
  );
}

void
corevm::runtime::instr_handler_int8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::int8>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_uint8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::uint8>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_int16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::int16>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_uint16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::uint16>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_int32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::int32>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_uint32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::uint32>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_int64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::int64>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_uint64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::uint64>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_bool::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::boolean>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_dec1::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::decimal>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_dec2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::decimal2>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_str::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::string>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_ary::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::array>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_map::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_creation_instr<corevm::types::map>(
    instr,
    process
  );
}

void
corevm::runtime::instr_handler_2int8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int8
  );
}

void
corevm::runtime::instr_handler_2uint8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint8
  );
}

void
corevm::runtime::instr_handler_2int16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int16
  );
}

void
corevm::runtime::instr_handler_2uint16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint16
  );
}

void
corevm::runtime::instr_handler_2int32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int32
  );
}

void
corevm::runtime::instr_handler_2uint32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint32
  );
}

void
corevm::runtime::instr_handler_2int64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int64
  );
}

void
corevm::runtime::instr_handler_2uint64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint64
  );
}

void
corevm::runtime::instr_handler_2bool::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_bool
  );
}

void
corevm::runtime::instr_handler_2dec1::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_dec1
  );
}

void
corevm::runtime::instr_handler_2dec2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_dec2
  );
}

void
corevm::runtime::instr_handler_2str::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_str
  );
}

void
corevm::runtime::instr_handler_2ary::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_ary
  );
}

void
corevm::runtime::instr_handler_2map::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_map
  );
}

void
corevm::runtime::instr_handler_strlen::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_1(
    instr,
    process,
    corevm::types::interface_string_get_size
  );
}

void
corevm::runtime::instr_handler_strclr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_1(
    instr,
    process,
    corevm::types::interface_string_clear
  );
}

void
corevm::runtime::instr_handler_strapd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_append
  );
}

void
corevm::runtime::instr_handler_strpsh::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_pushback
  );
}

void
corevm::runtime::instr_handler_strist::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
    instr,
    process,
    corevm::types::interface_string_insert_str
  );
}

void
corevm::runtime::instr_handler_strist2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
    instr,
    process,
    corevm::types::interface_string_insert_char
  );
}

void
corevm::runtime::instr_handler_strers::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_erase
  );
}

void
corevm::runtime::instr_handler_strers2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
    instr,
    process,
    corevm::types::interface_string_erase2
  );
}

void
corevm::runtime::instr_handler_strrplc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_4(
    instr,
    process,
    corevm::types::interface_string_replace_str
  );
}

void
corevm::runtime::instr_handler_strswp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_swap
  );
}

void
corevm::runtime::instr_handler_strsub::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_substr
  );
}

void
corevm::runtime::instr_handler_strsub2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
    instr,
    process,
    corevm::types::interface_string_substr2
  );
}

void
corevm::runtime::instr_handler_strfnd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_find
  );
}

void
corevm::runtime::instr_handler_strfnd2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
    instr,
    process,
    corevm::types::interface_string_find2
  );
}

void
corevm::runtime::instr_handler_strrfnd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_2(
    instr,
    process,
    corevm::types::interface_string_rfind
  );
}

void
corevm::runtime::instr_handler_strrfnd2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::_execute_native_type_complex_instr_3(
    instr,
    process,
    corevm::types::interface_string_rfind2
  );
}

void
corevm::runtime::instr_handler_arylen::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_aryemp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_aryat::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_aryfrt::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_arybak::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_aryapnd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_arypop::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_aryswp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_aryclr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_maplen::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_mapemp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_mapat::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_mapput::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_mapers::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_mapclr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}

void
corevm::runtime::instr_handler_mapswp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // TODO: to be implemented.
}
