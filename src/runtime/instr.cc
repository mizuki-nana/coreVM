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
#include "instr.h"

#include "catch_site.h"
#include "dbgmem_printer.h"
#include "frame_printer.h"
#include "process.h"
#include "process_printer.h"
#include "utils.h"
#include "corevm/macros.h"
#include "dyobj/util.h"
#include "types/interfaces.h"
#include "types/native_type_handle.h"

#include <algorithm>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/format.hpp>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc99-extensions"
#endif

InstrHandler*
InstrHandlerMeta::instr_handlers[INSTR_CODE_MAX] {

  /* -------------------------- Object instructions ------------------------- */

  /* NEW       */    instr_handler_new       ,
  /* LDOBJ     */    instr_handler_ldobj     ,
  /* STOBJ     */    instr_handler_stobj     ,
  /* STOBJN    */    instr_handler_stobjn    ,
  /* GETATTR   */    instr_handler_getattr   ,
  /* SETATTR   */    instr_handler_setattr   ,
  /* DELATTR   */    instr_handler_delattr   ,
  /* HASATTR2  */    instr_handler_hasattr2  ,
  /* GETATTR2  */    instr_handler_getattr2  ,
  /* SETATTR2  */    instr_handler_setattr2  ,
  /* DELATTR2  */    instr_handler_delattr2  ,
  /* POP       */    instr_handler_pop       ,
  /* LDOBJ2    */    instr_handler_ldobj2    ,
  /* STOBJ2    */    instr_handler_stobj2    ,
  /* DELOBJ    */    instr_handler_delobj    ,
  /* DELOBJ2   */    instr_handler_delobj2   ,
  /* GETHNDL   */    instr_handler_gethndl   ,
  /* SETHNDL   */    instr_handler_sethndl   ,
  /* GETHNDL2  */    instr_handler_gethndl2  ,
  /* CLRHNDL   */    instr_handler_clrhndl   ,
  /* CPYHNDL   */    instr_handler_cpyhndl   ,
  /* CPYREPR   */    instr_handler_cpyrepr   ,
  /* ISTRUTHY  */    instr_handler_istruthy  ,
  /* OBJEQ     */    instr_handler_objeq     ,
  /* OBJNEQ    */    instr_handler_objneq    ,
  /* SETCTX    */    instr_handler_setctx    ,
  /* CLDOBJ    */    instr_handler_cldobj    ,
  /* SETATTRS  */    instr_handler_setattrs  ,
  /* RSETATTRS */    instr_handler_rsetattrs ,
  /* SETATTRS2  */   instr_handler_setattrs2 ,
  /* PUTOBJ    */    instr_handler_putobj    ,
  /* GETOBJ    */    instr_handler_getobj    ,
  /* SWAP      */    instr_handler_swap      ,
  /* SETFLGC   */    instr_handler_setflgc   ,
  /* SETFLDEL  */    instr_handler_setfldel  ,
  /* SETFLCALL */    instr_handler_setflcall ,
  /* SETFLMUTE */    instr_handler_setflmute ,

  /* -------------------------- Control instructions ------------------------ */

  /* PINVK     */    instr_handler_pinvk     ,
  /* INVK      */    instr_handler_invk      ,
  /* RTRN      */    instr_handler_rtrn      ,
  /* JMP       */    instr_handler_jmp       ,
  /* JMPIF     */    instr_handler_jmpif     ,
  /* JMPR      */    instr_handler_jmpr      ,
  /* EXC       */    instr_handler_exc       ,
  /* EXCOBJ    */    instr_handler_excobj    ,
  /* CLREXC    */    instr_handler_clrexc    ,
  /* JMPEXC    */    instr_handler_jmpexc    ,
  /* EXIT      */    instr_handler_exit      ,

  /* ------------------------- Function instructions ------------------------ */

  /* PUTARG    */    instr_handler_putarg    ,
  /* PUTKWARG  */    instr_handler_putkwarg  ,
  /* PUTARGS   */    instr_handler_putargs   ,
  /* PUTKWARGS */    instr_handler_putkwargs ,
  /* GETARG    */    instr_handler_getarg    ,
  /* GETKWARG  */    instr_handler_getkwarg  ,
  /* GETARGS   */    instr_handler_getargs   ,
  /* GETKWARGS */    instr_handler_getkwargs ,
  /* HASARGS   */    instr_handler_hasargs   ,

  /* ------------------------- Runtime instructions ------------------------- */

  /* GC        */    instr_handler_gc        ,
  /* DEBUG     */    instr_handler_debug     ,
  /* DBGFRM    */    instr_handler_dbgfrm    ,
  /* DBGMEM    */    instr_handler_dbgmem    ,
  /* DBGVAR    */    instr_handler_dbgvar    ,
  /* PRINT     */    instr_handler_print     ,
  /* SWAP2     */    instr_handler_swap2     ,

  /* ---------------- Arithmetic and logic instructions --------------------- */

  /* POS      */     instr_handler_pos       ,
  /* NEG      */     instr_handler_neg       ,
  /* INC      */     instr_handler_inc       ,
  /* DEC      */     instr_handler_dec       ,
  /* ABS      */     instr_handler_abs       ,
  /* SQRT     */     instr_handler_sqrt      ,
  /* ADD      */     instr_handler_add       ,
  /* SUB      */     instr_handler_sub       ,
  /* MUL      */     instr_handler_mul       ,
  /* DIV      */     instr_handler_div       ,
  /* MOD      */     instr_handler_mod       ,
  /* POW      */     instr_handler_pow       ,
  /* BNOT     */     instr_handler_bnot      ,
  /* BAND     */     instr_handler_band      ,
  /* BOR      */     instr_handler_bor       ,
  /* BXOR     */     instr_handler_bxor      ,
  /* BLS      */     instr_handler_bls       ,
  /* BRS      */     instr_handler_brs       ,
  /* EQ       */     instr_handler_eq        ,
  /* NEQ      */     instr_handler_neq       ,
  /* GT       */     instr_handler_gt        ,
  /* LT       */     instr_handler_lt        ,
  /* GTE      */     instr_handler_gte       ,
  /* LTE      */     instr_handler_lte       ,
  /* LNOT     */     instr_handler_lnot      ,
  /* LAND     */     instr_handler_land      ,
  /* LOR      */     instr_handler_lor       ,
  /* CMP      */     instr_handler_cmp       ,

  /* ----------------- Native type creation instructions -------------------- */

  /* INT8     */     instr_handler_int8      ,
  /* UINT8    */     instr_handler_uint8     ,
  /* INT16    */     instr_handler_int16     ,
  /* UINT16   */     instr_handler_uint16    ,
  /* INT32    */     instr_handler_int32     ,
  /* UINT32   */     instr_handler_uint32    ,
  /* INT64    */     instr_handler_int64     ,
  /* UINT64   */     instr_handler_uint64    ,
  /* BOOL     */     instr_handler_bool      ,
  /* DEC1     */     instr_handler_dec1      ,
  /* DEC2     */     instr_handler_dec2      ,
  /* STR      */     instr_handler_str       ,
  /* ARY      */     instr_handler_ary       ,
  /* MAP      */     instr_handler_map       ,

  /* ----------------- Native type conversion instructions ------------------ */

  /* TOINT8   */     instr_handler_2int8     ,
  /* TOUINT8  */     instr_handler_2uint8    ,
  /* TOINT16  */     instr_handler_2int16    ,
  /* TOUINT16 */     instr_handler_2uint16   ,
  /* TOINT32  */     instr_handler_2int32    ,
  /* TOUINT32 */     instr_handler_2uint32   ,
  /* TOINT64  */     instr_handler_2int64    ,
  /* TOUINT64 */     instr_handler_2uint64   ,
  /* TOBOOL   */     instr_handler_2bool     ,
  /* TODEC1   */     instr_handler_2dec1     ,
  /* TODEC2   */     instr_handler_2dec2     ,
  /* TOSTR    */     instr_handler_2str      ,
  /* TOARY    */     instr_handler_2ary      ,
  /* TOMAP    */     instr_handler_2map      ,

  /* ----------------- Native type manipulation instructions ---------------- */

  /* TRUTHY   */     instr_handler_truthy    ,
  /* REPR     */     instr_handler_repr      ,
  /* HASH     */     instr_handler_hash      ,
  /* SLICE    */     instr_handler_slice     ,
  /* STRIDE   */     instr_handler_stride    ,
  /* REVERSE  */     instr_handler_reverse   ,
  /* ROUND    */     instr_handler_round     ,

  /* --------------------- String type instructions ------------------------- */

  /* STRLEN   */     instr_handler_strlen    ,
  /* STRAT    */     instr_handler_strat     ,
  /* STRCLR   */     instr_handler_strclr    ,
  /* STRAPD   */     instr_handler_strapd    ,
  /* STRPSH   */     instr_handler_strpsh    ,
  /* STRIST   */     instr_handler_strist    ,
  /* STRIST2  */     instr_handler_strist2   ,
  /* STRERS   */     instr_handler_strers    ,
  /* STRERS2  */     instr_handler_strers2   ,
  /* STRRPLC  */     instr_handler_strrplc   ,
  /* STRSWP   */     instr_handler_strswp    ,
  /* STRSUB   */     instr_handler_strsub    ,
  /* STRSUB2  */     instr_handler_strsub2   ,
  /* STRFND   */     instr_handler_strfnd    ,
  /* STRFND2  */     instr_handler_strfnd2   ,
  /* STRRFND  */     instr_handler_strrfnd   ,
  /* STRRFND2 */     instr_handler_strrfnd2  ,

  /* --------------------- Array type instructions -------------------------- */

  /* ARYLEN   */     instr_handler_arylen    ,
  /* ARYEMP   */     instr_handler_aryemp    ,
  /* ARYAT    */     instr_handler_aryat     ,
  /* ARYFRT   */     instr_handler_aryfrt    ,
  /* ARYBAK   */     instr_handler_arybak    ,
  /* ARYPUT   */     instr_handler_aryput    ,
  /* ARYAPND  */     instr_handler_aryapnd   ,
  /* ARYERS   */     instr_handler_aryers    ,
  /* ARYPOP   */     instr_handler_arypop    ,
  /* ARYSWP   */     instr_handler_aryswp    ,
  /* ARYCLR   */     instr_handler_aryclr    ,
  /* ARYMRG   */     instr_handler_arymrg    ,

  /* --------------------- Map type instructions ---------------------------- */

  /* MAPLEN   */     instr_handler_maplen    ,
  /* MAPEMP   */     instr_handler_mapemp    ,
  /* MAPFIND  */     instr_handler_mapfind   ,
  /* MAPAT    */     instr_handler_mapat     ,
  /* MAPPUT   */     instr_handler_mapput    ,
  /* MAPSET   */     instr_handler_mapset    ,
  /* MAPERS   */     instr_handler_mapers    ,
  /* MAPCLR   */     instr_handler_mapclr    ,
  /* MAPSWP   */     instr_handler_mapswp    ,
  /* MAPKEYS  */     instr_handler_mapkeys   ,
  /* MAPVALS  */     instr_handler_mapvals   ,
  /* MAPMRG   */     instr_handler_mapmrg

};

