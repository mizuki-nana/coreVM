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
#ifndef COREVM_INSTRUCTION_H_
#define COREVM_INSTRUCTION_H_

#include "common.h"
#include "errors.h"
#include "instr_fwd.h"

#include <memory>
#include <iosfwd>
#include <string>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

/**
 * Forward declaration of `corevm::runtime::frame`.
 */
class frame;

// -----------------------------------------------------------------------------

/**
 * Forward declaration of `corevm::runtime::invocation_ctx`.
 */
class invocation_ctx;

// -----------------------------------------------------------------------------

typedef struct instr
{
  const corevm::runtime::instr_code code;
  const corevm::runtime::instr_oprd oprd1;
  const corevm::runtime::instr_oprd oprd2;
} instr;

// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream&, const corevm::runtime::instr&);

// -----------------------------------------------------------------------------

// Forward declaration of `corevm::runtime::process`.
class process;

// -----------------------------------------------------------------------------

class instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&,
    corevm::runtime::process&,
    corevm::runtime::frame**,
    corevm::runtime::invocation_ctx**) = 0;

protected:
  template<typename InterfaceFunc>
  static void execute_unary_operator_instr(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_binary_operator_instr(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename NativeType>
  static void execute_native_integer_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::frame*);

  template<typename NativeType>
  static void execute_native_floating_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::frame*);

  template<typename NativeType>
  static void execute_native_complex_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::frame*);

  template<typename InterfaceFunc>
  static void execute_native_type_conversion_instr(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_single_operand(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_single_operand_in_place(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_two_operands(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_two_operands_in_place(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_three_operands(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_three_operands_in_place(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_four_operands(
    corevm::runtime::frame*, InterfaceFunc);

  template<typename InterfaceFunc>
  static void execute_native_type_complex_instr_with_four_operands_in_place(
    corevm::runtime::frame*, InterfaceFunc);
};

// -----------------------------------------------------------------------------


/* ------------------------- Object instructions ---------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_new : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_ldobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_stobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_stobjn : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getattr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setattr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_delattr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_hasattr2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getattr2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setattr2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_delattr2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_pop : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_ldobj2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_stobj2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_delobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_delobj2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_gethndl : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_sethndl : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_gethndl2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_clrhndl : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_cpyhndl : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_cpyrepr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_istruthy : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_objeq : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_objneq : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setctx : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_cldobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setattrs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_rsetattrs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setattrs2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_putobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_swap : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setflgc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setfldel : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setflcall : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_setflmute : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ------------------------ Control instructions ---------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_pinvk : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_invk : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_rtrn : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_jmp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_jmpif : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_jmpr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_exc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_excobj : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_clrexc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_jmpexc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_exit: public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ------------------------- Function instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_putarg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_putkwarg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_putargs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_putkwargs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getarg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getkwarg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getargs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_getkwargs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_hasargs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* -------------------------- Runtime instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_gc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_debug : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_print: public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_swap2: public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* -------------------- Arithmetic and logic instructions --------------------*/


// -----------------------------------------------------------------------------

class instr_handler_pos : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_neg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_inc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_dec : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_abs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_sqrt : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_add : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_sub : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mul : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_div : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mod : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_pow : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_bnot : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_band : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_bor : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_bxor : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_bls : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_brs : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_eq : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_neq : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_gt : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_lt : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_gte : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_lte : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_lnot : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_land : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_lor : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_cmp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/*--------------------- Native type creation instructions ------------------- */


// -----------------------------------------------------------------------------

class instr_handler_int8 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_uint8 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_int16 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_uint16 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_int32 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_uint32 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_int64 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_uint64 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_bool : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_dec1 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_dec2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_str : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_ary : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_map : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ------------------ Native type conversion instructions ------------------- */


// -----------------------------------------------------------------------------

class instr_handler_2int8 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint8 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2int16 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint16 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2int32 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint32 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2int64 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint64 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2bool : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2dec1 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2dec2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2str : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2ary : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_2map : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ------------------ Native type manipulation instructions ----------------- */


// -----------------------------------------------------------------------------

class instr_handler_truthy : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_repr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_hash : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_slice : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_stride : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_reverse : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_round : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ---------------------- String type instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_strlen : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strat : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strclr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strapd : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strpsh : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strist : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strist2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strers : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strers2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strrplc : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strswp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strsub : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strsub2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strfnd : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strfnd2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strrfnd : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_strrfnd2 : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ----------------------- Array type instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_arylen : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryemp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryat : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryfrt : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_arybak : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryput : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryapnd : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryers : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_arypop : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryswp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_aryclr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_arymrg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------


/* ----------------------- Map type instructions ---------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_maplen : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapemp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapfind : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapat : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapput : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapset : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapers : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapclr : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapswp : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapkeys : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapvals : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

class instr_handler_mapmrg : public instr_handler
{
public:
  virtual void execute(
    const corevm::runtime::instr&, corevm::runtime::process&,
    corevm::runtime::frame**, corevm::runtime::invocation_ctx**);
};

// -----------------------------------------------------------------------------

typedef struct instr_handler_wrapper
{
  const std::shared_ptr<instr_handler> handler;
} instr_handler_wrapper;

// -----------------------------------------------------------------------------

class instr_handler_meta
{
public:
  static const corevm::runtime::instr_handler_wrapper instr_handlers[INSTR_CODE_MAX];

};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTRUCTION_H_ */