#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif  /* #if defined(__clang__) and __clang__ */

// -----------------------------------------------------------------------------


/* --------------------------- INSTRUCTION HANDLERS ------------------------- */


// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_unary_operator_instr(Frame* frame, InterfaceFunc interface_func)
{
  types::NativeTypeHandle& oprd = frame->top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_binary_operator_instr(Frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& lhs = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& rhs = frame->eval_stack_element(eval_stack_size - 2);

  lhs = interface_func(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename NativeType>
static
void
execute_native_integer_type_creation_instr(const Instr& instr, Frame* frame)
{
  types::NativeTypeHandle hndl(NativeType(instr.oprd1));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename NativeType>
static
void
execute_native_floating_type_creation_instr(const Instr& instr, Frame* frame)
{
  const Compartment* compartment = frame->compartment();

  auto encoding_key = static_cast<encoding_key_t>(instr.oprd1);
  auto fpt_literal = static_cast<NativeType>(compartment->get_fpt_literal(encoding_key));

  types::NativeTypeHandle hndl(fpt_literal);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename NativeType>
static
void
execute_native_complex_type_creation_instr(
  const Instr& /* instr */, Frame* frame)
{
  NativeType value;
  types::NativeTypeHandle hndl(value);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_conversion_instr(Frame* frame, InterfaceFunc interface_func)
{
  types::NativeTypeHandle& oprd = frame->top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_single_operand(
  Frame* frame, InterfaceFunc interface_func)
{
  types::NativeTypeHandle& oprd = frame->top_eval_stack();

  oprd = interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_single_operand_in_place(
  Frame* frame, InterfaceFunc interface_func)
{
  types::NativeTypeHandle& oprd = frame->top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_two_operands(
  Frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);

  oprd1 = interface_func(oprd1, oprd2);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_two_operands_in_place(Frame* frame,
  InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);

  interface_func(oprd1, oprd2);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_three_operands(Frame* frame,
  InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 3)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::NativeTypeHandle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);

  oprd1 = interface_func(oprd1, oprd2, oprd3);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_three_operands_in_place(Frame* frame,
  InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 3)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::NativeTypeHandle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);

  interface_func(oprd1, oprd2, oprd3);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_four_operands(Frame* frame,
  InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 4)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::NativeTypeHandle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);
  types::NativeTypeHandle& oprd4 = frame->eval_stack_element(eval_stack_size - 4);

  oprd1 = interface_func(oprd1, oprd2, oprd3, oprd4);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
static
void
execute_native_type_complex_instr_with_four_operands_in_place(Frame* frame,
  InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 4)
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::NativeTypeHandle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::NativeTypeHandle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);
  types::NativeTypeHandle& oprd4 = frame->eval_stack_element(eval_stack_size - 4);

  interface_func(oprd1, oprd2, oprd3, oprd4);
}

// -----------------------------------------------------------------------------

void
instr_handler_new(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.create_dyobj();
  process.push_stack(obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_ldobj(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  Process::dyobj_ptr obj = NULL;

  if (frame->get_visible_var_through_ancestry(key, &obj))
  {
#if __DEBUG__
    ASSERT(obj);
#endif

    process.push_stack(obj);
  }
  else
  {
    std::string name;
    const auto encoding_key = static_cast<encoding_key_t>(key);
    frame->compartment()->get_string_literal(encoding_key, &name);
    THROW(NameNotFoundError(name.c_str()));
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_stobj(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);

  Frame* frame = *frame_ptr;

  auto obj = process.pop_stack();

  obj->manager().on_setattr();

  frame->set_visible_var(key, obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_stobjn(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  size_t n = static_cast<size_t>(instr.oprd2);

  Frame& frame = process.top_nth_frame(n);

  auto obj = process.pop_stack();

  obj->manager().on_setattr();

  frame.set_visible_var(key, obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_getattr(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto str_key = static_cast<encoding_key_t>(instr.oprd1);
  auto frame = *frame_ptr;

  auto obj = process.pop_stack();

  auto attr_obj = getattr(obj, frame->compartment(), str_key);

  process.push_stack(attr_obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_setattr(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto str_key = static_cast<encoding_key_t>(instr.oprd1);
  auto frame = *frame_ptr;
  dyobj::attr_key_t attr_key = get_attr_key(frame->compartment(), str_key);

  auto attr_obj = process.pop_stack();
  auto target_obj = process.top_stack();

  if (target_obj->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE))
  {
    THROW(InvalidOperationError(
      str(boost::format("cannot mutate immutable object 0x%08x") % target_obj->id()).c_str()));
  }

  target_obj->putattr(attr_key, attr_obj);
  attr_obj->manager().on_setattr();
}

// -----------------------------------------------------------------------------

void
instr_handler_delattr(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  dyobj::attr_key_t attr_key = static_cast<dyobj::attr_key_t>(instr.oprd1);

  auto obj = process.pop_stack();

  if (obj->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE))
  {
    THROW(InvalidOperationError(
      str(boost::format("cannot mutate immutable object 0x%08x") % obj->id()).c_str()));
  }

  auto attr_obj = obj->getattr(attr_key);
  attr_obj->manager().on_delattr();
  obj->delattr(attr_key);

  process.push_stack(obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_hasattr2(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  const auto frame = *frame_ptr;
  types::NativeTypeHandle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::attr_key_t attr_key = dyobj::hash_attr_str(attr_str);

  const bool res_value = obj->hasattr(attr_key);

  types::NativeTypeHandle res( (types::boolean(res_value)) );

  frame->push_eval_stack(std::move(res));
}

// -----------------------------------------------------------------------------

void
instr_handler_getattr2(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.pop_stack();

  const auto frame = *frame_ptr;
  types::NativeTypeHandle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  auto attr_obj = getattr(obj, attr_str_value);

  process.push_stack(attr_obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_setattr2(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto attr_obj = process.pop_stack();
  auto target_obj = process.top_stack();

  const auto frame = *frame_ptr;
  types::NativeTypeHandle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::attr_key_t attr_key = dyobj::hash_attr_str(attr_str);

  target_obj->putattr(attr_key, attr_obj);

  attr_obj->manager().on_setattr();
}

// -----------------------------------------------------------------------------

void
instr_handler_delattr2(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  const auto frame = *frame_ptr;
  types::NativeTypeHandle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::attr_key_t attr_key = dyobj::hash_attr_str(attr_str);

  obj->delattr(attr_key);
}

// -----------------------------------------------------------------------------

void
instr_handler_pop(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  process.pop_stack();
}

// -----------------------------------------------------------------------------

void
instr_handler_ldobj2(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  Process::dyobj_ptr obj = NULL;

  if (frame->get_invisible_var_through_ancestry(key, &obj))
  {
#if __DEBUG__
    ASSERT(obj);
#endif

    process.push_stack(obj);
  }
  else
  {
    std::string name;
    const auto encoding_key = static_cast<encoding_key_t>(key);
    frame->compartment()->get_string_literal(encoding_key, &name);
    THROW(NameNotFoundError(name.c_str()));
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_stobj2(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);

  Frame* frame = *frame_ptr;
  auto obj = process.pop_stack();

  frame->set_invisible_var(key, obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_delobj(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  Frame* frame = *frame_ptr;

  auto obj = frame->pop_visible_var(key);

  if (obj->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_INDELIBLE))
  {
    THROW(ObjectDeletionError(obj->id()));
  }

  obj->manager().on_delete();
}

// -----------------------------------------------------------------------------

void
instr_handler_delobj2(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  Frame* frame = *frame_ptr;

  auto obj = frame->pop_invisible_var(key);

  if (obj->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_INDELIBLE))
  {
    THROW(ObjectDeletionError(obj->id()));
  }

  obj->manager().on_delete();
}

// -----------------------------------------------------------------------------

void
instr_handler_gethndl(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  auto obj = process.top_stack();

  if (!obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  frame->push_eval_stack(obj->ntvhndl());
}

// -----------------------------------------------------------------------------

void
instr_handler_sethndl(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  types::NativeTypeHandle hndl(std::move(frame->pop_eval_stack()));

  auto obj = process.top_stack();

  if (!obj->has_ntvhndl())
  {
    auto new_ntvhndl = process.insert_ntvhndl(hndl);

    obj->set_ntvhndl(new_ntvhndl);
  }
  else
  {
    process.get_ntvhndl(&obj->ntvhndl()) = std::move(hndl);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_gethndl2(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);

  auto obj = frame->get_visible_var(key);

  if (!obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  frame->push_eval_stack(obj->ntvhndl());
}

// -----------------------------------------------------------------------------

void
instr_handler_clrhndl(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  if (!obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  process.erase_ntvhndl(&obj->ntvhndl());
  obj->clear_ntvhndl();
}

// -----------------------------------------------------------------------------

void
instr_handler_cpyhndl(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto src_obj = process.pop_stack();
  auto target_obj = process.pop_stack();

  if (!src_obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  types::NativeTypeHandle res = src_obj->ntvhndl();

  uint32_t type = static_cast<uint32_t>(instr.oprd1);

  switch (type)
  {
    case 1:
    {
      types::interface_to_int8(res);
      break;
    }
    case 2:
    {
      types::interface_to_uint8(res);
      break;
    }
    case 3:
    {
      types::interface_to_int16(res);
      break;
    }
    case 4:
    {
      types::interface_to_uint16(res);
      break;
    }
    case 5:
    {
      types::interface_to_int32(res);
      break;
    }
    case 6:
    {
      types::interface_to_uint32(res);
      break;
    }
    case 7:
    {
      types::interface_to_int64(res);
      break;
    }
    case 8:
    {
      types::interface_to_uint64(res);
      break;
    }
    case 9:
    {
      types::interface_to_bool(res);
      break;
    }
    case 10:
    {
      types::interface_to_dec1(res);
      break;
    }
    case 11:
    {
      types::interface_to_dec2(res);
      break;
    }
    case 12:
    {
      types::interface_to_str(res);
      break;
    }
    case 13:
    {
      types::interface_to_ary(res);
      break;
    }
    case 14:
    {
      types::interface_to_map(res);
      break;
    }
    default:
      // Ignore invalid type.
      break;
  }

  auto new_ntvhndl = process.insert_ntvhndl(res);
  target_obj->set_ntvhndl(new_ntvhndl);
}

// -----------------------------------------------------------------------------

void
instr_handler_cpyrepr(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  Process::dyobj_ptr src_obj = process.pop_stack();
  Process::dyobj_ptr target_obj = process.pop_stack();

  if (!src_obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  const types::NativeTypeHandle& hndl = src_obj->ntvhndl();
  auto res = types::interface_compute_repr_value(hndl);

  auto new_ntvhndl = process.insert_ntvhndl(res);
  target_obj->set_ntvhndl(new_ntvhndl);
}

// -----------------------------------------------------------------------------

void
instr_handler_istruthy(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  auto obj = process.top_stack();

  if (!obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  const types::NativeTypeHandle& hndl = obj->ntvhndl();

  auto res = types::interface_compute_truthy_value(hndl);

  frame->push_eval_stack(std::move(res));
}

// -----------------------------------------------------------------------------

void
instr_handler_objeq(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj1 = process.pop_stack();
  auto obj2 = process.pop_stack();

  types::NativeTypeHandle hndl(types::boolean(obj1 == obj2));

  Frame* frame = *frame_ptr;
  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_objneq(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj1 = process.pop_stack();
  auto obj2 = process.pop_stack();

  types::NativeTypeHandle hndl(types::boolean(obj1 != obj2));

  Frame* frame = *frame_ptr;
  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_setctx(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  Frame* frame = *frame_ptr;
  ClosureCtx frame_cls = frame->closure_ctx();

  ClosureCtx ctx(frame_cls.compartment_id, static_cast<closure_id_t>(instr.oprd1));

  obj->set_closure_ctx(ctx);
}

// -----------------------------------------------------------------------------

void
instr_handler_cldobj(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  types::NativeTypeHandle hndl = frame->pop_eval_stack();

  bool value = types::get_value_from_handle<bool>(hndl);

  variable_key_t key1 = static_cast<variable_key_t>(instr.oprd1);
  variable_key_t key2 = static_cast<variable_key_t>(instr.oprd2);

  variable_key_t key = value ? key1 : key2;

  Process::dyobj_ptr obj = NULL;

  while (!frame->get_visible_var_fast(key, &obj))
  {
    frame = frame->parent();

    if (!frame)
    {
      std::string name;
      const auto encoding_key = static_cast<encoding_key_t>(key);
      (*frame_ptr)->compartment()->get_string_literal(encoding_key, &name);
      THROW(NameNotFoundError(name.c_str()));
    }
  }

#if __DEBUG__
  ASSERT(obj);
#endif

  process.push_stack(obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_setattrs(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto src_obj = process.pop_stack();

  auto obj = process.top_stack();

  Frame* frame = *frame_ptr;

  const types::NativeTypeHandle& hndl = src_obj->ntvhndl();

  types::native_map map = types::get_value_from_handle<types::native_map>(hndl);

  // If we should clone each mapped object before setting it.
  bool should_clone = static_cast<bool>(instr.oprd1);
  bool should_override_map_values = static_cast<bool>(instr.oprd2);

  auto compartment_id = src_obj->closure_ctx().compartment_id;

  Compartment* compartment = nullptr;
  process.get_compartment(compartment_id, &compartment);

  if (!compartment)
  {
    THROW(CompartmentNotFoundError(compartment_id));
  }

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    uint64_t str_key = static_cast<uint64_t>(itr->first);

    dyobj::attr_key_t attr_key = get_attr_key(compartment, str_key);

    dyobj::dyobj_id_t attr_id = static_cast<dyobj::dyobj_id_t>(itr->second);

    auto &attr_obj = process.get_dyobj(attr_id);

    if (should_clone)
    {
      auto cloned_attr_obj = process.create_dyobj();

      cloned_attr_obj->copy_from(attr_obj);
      cloned_attr_obj->manager().on_setattr();
      obj->putattr(attr_key, cloned_attr_obj);

      if (should_override_map_values)
      {
        itr->second = static_cast<types::native_map_mapped_type>(cloned_attr_obj->id());
      }
    }
    else
    {
      attr_obj.manager().on_setattr();
      obj->putattr(attr_key, &attr_obj);
    }
  }

  frame->push_eval_stack(std::move( types::NativeTypeHandle(map) ));
}

// -----------------------------------------------------------------------------

void
instr_handler_rsetattrs(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto str_key = static_cast<encoding_key_t>(instr.oprd1);

  Frame* frame = *frame_ptr;
  dyobj::attr_key_t attr_key = get_attr_key(
    frame->compartment(), str_key);

  auto attr_obj = process.top_stack();

  types::NativeTypeHandle& hndl = frame->top_eval_stack();

  types::native_map map = types::get_value_from_handle<
    types::native_map>(hndl);

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    dyobj::dyobj_id_t id = static_cast<dyobj::dyobj_id_t>(itr->second);

    auto &obj = process.get_dyobj(id);
    attr_obj->manager().on_setattr();
    obj.putattr(attr_key, attr_obj);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setattrs2(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto self_str_key = static_cast<encoding_key_t>(instr.oprd1);
  auto frame = *frame_ptr;
  dyobj::attr_key_t self_attr_key = get_attr_key(
    frame->compartment(), self_str_key);

  auto src_obj = process.pop_stack();

  auto target_obj = process.top_stack();

  auto * objects = process.create_dyobjs(src_obj->attr_count());

  size_t i = 0;
  for (auto itr = src_obj->begin(); itr != src_obj->end(); ++itr, ++i)
  {
    dyobj::attr_key_t attr_key = static_cast<dyobj::attr_key_t>(itr->first);

    auto attr_obj = itr->second;

    auto& cloned_attr_obj = objects[i];

    cloned_attr_obj.copy_from(*attr_obj);
    cloned_attr_obj.manager().on_setattr();
    cloned_attr_obj.putattr(self_attr_key, target_obj);

    target_obj->putattr(attr_key, &cloned_attr_obj);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_putobj(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto ptr = process.top_stack();
  Frame* frame = *frame_ptr;

  types::uint64 value(ptr->id());
  types::NativeTypeHandle hndl(value);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_getobj(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  auto hndl = frame->pop_eval_stack();

  dyobj::dyobj_id_t id = types::get_value_from_handle<dyobj::dyobj_id_t>(hndl);

  process.push_stack(&process.get_dyobj(id));
}

// -----------------------------------------------------------------------------

void
instr_handler_swap(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  process.swap_stack();
}

// -----------------------------------------------------------------------------

void
instr_handler_setflgc(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj->set_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }
  else
  {
    obj->clear_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setfldel(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj->set_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_INDELIBLE);
  }
  else
  {
    obj->clear_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_INDELIBLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setflcall(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj->set_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NON_CALLABLE);
  }
  else
  {
    obj->clear_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NON_CALLABLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setflmute(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj->set_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE);
  }
  else
  {
    obj->clear_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_pinvk(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** invk_ctx_ptr)
{
  auto obj = process.top_stack();

  if (obj->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NON_CALLABLE))
  {
    THROW(InvocationError(obj->id()));
  }

  const ClosureCtx& ctx = obj->closure_ctx();

  if (ctx.compartment_id == NONESET_COMPARTMENT_ID)
  {
    THROW(CompartmentNotFoundError(ctx.compartment_id));
  }

  if (ctx.closure_id == NONESET_CLOSURE_ID)
  {
    THROW(ClosureNotFoundError(ctx.closure_id));
  }

  Compartment* compartment = nullptr;
  process.get_compartment(ctx.compartment_id, &compartment);

  Closure *closure = nullptr;
  compartment->get_closure_by_id(ctx.closure_id, &closure);

#if __DEBUG__
  ASSERT(compartment);
  ASSERT(closure);
#endif

  process.emplace_invocation_ctx(ctx, compartment, closure);
  process.top_invocation_ctx(invk_ctx_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_invk(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  InvocationCtx* invk_ctx = *invk_ctx_ptr;

  const ClosureCtx& ctx = invk_ctx->closure_ctx();
  Compartment* compartment = invk_ctx->compartment();
  Closure* closure = invk_ctx->closure();

  process.emplace_frame(ctx, compartment, closure, process.pc());
  process.top_frame(frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_rtrn(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  Frame* frame = *frame_ptr;

  instr_addr_t return_addr = frame->return_addr();

  if (return_addr == NONESET_INSTR_ADDR)
  {
    THROW(InvalidInstrAddrError());
  }

  process.pop_frame();

  if (process.has_frame())
  {
    process.top_frame(frame_ptr);
    process.top_invocation_ctx(invk_ctx_ptr);
  }

  if (process.stack_size() > 0)
  {
    auto obj = process.top_stack();
    obj->manager().on_setattr();
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_jmp(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  instr_addr_t starting_addr = process.pc();
  instr_addr_t relative_addr = static_cast<instr_addr_t>(instr.oprd1);

  instr_addr_t addr = starting_addr + relative_addr;

  if (addr == NONESET_INSTR_ADDR)
  {
    THROW(InvalidInstrAddrError());
  }

  process.set_pc(addr);
}

// -----------------------------------------------------------------------------

void
instr_handler_jmpif(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  instr_addr_t starting_addr = process.pc();
  instr_addr_t relative_addr = static_cast<instr_addr_t>(instr.oprd1);

  instr_addr_t addr = starting_addr + relative_addr;

  if (addr == NONESET_INSTR_ADDR)
  {
    THROW(InvalidInstrAddrError());
  }

  types::NativeTypeHandle& hndl = frame->top_eval_stack();

  bool value = types::get_value_from_handle<bool>(hndl);

  if (value)
  {
    process.set_pc(addr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_jmpr(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  const instr_addr_t starting_addr = 0;

  auto relative_addr = static_cast<instr_addr_t>(instr.oprd1);

  const instr_addr_t addr = starting_addr + relative_addr;

  if (addr == NONESET_INSTR_ADDR)
  {
    THROW(InvalidInstrAddrError());
  }

  process.set_pc(addr);
}

// -----------------------------------------------------------------------------

void
instr_handler_exc(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  bool search_catch_sites = static_cast<bool>(instr.oprd1);

  while (process.has_frame())
  {
    Frame& frame = process.top_frame();
    auto exc_obj = process.pop_stack();
    instr_addr_t starting_addr = 0;
    int64_t dst = 0;

    if (search_catch_sites)
    {
      const Closure *closure = frame.closure();

#if __DEBUG__
      ASSERT(closure);
#endif

      uint32_t index = static_cast<uint32_t>(process.pc() - starting_addr);

      if (search_catch_sites)
      {
        const auto& catch_sites = closure->catch_sites;

        auto itr = std::find_if(
          catch_sites.begin(),
          catch_sites.end(),
          [&index](const CatchSite& catch_site) -> bool {
            return index >= catch_site.from && index <= catch_site.to;
          }
        );

        if (itr != catch_sites.end())
        {
          const CatchSite& catch_site = *itr;

          dst = catch_site.dst;
        }
      }
    }

    if (dst)
    {
      // A catch site found in the current frame. Jump to its destination.
      frame.set_exc_obj(exc_obj);

      instr_addr_t addr =
        starting_addr + static_cast<instr_addr_t>(dst);

      // Minus one so that it will be `addr` after this instruction finishes,
      // since the pc gets incremented after every instruction.
      process.set_pc(addr - 1);
      return;
    }
    else
    {
      // No matching catch site found in the current frame.
      // Pop the current frame, and set the exc object on the previous frame.
      process.pop_frame();

      if (process.has_frame())
      {
        runtime::Frame& previous_frame = process.top_frame();
        *frame_ptr = &previous_frame;
        *invk_ctx_ptr = &process.top_invocation_ctx();

        previous_frame.set_exc_obj(exc_obj);

        process.push_stack(exc_obj);

        // Need to search catch sites in the other frames.
        search_catch_sites = true;
      }
    }
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_excobj(const Instr& /* instr */, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  const Frame* frame = *frame_ptr;

  auto exc_obj = frame->exc_obj();

  if (!exc_obj)
  {
    THROW(InvalidOperationError("No exception raised"));
  }
  else
  {
    process.push_stack(exc_obj);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_clrexc(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  frame->clear_exc_obj();
}

// -----------------------------------------------------------------------------

void
instr_handler_jmpexc(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  const Frame* frame = *frame_ptr;
  auto exc_obj = frame->exc_obj();

  bool jump_on_exc = static_cast<bool>(instr.oprd2);

  bool jump = jump_on_exc ? static_cast<bool>(exc_obj) : exc_obj == NULL;

  if (jump)
  {
    instr_addr_t starting_addr = process.pc();
    instr_addr_t relative_addr =
      static_cast<instr_addr_t>(instr.oprd1);

    instr_addr_t addr = starting_addr + relative_addr;

    if (addr == NONESET_INSTR_ADDR)
    {
      THROW(InvalidInstrAddrError());
    }
    else if (addr < starting_addr)
    {
      THROW(InvalidInstrAddrError());
    }

    process.set_pc(addr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_exit(const Instr& /* instr */, Process& /* process */,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  raise(SIGTERM);
}

// -----------------------------------------------------------------------------

void
instr_handler_putarg(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** invk_ctx_ptr)
{
  auto obj = process.pop_stack();
  obj->manager().on_setattr();
  InvocationCtx* invk_ctx = *invk_ctx_ptr;
  invk_ctx->put_param(obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_putkwarg(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** invk_ctx_ptr)
{
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  InvocationCtx* invk_ctx = *invk_ctx_ptr;

  auto obj = process.pop_stack();
  obj->manager().on_setattr();
  invk_ctx->put_param_value_pair(key, obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_putargs(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** invk_ctx_ptr)
{
  InvocationCtx* invk_ctx = *invk_ctx_ptr;
  auto obj = process.pop_stack();

  const types::NativeTypeHandle& hndl = obj->ntvhndl();

  types::native_array array =
    types::get_value_from_handle<types::native_array>(hndl);

  for (auto itr = array.begin(); itr != array.end(); ++itr)
  {
    dyobj::dyobj_id_t arg_id = static_cast<dyobj::dyobj_id_t>(*itr);
    auto& arg_obj = process.get_dyobj(arg_id);
    arg_obj.manager().on_setattr();
    invk_ctx->put_param(&arg_obj);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_putkwargs(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** invk_ctx_ptr)
{
  InvocationCtx* invk_ctx = *invk_ctx_ptr;
  auto obj = process.pop_stack();

  const types::NativeTypeHandle& result = obj->ntvhndl();

  types::native_map map =
    types::get_value_from_handle<types::native_map>(result);

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    variable_key_t arg_key = static_cast<variable_key_t>(itr->first);
    dyobj::dyobj_id_t arg_id = static_cast<dyobj::dyobj_id_t>(itr->second);

    auto& arg_obj = process.get_dyobj(arg_id);
    arg_obj.manager().on_setattr();

    invk_ctx->put_param_value_pair(arg_key, &arg_obj);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_getarg(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  InvocationCtx* invk_ctx = *invk_ctx_ptr;
  auto obj = invk_ctx->pop_param();
  obj->manager().on_setattr();

  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);

  Frame* frame = *frame_ptr;
  frame->set_visible_var(key, obj);
}

// -----------------------------------------------------------------------------

void
instr_handler_getkwarg(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  InvocationCtx* invk_ctx = *invk_ctx_ptr;
  variable_key_t key = static_cast<variable_key_t>(instr.oprd1);

  if (invk_ctx->has_param_value_pair_with_key(key))
  {
    auto obj = invk_ctx->pop_param_value_pair(key);

    Frame* frame = *frame_ptr;
    frame->set_visible_var(key, obj);

    auto relative_addr = static_cast<instr_addr_t>(instr.oprd2);
    process.set_pc(process.pc() + relative_addr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_getargs(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  Frame* frame = *frame_ptr;
  InvocationCtx* invk_ctx = *invk_ctx_ptr;

  types::native_array array;

  while (invk_ctx->has_params())
  {
    auto obj = invk_ctx->pop_param();
    array.push_back(obj->id());
  }

  types::NativeTypeHandle hndl(std::move(array));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_getkwargs(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  Frame* frame = *frame_ptr;
  InvocationCtx* invk_ctx = *invk_ctx_ptr;

  types::native_map map;

  std::vector<variable_key_t> params = invk_ctx->param_value_pair_keys();

  for (auto itr = params.begin(); itr != params.end(); ++itr)
  {
    variable_key_t key = static_cast<variable_key_t>(*itr);
    auto obj = invk_ctx->pop_param_value_pair(key);

    auto key_val = static_cast<typename types::native_map::key_type>(key);
    map[key_val] = obj->id();
  }

  types::NativeTypeHandle hndl(std::move(map));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_hasargs(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** invk_ctx_ptr)
{
  Frame* frame = *frame_ptr;
  InvocationCtx* invk_ctx = *invk_ctx_ptr;

  const bool result = invk_ctx->has_params();

  types::NativeTypeHandle hndl( (types::boolean(result)) );

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_gc(const Instr& /* instr */, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  process.do_gc();
}

// -----------------------------------------------------------------------------

void
instr_handler_debug(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  const uint32_t opts = static_cast<uint32_t>(instr.oprd1);
  ProcessPrinter printer(process, opts);
  printer(std::cout) << std::endl;
}

// -----------------------------------------------------------------------------

void
instr_handler_dbgfrm(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  auto& frame = *frame_ptr;

  const uint32_t opts = static_cast<uint32_t>(instr.oprd1);
  FramePrinter printer(*frame, opts);
  printer(std::cout) << std::endl;
}

// -----------------------------------------------------------------------------

void
instr_handler_dbgmem(const Instr& instr, Process& /* process */,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  const uint32_t opts = static_cast<uint32_t>(instr.oprd1);
  DbgMemPrinter printer(opts);
  printer(std::cout) << std::endl;
}

// -----------------------------------------------------------------------------

void
instr_handler_dbgvar(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  const encoding_key_t encoding_key = static_cast<encoding_key_t>(instr.oprd1);

  Frame* frame = *frame_ptr;
  const std::string variable_name(
    frame->compartment()->get_string_literal(encoding_key));

  const variable_key_t key = static_cast<variable_key_t>(instr.oprd1);
  Process::dyobj_ptr obj = NULL;

  if (frame->get_visible_var_through_ancestry(key, &obj))
  {
#if __DEBUG__
    ASSERT(obj);
#endif
  }
  else
  {
    std::string name;
    frame->compartment()->get_string_literal(encoding_key, &name);
    THROW(NameNotFoundError(name.c_str()));
  }

  std::stringstream ss;
  ss << "------------------- DEBUG -------------------" << std::endl;
  ss << std::setw(10) << "Module: " << frame->compartment()->path() << std::endl;
  ss << std::setw(10) << "Frame: " << frame->closure()->name << std::endl;
  ss << std::setw(10) << "Variable: " << variable_name << std::endl;
  ss << "\tAttribute count: " << obj->attr_count() << std::endl;
  ss << "\tFlags: " << obj->flags() << std::endl;
  ss << "\tIs garbage collectible: " << obj->is_garbage_collectible() << std::endl;
  std::cout << ss.str();
}

// -----------------------------------------------------------------------------

void
instr_handler_print(const Instr& instr, Process& process,
  Frame** /* frame_ptr */, InvocationCtx** /* invk_ctx_ptr */)
{
  auto obj = process.top_stack();

  if (!obj->has_ntvhndl())
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  const types::NativeTypeHandle& hndl = obj->ntvhndl();

  types::native_string native_str =
    types::get_value_from_handle<types::native_string>(hndl);

  static const char* PRINT_FORMATS[2] = { "%s", "%s\n" };

  const size_t index = static_cast<size_t>(instr.oprd1) % 2;

  printf(PRINT_FORMATS[index], native_str.c_str());
}

// -----------------------------------------------------------------------------

void
instr_handler_swap2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;
  frame->swap_eval_stack();
}

// -----------------------------------------------------------------------------

void
instr_handler_pos(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_positive_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_neg(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_negation_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_inc(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_increment_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_dec(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_decrement_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_abs(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_abs_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_sqrt(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_sqrt_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_add(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_addition_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_sub(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_subtraction_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_mul(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_multiplication_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_div(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_division_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_mod(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_modulus_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_pow(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_pow_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_bnot(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_bitwise_not_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_band(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_bitwise_and_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_bor(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_bitwise_or_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_bxor(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_bitwise_xor_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_bls(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_bitwise_left_shift_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_brs(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_bitwise_right_shift_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_eq(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_eq_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_neq(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_neq_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_gt(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_gt_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_lt(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_lt_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_gte(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_gte_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_lte(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_lte_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_lnot(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_unary_operator_instr(*frame_ptr, types::interface_apply_logical_not_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_land(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_logical_and_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_lor(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_logical_or_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_cmp(
  const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_binary_operator_instr(*frame_ptr, types::interface_apply_cmp_operator);
}

// -----------------------------------------------------------------------------

void
instr_handler_int8(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::int8>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_uint8(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::uint8>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_int16(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::int16>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_uint16(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::uint16>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_int32(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::int32>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_uint32(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::uint32>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_int64(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::int64>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_uint64(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::uint64>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_bool(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_integer_type_creation_instr<types::boolean>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_dec1(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_floating_type_creation_instr<types::decimal>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_dec2(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_floating_type_creation_instr<types::decimal2>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_str(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  // String type is different than other complex types.
  Frame* frame = *frame_ptr;

  std::string str;

  if (instr.oprd1 > 0)
  {
    auto encoding_key = static_cast<runtime::encoding_key_t>(instr.oprd1);

    const Compartment* compartment = frame->compartment();

    str = compartment->get_string_literal(encoding_key);
  }

  types::NativeTypeHandle hndl = types::string(str);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_ary(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_complex_type_creation_instr<types::array>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_map(const Instr& instr, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_complex_type_creation_instr<types::map>(instr, *frame_ptr);
}

// -----------------------------------------------------------------------------

void
instr_handler_2int8(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_int8);
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint8(
  const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_uint8);
}

// -----------------------------------------------------------------------------

void
instr_handler_2int16(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_int16);
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint16(
  const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_uint16);
}

// -----------------------------------------------------------------------------

void
instr_handler_2int32(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_int32);
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint32(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_uint32);
}

// -----------------------------------------------------------------------------

void
instr_handler_2int64(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_int64);
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint64(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_uint64);
}

// -----------------------------------------------------------------------------

void
instr_handler_2bool(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_bool);
}

// -----------------------------------------------------------------------------

void
instr_handler_2dec1(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_dec1);
}

// -----------------------------------------------------------------------------

void
instr_handler_2dec2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_dec2);
}

// -----------------------------------------------------------------------------

void
instr_handler_2str(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_str);
}

// -----------------------------------------------------------------------------

void
instr_handler_2ary(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_ary);
}

// -----------------------------------------------------------------------------

void
instr_handler_2map(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_conversion_instr(*frame_ptr, types::interface_to_map);
}

// -----------------------------------------------------------------------------

void
instr_handler_truthy(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  types::NativeTypeHandle& oprd = frame->top_eval_stack();
  types::NativeTypeHandle result =
    types::interface_compute_truthy_value(oprd);

  frame->push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
instr_handler_repr(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  types::NativeTypeHandle& oprd = frame->top_eval_stack();
  types::NativeTypeHandle result =
    types::interface_compute_repr_value(oprd);

  frame->push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
instr_handler_hash(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  Frame* frame = *frame_ptr;

  types::NativeTypeHandle& oprd = frame->top_eval_stack();
  types::NativeTypeHandle result =
    types::interface_compute_hash_value(oprd);

  frame->push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
instr_handler_slice(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands(*frame_ptr,
    types::interface_compute_slice);
}

// -----------------------------------------------------------------------------

void
instr_handler_stride(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_compute_stride);
}

// -----------------------------------------------------------------------------

void
instr_handler_reverse(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_compute_reverse);
}

// -----------------------------------------------------------------------------

void
instr_handler_round(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_apply_rounding);
}

// -----------------------------------------------------------------------------

void
instr_handler_strlen(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_string_get_size);
}

// -----------------------------------------------------------------------------

void
instr_handler_strat(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_string_at_2);
}

// -----------------------------------------------------------------------------

void
instr_handler_strclr(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand_in_place(*frame_ptr,
    types::interface_string_clear);
}

// -----------------------------------------------------------------------------

void
instr_handler_strapd(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_string_append);
}

// -----------------------------------------------------------------------------

void
instr_handler_strpsh(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_string_pushback);
}

// -----------------------------------------------------------------------------

void
instr_handler_strist(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands_in_place(*frame_ptr,
    types::interface_string_insert_str);
}

// -----------------------------------------------------------------------------

void
instr_handler_strist2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands_in_place(*frame_ptr,
    types::interface_string_insert_char);
}

// -----------------------------------------------------------------------------

void
instr_handler_strers(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_string_erase);
}

// -----------------------------------------------------------------------------

void
instr_handler_strers2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands_in_place(*frame_ptr,
    types::interface_string_erase2);
}

// -----------------------------------------------------------------------------

void
instr_handler_strrplc(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_four_operands_in_place(*frame_ptr,
    types::interface_string_replace_str);
}

// -----------------------------------------------------------------------------

void
instr_handler_strswp(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_string_swap);
}

// -----------------------------------------------------------------------------

void
instr_handler_strsub(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_string_substr);
}

// -----------------------------------------------------------------------------

void
instr_handler_strsub2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands(*frame_ptr,
    types::interface_string_substr2);
}

// -----------------------------------------------------------------------------

void
instr_handler_strfnd(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_string_find);
}

// -----------------------------------------------------------------------------

void
instr_handler_strfnd2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands(*frame_ptr,
    types::interface_string_find2);
}

// -----------------------------------------------------------------------------

void
instr_handler_strrfnd(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_string_rfind);
}

// -----------------------------------------------------------------------------

void
instr_handler_strrfnd2(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands(*frame_ptr,
    types::interface_string_rfind2);
}

// -----------------------------------------------------------------------------

void
instr_handler_arylen(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_array_size);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryemp(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_array_empty);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryat(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_array_at);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryfrt(
  const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_array_front);
}

// -----------------------------------------------------------------------------

void
instr_handler_arybak(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_array_back);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryput(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands_in_place(*frame_ptr,
    types::interface_array_put);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryapnd(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_array_append);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryers(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_array_erase);
}

// -----------------------------------------------------------------------------

void
instr_handler_arypop(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_array_pop);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryswp(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_array_swap);
}

// -----------------------------------------------------------------------------

void
instr_handler_aryclr(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand_in_place(*frame_ptr,
    types::interface_array_clear);
}

// -----------------------------------------------------------------------------

void
instr_handler_arymrg(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_array_merge);
}

// -----------------------------------------------------------------------------

void
instr_handler_maplen(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_map_size);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapemp(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_map_empty);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapat(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_map_at);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapfind(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_map_find);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapput(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_three_operands_in_place(*frame_ptr,
    types::interface_map_put);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapset(const Instr& instr, Process& process,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  types::native_map_key_type key = static_cast<
    types::native_map_key_type>(instr.oprd1);

  auto ptr = process.top_stack();

  Frame* frame = *frame_ptr;

  types::NativeTypeHandle& res = frame->top_eval_stack();

  types::native_map map = types::get_value_from_handle<types::native_map>(res);

  map[key] = ptr->id();

  res = map;
}

// -----------------------------------------------------------------------------

void
instr_handler_mapers(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_map_erase);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapclr(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand_in_place(*frame_ptr,
    types::interface_map_clear);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapswp(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands_in_place(*frame_ptr,
    types::interface_map_swap);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapkeys(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_map_keys);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapvals(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_single_operand(*frame_ptr,
    types::interface_map_vals);
}

// -----------------------------------------------------------------------------

void
instr_handler_mapmrg(const Instr& /* instr */, Process& /* process */,
  Frame** frame_ptr, InvocationCtx** /* invk_ctx_ptr */)
{
  execute_native_type_complex_instr_with_two_operands(*frame_ptr,
    types::interface_map_merge);
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
