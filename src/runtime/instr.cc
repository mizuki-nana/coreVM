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
#include "instr.h"

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


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc99-extensions"
#endif

const instr_handler_wrapper
instr_handler_meta::instr_handlers[INSTR_CODE_MAX] {

  /* -------------------------- Object instructions ------------------------- */

  /* NEW       */    { .handler=std::make_shared<instr_handler_new>()       },
  /* LDOBJ     */    { .handler=std::make_shared<instr_handler_ldobj>()     },
  /* STOBJ     */    { .handler=std::make_shared<instr_handler_stobj>()     },
  /* STOBJN    */    { .handler=std::make_shared<instr_handler_stobjn>()    },
  /* GETATTR   */    { .handler=std::make_shared<instr_handler_getattr>()   },
  /* SETATTR   */    { .handler=std::make_shared<instr_handler_setattr>()   },
  /* DELATTR   */    { .handler=std::make_shared<instr_handler_delattr>()   },
  /* HASATTR2  */    { .handler=std::make_shared<instr_handler_hasattr2>()  },
  /* GETATTR2  */    { .handler=std::make_shared<instr_handler_getattr2>()  },
  /* SETATTR2  */    { .handler=std::make_shared<instr_handler_setattr2>()  },
  /* DELATTR2  */    { .handler=std::make_shared<instr_handler_delattr2>()  },
  /* POP       */    { .handler=std::make_shared<instr_handler_pop>()       },
  /* LDOBJ2    */    { .handler=std::make_shared<instr_handler_ldobj2>()    },
  /* STOBJ2    */    { .handler=std::make_shared<instr_handler_stobj2>()    },
  /* DELOBJ    */    { .handler=std::make_shared<instr_handler_delobj>()    },
  /* DELOBJ2   */    { .handler=std::make_shared<instr_handler_delobj2>()   },
  /* GETHNDL   */    { .handler=std::make_shared<instr_handler_gethndl>()   },
  /* SETHNDL   */    { .handler=std::make_shared<instr_handler_sethndl>()   },
  /* GETHNDL2  */    { .handler=std::make_shared<instr_handler_gethndl2>()  },
  /* CLRHNDL   */    { .handler=std::make_shared<instr_handler_clrhndl>()   },
  /* CPYHNDL   */    { .handler=std::make_shared<instr_handler_cpyhndl>()   },
  /* CPYREPR   */    { .handler=std::make_shared<instr_handler_cpyrepr>()   },
  /* ISTRUTHY  */    { .handler=std::make_shared<instr_handler_istruthy>()  },
  /* OBJEQ     */    { .handler=std::make_shared<instr_handler_objeq>()     },
  /* OBJNEQ    */    { .handler=std::make_shared<instr_handler_objneq>()    },
  /* SETCTX    */    { .handler=std::make_shared<instr_handler_setctx>()    },
  /* CLDOBJ    */    { .handler=std::make_shared<instr_handler_cldobj>()    },
  /* SETATTRS  */    { .handler=std::make_shared<instr_handler_setattrs>()  },
  /* RSETATTRS */    { .handler=std::make_shared<instr_handler_rsetattrs>() },
  /* SETATTRS2  */   { .handler=std::make_shared<instr_handler_setattrs2>() },
  /* PUTOBJ    */    { .handler=std::make_shared<instr_handler_putobj>()    },
  /* GETOBJ    */    { .handler=std::make_shared<instr_handler_getobj>()    },
  /* SWAP      */    { .handler=std::make_shared<instr_handler_swap>()      },
  /* SETFLGC   */    { .handler=std::make_shared<instr_handler_setflgc>()   },
  /* SETFLDEL  */    { .handler=std::make_shared<instr_handler_setfldel>()  },
  /* SETFLCALL */    { .handler=std::make_shared<instr_handler_setflcall>() },
  /* SETFLMUTE */    { .handler=std::make_shared<instr_handler_setflmute>() },

  /* -------------------------- Control instructions ------------------------ */

  /* PINVK     */    { .handler=std::make_shared<instr_handler_pinvk>()     },
  /* INVK      */    { .handler=std::make_shared<instr_handler_invk>()      },
  /* RTRN      */    { .handler=std::make_shared<instr_handler_rtrn>()      },
  /* JMP       */    { .handler=std::make_shared<instr_handler_jmp>()       },
  /* JMPIF     */    { .handler=std::make_shared<instr_handler_jmpif>()     },
  /* JMPR      */    { .handler=std::make_shared<instr_handler_jmpr>()      },
  /* EXC       */    { .handler=std::make_shared<instr_handler_exc>()       },
  /* EXCOBJ    */    { .handler=std::make_shared<instr_handler_excobj>()    },
  /* CLREXC    */    { .handler=std::make_shared<instr_handler_clrexc>()    },
  /* JMPEXC    */    { .handler=std::make_shared<instr_handler_jmpexc>()    },
  /* EXIT      */    { .handler=std::make_shared<instr_handler_exit>()      },

  /* ------------------------- Function instructions ------------------------ */

  /* PUTARG    */    { .handler=std::make_shared<instr_handler_putarg>()    },
  /* PUTKWARG  */    { .handler=std::make_shared<instr_handler_putkwarg>()  },
  /* PUTARGS   */    { .handler=std::make_shared<instr_handler_putargs>()   },
  /* PUTKWARGS */    { .handler=std::make_shared<instr_handler_putkwargs>() },
  /* GETARG    */    { .handler=std::make_shared<instr_handler_getarg>()    },
  /* GETKWARG  */    { .handler=std::make_shared<instr_handler_getkwarg>()  },
  /* GETARGS   */    { .handler=std::make_shared<instr_handler_getargs>()   },
  /* GETKWARGS */    { .handler=std::make_shared<instr_handler_getkwargs>() },
  /* HASARGS   */    { .handler=std::make_shared<instr_handler_hasargs>()   },

  /* ------------------------- Runtime instructions ------------------------- */

  /* GC        */    { .handler=std::make_shared<instr_handler_gc>()        },
  /* DEBUG     */    { .handler=std::make_shared<instr_handler_debug>()     },
  /* DBGFRM    */    { .handler=std::make_shared<instr_handler_dbgfrm>()    },
  /* PRINT     */    { .handler=std::make_shared<instr_handler_print>()     },
  /* SWAP2     */    { .handler=std::make_shared<instr_handler_swap2>()     },

  /* ---------------- Arithmetic and logic instructions --------------------- */

  /* POS      */     { .handler=std::make_shared<instr_handler_pos>()       },
  /* NEG      */     { .handler=std::make_shared<instr_handler_neg>()       },
  /* INC      */     { .handler=std::make_shared<instr_handler_inc>()       },
  /* DEC      */     { .handler=std::make_shared<instr_handler_dec>()       },
  /* ABS      */     { .handler=std::make_shared<instr_handler_abs>()       },
  /* SQRT     */     { .handler=std::make_shared<instr_handler_sqrt>()      },
  /* ADD      */     { .handler=std::make_shared<instr_handler_add>()       },
  /* SUB      */     { .handler=std::make_shared<instr_handler_sub>()       },
  /* MUL      */     { .handler=std::make_shared<instr_handler_mul>()       },
  /* DIV      */     { .handler=std::make_shared<instr_handler_div>()       },
  /* MOD      */     { .handler=std::make_shared<instr_handler_mod>()       },
  /* POW      */     { .handler=std::make_shared<instr_handler_pow>()       },
  /* BNOT     */     { .handler=std::make_shared<instr_handler_bnot>()      },
  /* BAND     */     { .handler=std::make_shared<instr_handler_band>()      },
  /* BOR      */     { .handler=std::make_shared<instr_handler_bor>()       },
  /* BXOR     */     { .handler=std::make_shared<instr_handler_bxor>()      },
  /* BLS      */     { .handler=std::make_shared<instr_handler_bls>()       },
  /* BRS      */     { .handler=std::make_shared<instr_handler_brs>()       },
  /* EQ       */     { .handler=std::make_shared<instr_handler_eq>()        },
  /* NEQ      */     { .handler=std::make_shared<instr_handler_neq>()       },
  /* GT       */     { .handler=std::make_shared<instr_handler_gt>()        },
  /* LT       */     { .handler=std::make_shared<instr_handler_lt>()        },
  /* GTE      */     { .handler=std::make_shared<instr_handler_gte>()       },
  /* LTE      */     { .handler=std::make_shared<instr_handler_lte>()       },
  /* LNOT     */     { .handler=std::make_shared<instr_handler_lnot>()      },
  /* LAND     */     { .handler=std::make_shared<instr_handler_land>()      },
  /* LOR      */     { .handler=std::make_shared<instr_handler_lor>()       },
  /* CMP      */     { .handler=std::make_shared<instr_handler_cmp>()       },

  /* ----------------- Native type creation instructions -------------------- */

  /* INT8     */     { .handler=std::make_shared<instr_handler_int8>()      },
  /* UINT8    */     { .handler=std::make_shared<instr_handler_uint8>()     },
  /* INT16    */     { .handler=std::make_shared<instr_handler_int16>()     },
  /* UINT16   */     { .handler=std::make_shared<instr_handler_uint16>()    },
  /* INT32    */     { .handler=std::make_shared<instr_handler_int32>()     },
  /* UINT32   */     { .handler=std::make_shared<instr_handler_uint32>()    },
  /* INT64    */     { .handler=std::make_shared<instr_handler_int64>()     },
  /* UINT64   */     { .handler=std::make_shared<instr_handler_uint64>()    },
  /* BOOL     */     { .handler=std::make_shared<instr_handler_bool>()      },
  /* DEC1     */     { .handler=std::make_shared<instr_handler_dec1>()      },
  /* DEC2     */     { .handler=std::make_shared<instr_handler_dec2>()      },
  /* STR      */     { .handler=std::make_shared<instr_handler_str>()       },
  /* ARY      */     { .handler=std::make_shared<instr_handler_ary>()       },
  /* MAP      */     { .handler=std::make_shared<instr_handler_map>()       },

  /* ----------------- Native type conversion instructions ------------------ */

  /* TOINT8   */     { .handler=std::make_shared<instr_handler_2int8>()     },
  /* TOUINT8  */     { .handler=std::make_shared<instr_handler_2uint8>()    },
  /* TOINT16  */     { .handler=std::make_shared<instr_handler_2int16>()    },
  /* TOUINT16 */     { .handler=std::make_shared<instr_handler_2uint16>()   },
  /* TOINT32  */     { .handler=std::make_shared<instr_handler_2int32>()    },
  /* TOUINT32 */     { .handler=std::make_shared<instr_handler_2uint32>()   },
  /* TOINT64  */     { .handler=std::make_shared<instr_handler_2int64>()    },
  /* TOUINT64 */     { .handler=std::make_shared<instr_handler_2uint64>()   },
  /* TOBOOL   */     { .handler=std::make_shared<instr_handler_2bool>()     },
  /* TODEC1   */     { .handler=std::make_shared<instr_handler_2dec1>()     },
  /* TODEC2   */     { .handler=std::make_shared<instr_handler_2dec2>()     },
  /* TOSTR    */     { .handler=std::make_shared<instr_handler_2str>()      },
  /* TOARY    */     { .handler=std::make_shared<instr_handler_2ary>()      },
  /* TOMAP    */     { .handler=std::make_shared<instr_handler_2map>()      },

  /* ----------------- Native type manipulation instructions ---------------- */

  /* TRUTHY   */     { .handler=std::make_shared<instr_handler_truthy>()    },
  /* REPR     */     { .handler=std::make_shared<instr_handler_repr>()      },
  /* HASH     */     { .handler=std::make_shared<instr_handler_hash>()      },
  /* SLICE    */     { .handler=std::make_shared<instr_handler_slice>()     },
  /* STRIDE   */     { .handler=std::make_shared<instr_handler_stride>()    },
  /* REVERSE  */     { .handler=std::make_shared<instr_handler_reverse>()   },
  /* ROUND    */     { .handler=std::make_shared<instr_handler_round>()     },

  /* --------------------- String type instructions ------------------------- */

  /* STRLEN   */     { .handler=std::make_shared<instr_handler_strlen>()    },
  /* STRAT    */     { .handler=std::make_shared<instr_handler_strat>()     },
  /* STRCLR   */     { .handler=std::make_shared<instr_handler_strclr>()    },
  /* STRAPD   */     { .handler=std::make_shared<instr_handler_strapd>()    },
  /* STRPSH   */     { .handler=std::make_shared<instr_handler_strpsh>()    },
  /* STRIST   */     { .handler=std::make_shared<instr_handler_strist>()    },
  /* STRIST2  */     { .handler=std::make_shared<instr_handler_strist2>()   },
  /* STRERS   */     { .handler=std::make_shared<instr_handler_strers>()    },
  /* STRERS2  */     { .handler=std::make_shared<instr_handler_strers2>()   },
  /* STRRPLC  */     { .handler=std::make_shared<instr_handler_strrplc>()   },
  /* STRSWP   */     { .handler=std::make_shared<instr_handler_strswp>()    },
  /* STRSUB   */     { .handler=std::make_shared<instr_handler_strsub>()    },
  /* STRSUB2  */     { .handler=std::make_shared<instr_handler_strsub2>()   },
  /* STRFND   */     { .handler=std::make_shared<instr_handler_strfnd>()    },
  /* STRFND2  */     { .handler=std::make_shared<instr_handler_strfnd2>()   },
  /* STRRFND  */     { .handler=std::make_shared<instr_handler_strrfnd>()   },
  /* STRRFND2 */     { .handler=std::make_shared<instr_handler_strrfnd2>()  },

  /* --------------------- Array type instructions -------------------------- */

  /* ARYLEN   */     { .handler=std::make_shared<instr_handler_arylen>()    },
  /* ARYEMP   */     { .handler=std::make_shared<instr_handler_aryemp>()    },
  /* ARYAT    */     { .handler=std::make_shared<instr_handler_aryat>()     },
  /* ARYFRT   */     { .handler=std::make_shared<instr_handler_aryfrt>()    },
  /* ARYBAK   */     { .handler=std::make_shared<instr_handler_arybak>()    },
  /* ARYPUT   */     { .handler=std::make_shared<instr_handler_aryput>()    },
  /* ARYAPND  */     { .handler=std::make_shared<instr_handler_aryapnd>()   },
  /* ARYERS   */     { .handler=std::make_shared<instr_handler_aryers>()    },
  /* ARYPOP   */     { .handler=std::make_shared<instr_handler_arypop>()    },
  /* ARYSWP   */     { .handler=std::make_shared<instr_handler_aryswp>()    },
  /* ARYCLR   */     { .handler=std::make_shared<instr_handler_aryclr>()    },
  /* ARYMRG   */     { .handler=std::make_shared<instr_handler_arymrg>()    },

  /* --------------------- Map type instructions ---------------------------- */

  /* MAPLEN   */     { .handler=std::make_shared<instr_handler_maplen>()    },
  /* MAPEMP   */     { .handler=std::make_shared<instr_handler_mapemp>()    },
  /* MAPFIND  */     { .handler=std::make_shared<instr_handler_mapfind>()   },
  /* MAPAT    */     { .handler=std::make_shared<instr_handler_mapat>()     },
  /* MAPPUT   */     { .handler=std::make_shared<instr_handler_mapput>()    },
  /* MAPSET   */     { .handler=std::make_shared<instr_handler_mapset>()    },
  /* MAPERS   */     { .handler=std::make_shared<instr_handler_mapers>()    },
  /* MAPCLR   */     { .handler=std::make_shared<instr_handler_mapclr>()    },
  /* MAPSWP   */     { .handler=std::make_shared<instr_handler_mapswp>()    },
  /* MAPKEYS  */     { .handler=std::make_shared<instr_handler_mapkeys>()   },
  /* MAPVALS  */     { .handler=std::make_shared<instr_handler_mapvals>()   },
  /* MAPMRG   */     { .handler=std::make_shared<instr_handler_mapmrg>()    },

};

#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif  /* #if defined(__clang__) and __clang__ */

// -----------------------------------------------------------------------------


/* --------------------------- INSTRUCTION HANDLERS ------------------------- */


// -----------------------------------------------------------------------------

/* virtual */
instr_handler::~instr_handler()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_unary_operator_instr(
  frame* frame, InterfaceFunc interface_func)
{
  types::native_type_handle& oprd = frame->top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_binary_operator_instr(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& lhs = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& rhs = frame->eval_stack_element(eval_stack_size - 2);

  lhs = interface_func(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename NativeType>
/* static */
void
instr_handler::execute_native_integer_type_creation_instr(
  const instr& instr, frame* frame)
{
  types::native_type_handle hndl(NativeType(instr.oprd1));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename NativeType>
/* static */
void
instr_handler::execute_native_floating_type_creation_instr(
  const instr& instr, frame* frame)
{
  std::stringstream oprd2_ss;
  oprd2_ss << instr.oprd2;
  std::string oprd2_str = oprd2_ss.str();

  std::reverse(oprd2_str.begin(), oprd2_str.end());

  std::stringstream ss;
  ss << instr.oprd1 << "." << oprd2_str;

  types::native_type_handle hndl(NativeType(stod(ss.str())));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename NativeType>
/* static */
void
instr_handler::execute_native_complex_type_creation_instr(
  const instr& /* instr */, frame* frame)
{
  NativeType value;
  types::native_type_handle hndl(value);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_conversion_instr(
  frame* frame, InterfaceFunc interface_func)
{
  types::native_type_handle& oprd = frame->top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_single_operand(
  frame* frame, InterfaceFunc interface_func)
{
  types::native_type_handle& oprd = frame->top_eval_stack();

  oprd = interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_single_operand_in_place(
  frame* frame, InterfaceFunc interface_func)
{
  types::native_type_handle& oprd = frame->top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_two_operands(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);

  oprd1 = interface_func(oprd1, oprd2);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);

  interface_func(oprd1, oprd2);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_three_operands(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 3)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::native_type_handle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);

  oprd1 = interface_func(oprd1, oprd2, oprd3);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_three_operands_in_place(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 3)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::native_type_handle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);

  interface_func(oprd1, oprd2, oprd3);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_four_operands(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 4)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::native_type_handle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);
  types::native_type_handle& oprd4 = frame->eval_stack_element(eval_stack_size - 4);

  oprd1 = interface_func(oprd1, oprd2, oprd3, oprd4);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
/* static */
void
instr_handler::execute_native_type_complex_instr_with_four_operands_in_place(
  frame* frame, InterfaceFunc interface_func)
{
  size_t eval_stack_size = frame->eval_stack_size();

  if (eval_stack_size < 4)
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle& oprd1 = frame->eval_stack_element(eval_stack_size - 1);
  types::native_type_handle& oprd2 = frame->eval_stack_element(eval_stack_size - 2);
  types::native_type_handle& oprd3 = frame->eval_stack_element(eval_stack_size - 3);
  types::native_type_handle& oprd4 = frame->eval_stack_element(eval_stack_size - 4);

  interface_func(oprd1, oprd2, oprd3, oprd4);
}

// -----------------------------------------------------------------------------

void
instr_handler_new::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  auto id = process.create_dyobj();

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_ldobj::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);

  frame* frame = *frame_ptr;

  dyobj::dyobj_id id = 0;

  while (!frame->get_visible_var_fast(key, &id))
  {
    frame = frame->parent();

    if (!frame)
    {
      THROW(name_not_found_error());
    }
  }

#if __DEBUG__
  ASSERT(id);
#endif

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_stobj::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);

  frame* frame= *frame_ptr;

  dyobj::dyobj_id id = process.pop_stack();

  frame->set_visible_var(key, id);
}

// -----------------------------------------------------------------------------

void
instr_handler_stobjn::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);
  size_t n = static_cast<size_t>(instr.oprd2);

  frame& frame = process.top_nth_frame(n);

  dyobj::dyobj_id id = process.pop_stack();

  frame.set_visible_var(key, id);
}

// -----------------------------------------------------------------------------

void
instr_handler_getattr::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  auto str_key = static_cast<encoding_key>(instr.oprd1);
  auto frame = *frame_ptr;

  dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::dyobj_id attr_id = getattr(
    obj, frame->compartment_ptr(), str_key);

  process.push_stack(attr_id);
}

// -----------------------------------------------------------------------------

void
instr_handler_setattr::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  auto str_key = static_cast<encoding_key>(instr.oprd1);
  auto frame = *frame_ptr;
  dyobj::attr_key attr_key = get_attr_key(
    frame->compartment_ptr(), str_key);

  dyobj::dyobj_id attr_id= process.pop_stack();
  dyobj::dyobj_id target_id = process.top_stack();

  auto &obj = process.get_dyobj(target_id);

  if (obj.get_flag(dyobj::flags::DYOBJ_IS_IMMUTABLE))
  {
    THROW(invalid_operation_error(
      str(format("cannot mutate immutable object 0x%08x") % target_id)));
  }

  auto &attr_obj = process.get_dyobj(attr_id);
  obj.putattr(attr_key, attr_id);
  attr_obj.manager().on_setattr();
}

// -----------------------------------------------------------------------------

void
instr_handler_delattr::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::attr_key attr_key = static_cast<dyobj::attr_key>(instr.oprd1);

  dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  if (obj.get_flag(dyobj::flags::DYOBJ_IS_IMMUTABLE))
  {
    THROW(invalid_operation_error(
      str(format("cannot mutate immutable object 0x%08x") % id)));
  }

  dyobj::dyobj_id attr_id = obj.getattr(attr_key);
  auto &attr_obj = process.get_dyobj(attr_id);
  attr_obj.manager().on_delattr();
  obj.delattr(attr_key);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_hasattr2::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  const auto frame = *frame_ptr;
  types::native_type_handle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::attr_key attr_key = dyobj::hash_attr_str(attr_str);

  const bool res_value = obj.hasattr(attr_key);

  types::native_type_handle res( (types::boolean(res_value)) );

  frame->push_eval_stack(std::move(res));
}

// -----------------------------------------------------------------------------

void
instr_handler_getattr2::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  const auto frame = *frame_ptr;
  types::native_type_handle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::dyobj_id attr_id = getattr(obj, attr_str_value);

  process.push_stack(attr_id);
}

// -----------------------------------------------------------------------------

void
instr_handler_setattr2::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id attr_value = process.pop_stack();

  dyobj::dyobj_id target_obj_id = process.top_stack();
  auto &target_obj = process.get_dyobj(target_obj_id);

  const auto frame = *frame_ptr;
  types::native_type_handle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::attr_key attr_key = dyobj::hash_attr_str(attr_str);

  target_obj.putattr(attr_key, attr_value);
}

// -----------------------------------------------------------------------------

void
instr_handler_delattr2::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  const auto frame = *frame_ptr;
  types::native_type_handle hndl = frame->top_eval_stack();

  auto attr_str = types::get_value_from_handle<types::native_string>(hndl);
  std::string attr_str_value = static_cast<std::string>(attr_str);

  dyobj::attr_key attr_key = dyobj::hash_attr_str(attr_str);

  obj.delattr(attr_key);
}

// -----------------------------------------------------------------------------

void
instr_handler_pop::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  process.pop_stack();
}

// -----------------------------------------------------------------------------

void
instr_handler_ldobj2::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);

  frame* frame = *frame_ptr;

  dyobj::dyobj_id id = 0;

  while (!frame->get_invisible_var_fast(key, &id))
  {
    frame = frame->parent();

    if (!frame)
    {
      THROW(name_not_found_error());
    }
  }

#if __DEBUG__
  ASSERT(id);
#endif

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_stobj2::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);

  frame* frame = *frame_ptr;
  dyobj::dyobj_id id = process.pop_stack();

  frame->set_invisible_var(key, id);
}

// -----------------------------------------------------------------------------

void
instr_handler_delobj::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);
  frame* frame = *frame_ptr;

  dyobj::dyobj_id id = frame->pop_visible_var(key);
  auto &obj = process.get_dyobj(id);

  if (obj.get_flag(dyobj::flags::DYOBJ_IS_INDELIBLE))
  {
    THROW(object_deletion_error(id));
  }

  obj.manager().on_delete();
}

// -----------------------------------------------------------------------------

void
instr_handler_delobj2::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);
  frame* frame = *frame_ptr;

  dyobj::dyobj_id id = frame->pop_invisible_var(key);
  auto &obj = process.get_dyobj(id);

  if (obj.get_flag(dyobj::flags::DYOBJ_IS_INDELIBLE))
  {
    THROW(object_deletion_error(id));
  }

  obj.manager().on_delete();
}

// -----------------------------------------------------------------------------

void
instr_handler_gethndl::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_not_found_error());
  }

  types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  frame->push_eval_stack(hndl);
}

// -----------------------------------------------------------------------------

void
instr_handler_sethndl::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  types::native_type_handle hndl(std::move(frame->pop_eval_stack()));

  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key key = obj.ntvhndl_key();

  if (key == dyobj::NONESET_NTVHNDL_KEY)
  {
    dyobj::ntvhndl_key new_ntvhndl_key = process.insert_ntvhndl(hndl);

    obj.set_ntvhndl_key(new_ntvhndl_key);
  }
  else
  {
    process.get_ntvhndl(key) = std::move(hndl);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_gethndl2::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;
  variable_key key = static_cast<variable_key>(instr.oprd1);

  auto id = frame->get_visible_var(key);
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_not_found_error());
  }

  types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  frame->push_eval_stack(hndl);
}

// -----------------------------------------------------------------------------

void
instr_handler_clrhndl::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_deletion_error());
  }

  process.erase_ntvhndl(ntvhndl_key);
  obj.clear_ntvhndl_key();
}

// -----------------------------------------------------------------------------

void
instr_handler_cpyhndl::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id src_obj_id= process.pop_stack();
  dyobj::dyobj_id target_obj_id = process.pop_stack();

  auto &src_obj = process.get_dyobj(src_obj_id);
  auto &target_obj = process.get_dyobj(target_obj_id);

  dyobj::ntvhndl_key ntvhndl_key = src_obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_deletion_error());
  }

  types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);
  types::native_type_handle res(hndl);

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

  auto new_key = process.insert_ntvhndl(res);
  target_obj.set_ntvhndl_key(new_key);
}

// -----------------------------------------------------------------------------

void
instr_handler_cpyrepr::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id src_obj_id= process.pop_stack();
  dyobj::dyobj_id target_obj_id = process.pop_stack();

  auto &src_obj = process.get_dyobj(src_obj_id);
  auto &target_obj = process.get_dyobj(target_obj_id);

  dyobj::ntvhndl_key ntvhndl_key = src_obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_deletion_error());
  }

  types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);
  types::native_type_handle res =
    types::interface_compute_repr_value(hndl);

  auto new_key = process.insert_ntvhndl(res);
  target_obj.set_ntvhndl_key(new_key);
}

// -----------------------------------------------------------------------------

void
instr_handler_istruthy::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  dyobj::dyobj_id obj_id= process.top_stack();
  auto &obj = process.get_dyobj(obj_id);

  dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_not_found_error());
  }

  types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  types::native_type_handle res =
    types::interface_compute_truthy_value(hndl);

  frame->push_eval_stack(std::move(res));
}

// -----------------------------------------------------------------------------

void
instr_handler_objeq::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id1 = process.pop_stack();
  dyobj::dyobj_id id2 = process.pop_stack();

  types::native_type_handle hndl(types::boolean(id1 == id2));

  frame* frame = *frame_ptr;
  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_objneq::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id1 = process.pop_stack();
  dyobj::dyobj_id id2 = process.pop_stack();

  types::native_type_handle hndl(types::boolean(id1 != id2));

  frame* frame = *frame_ptr;
  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_setctx::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  frame* frame = *frame_ptr;
  closure_ctx frame_cls = frame->closure_ctx();

  closure_ctx ctx(
    frame_cls.compartment_id,
    static_cast<closure_id>(instr.oprd1));

  obj.set_closure_ctx(ctx);
}

// -----------------------------------------------------------------------------

void
instr_handler_cldobj::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  types::native_type_handle hndl = frame->pop_eval_stack();

  bool value = types::get_value_from_handle<bool>(hndl);

  variable_key key1 = static_cast<variable_key>(instr.oprd1);
  variable_key key2 = static_cast<variable_key>(instr.oprd2);

  variable_key key = value ? key1 : key2;

  dyobj::dyobj_id id = 0;

  while (!frame->get_visible_var_fast(key, &id))
  {
    frame = frame->parent();

    if (!frame)
    {
      THROW(name_not_found_error());
    }
  }

#if __DEBUG__
  ASSERT(id);
#endif

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_setattrs::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id src_id = process.pop_stack();
  auto& src_obj = process.get_dyobj(src_id);

  dyobj::dyobj_id id = process.top_stack();
  auto& obj = process.get_dyobj(id);

  frame* frame = *frame_ptr;

  types::native_type_handle& hndl = process.get_ntvhndl(src_obj.ntvhndl_key());
  types::native_type_handle res = hndl;

  types::native_map map = types::get_value_from_handle<
    types::native_map>(res);

  // If we should clone each mapped object before setting it.
  bool should_clone = static_cast<bool>(instr.oprd1);
  bool should_override_map_values = static_cast<bool>(instr.oprd2);

  auto compartment_id = src_obj.closure_ctx().compartment_id;

  compartment* compartment = nullptr;
  process.get_compartment(compartment_id, &compartment);

  if (!compartment)
  {
    THROW(compartment_not_found_error(compartment_id));
  }

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    uint64_t str_key = static_cast<uint64_t>(itr->first);

    dyobj::attr_key attr_key = get_attr_key(compartment, str_key);

    dyobj::dyobj_id attr_id = static_cast<dyobj::dyobj_id>(itr->second);

    auto &attr_obj = process.get_dyobj(attr_id);

    if (should_clone)
    {
      auto cloned_attr_id = process.create_dyobj();
      auto& cloned_attr_obj = process.get_dyobj(cloned_attr_id);

      cloned_attr_obj.copy_from(attr_obj);
      cloned_attr_obj.manager().on_setattr();
      obj.putattr(attr_key, cloned_attr_id);

      if (should_override_map_values)
      {
        itr->second = static_cast<types::native_map_mapped_type>(cloned_attr_id);
      }
    }
    else
    {
      attr_obj.manager().on_setattr();
      obj.putattr(attr_key, attr_id);
    }
  }

  res = map;

  frame->push_eval_stack(res);
}

// -----------------------------------------------------------------------------

void
instr_handler_rsetattrs::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  auto str_key = static_cast<encoding_key>(instr.oprd1);

  frame* frame = *frame_ptr;
  dyobj::attr_key attr_key = get_attr_key(
    frame->compartment_ptr(), str_key);

  dyobj::dyobj_id attr_id = process.top_stack();
  auto& attr_obj = process.get_dyobj(attr_id);

  types::native_type_handle& hndl = frame->top_eval_stack();

  types::native_map map = types::get_value_from_handle<
    types::native_map>(hndl);

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    dyobj::dyobj_id id = static_cast<dyobj::dyobj_id>(itr->second);

    auto &obj = process.get_dyobj(id);
    attr_obj.manager().on_setattr();
    obj.putattr(attr_key, attr_id);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setattrs2::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  auto self_str_key = static_cast<encoding_key>(instr.oprd1);
  auto frame = *frame_ptr;
  dyobj::attr_key self_attr_key = get_attr_key(
    frame->compartment_ptr(), self_str_key);

  dyobj::dyobj_id src_id = process.pop_stack();
  auto& src_obj = process.get_dyobj(src_id);

  dyobj::dyobj_id target_id = process.top_stack();
  auto& target_obj = process.get_dyobj(target_id);

  auto * objects = process.create_dyobjs(src_obj.attr_count());

  size_t i = 0;
  for (auto itr = src_obj.begin(); itr != src_obj.end(); ++itr, ++i)
  {
    dyobj::attr_key attr_key = static_cast<dyobj::attr_key>(itr->first);

    dyobj::dyobj_id attr_id = static_cast<dyobj::dyobj_id>(itr->second);

    auto &attr_obj = process.get_dyobj(attr_id);

    auto& cloned_attr_obj = objects[i];

    cloned_attr_obj.copy_from(attr_obj);
    cloned_attr_obj.manager().on_setattr();
    cloned_attr_obj.putattr(self_attr_key, target_id);

    target_obj.putattr(attr_key, cloned_attr_obj.id());
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_putobj::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  frame* frame = *frame_ptr;

  types::uint64 value(id);
  types::native_type_handle hndl(value);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_getobj::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;
  auto hndl = frame->pop_eval_stack();

  dyobj::dyobj_id id = types::get_value_from_handle<
    dyobj::dyobj_id>(hndl);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_swap::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  process.swap_stack();
}

// -----------------------------------------------------------------------------

void
instr_handler_setflgc::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(dyobj::flags::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }
  else
  {
    obj.clear_flag(dyobj::flags::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setfldel::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(dyobj::flags::DYOBJ_IS_INDELIBLE);
  }
  else
  {
    obj.clear_flag(dyobj::flags::DYOBJ_IS_INDELIBLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setflcall::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(dyobj::flags::DYOBJ_IS_NON_CALLABLE);
  }
  else
  {
    obj.clear_flag(dyobj::flags::DYOBJ_IS_NON_CALLABLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_setflmute::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(dyobj::flags::DYOBJ_IS_IMMUTABLE);
  }
  else
  {
    obj.clear_flag(dyobj::flags::DYOBJ_IS_IMMUTABLE);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_pinvk::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** invk_ctx_ptr)
{
  dyobj::dyobj_id id = process.top_stack();
  auto& obj = process.get_dyobj(id);

  if (obj.get_flag(dyobj::flags::DYOBJ_IS_NON_CALLABLE))
  {
    THROW(invocation_error(id));
  }

  const closure_ctx& ctx = obj.closure_ctx();

  if (ctx.compartment_id == NONESET_COMPARTMENT_ID)
  {
    THROW(compartment_not_found_error(ctx.compartment_id));
  }

  if (ctx.closure_id == NONESET_CLOSURE_ID)
  {
    THROW(closure_not_found_error(ctx.closure_id));
  }

  compartment* compartment = nullptr;
  process.get_compartment(ctx.compartment_id, &compartment);

  closure *closure = nullptr;
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
instr_handler_invk::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  invocation_ctx* invk_ctx = *invk_ctx_ptr;

  const closure_ctx& ctx = invk_ctx->closure_ctx();
  compartment* compartment = invk_ctx->compartment_ptr();
  closure* closure = invk_ctx->closure_ptr();

  process.emplace_frame(ctx, compartment, closure, process.pc());
  process.top_frame(frame_ptr);

  process.insert_vector(closure->vector);
}

// -----------------------------------------------------------------------------

void
instr_handler_rtrn::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  frame* frame = *frame_ptr;

  instr_addr return_addr = frame->return_addr();

  if (return_addr == NONESET_INSTR_ADDR)
  {
    THROW(invalid_instr_addr_error());
  }

  process.pop_frame();

  if (process.has_frame())
  {
    process.top_frame(frame_ptr);
    process.top_invocation_ctx(invk_ctx_ptr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_jmp::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_addr starting_addr = process.pc();
  instr_addr relative_addr = static_cast<instr_addr>(instr.oprd1);

  instr_addr addr = starting_addr + relative_addr;

  if (addr == NONESET_INSTR_ADDR)
  {
    THROW(invalid_instr_addr_error());
  }
  else if (addr < starting_addr)
  {
    THROW(invalid_instr_addr_error());
  }

  process.set_pc(addr);
}

// -----------------------------------------------------------------------------

void
instr_handler_jmpif::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  instr_addr starting_addr = process.pc();
  instr_addr relative_addr = static_cast<instr_addr>(instr.oprd1);

  instr_addr addr = starting_addr + relative_addr;

  if (addr == NONESET_INSTR_ADDR)
  {
    THROW(invalid_instr_addr_error());
  }
  else if (addr < starting_addr)
  {
    THROW(invalid_instr_addr_error());
  }

  types::native_type_handle& hndl = frame->top_eval_stack();

  bool value = types::get_value_from_handle<bool>(hndl);

  if (value)
  {
    process.set_pc(addr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_jmpr::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;
  instr_addr starting_addr = frame->return_addr() + 1;

  instr_addr relative_addr =
    static_cast<instr_addr>(instr.oprd1);

  instr_addr addr = starting_addr + relative_addr;

  if (addr == NONESET_INSTR_ADDR)
  {
    THROW(invalid_instr_addr_error());
  }
  else if (addr < starting_addr)
  {
    THROW(invalid_instr_addr_error());
  }

  process.set_pc(addr);
}

// -----------------------------------------------------------------------------

void
instr_handler_exc::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  bool search_catch_sites = static_cast<bool>(instr.oprd1);

  while (process.has_frame())
  {
    frame& frame = process.top_frame();
    dyobj::dyobj_id exc_obj_id = process.pop_stack();
    instr_addr starting_addr = frame.return_addr() + 1;
    uint32_t dst = 0;

    if (search_catch_sites)
    {
      const closure *closure = frame.closure_ptr();

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
          [&index](const catch_site& catch_site) -> bool {
            return index >= catch_site.from && index <= catch_site.to;
          }
        );

        if (itr != catch_sites.end())
        {
          const catch_site& catch_site = *itr;

          dst = catch_site.dst;
        }
      }
    }

    if (dst)
    {
      // A catch site found in the current frame. Jump to its destination.
      frame.set_exc_obj(exc_obj_id);

      instr_addr addr =
        starting_addr + static_cast<instr_addr>(dst);

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
        runtime::frame& previous_frame = process.top_frame();
        *frame_ptr = &previous_frame;
        *invk_ctx_ptr = &process.top_invocation_ctx();

        previous_frame.set_exc_obj(exc_obj_id);

        process.push_stack(exc_obj_id);

        // Need to search catch sites in the other frames.
        search_catch_sites = true;
      }
    }
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_excobj::execute(
  const instr& /* instr */, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  const frame* frame = *frame_ptr;

  dyobj::dyobj_id exc_obj_id = frame->exc_obj();

  if (!exc_obj_id)
  {
    THROW(invalid_operation_error("No exception raised"));
  }
  else
  {
    process.push_stack(exc_obj_id);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_clrexc::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;
  frame->clear_exc_obj();
}

// -----------------------------------------------------------------------------

void
instr_handler_jmpexc::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  const frame* frame = *frame_ptr;
  dyobj::dyobj_id exc_obj_id = frame->exc_obj();

  bool jump_on_exc = static_cast<bool>(instr.oprd2);

  bool jump = jump_on_exc ? static_cast<bool>(exc_obj_id) : exc_obj_id == 0;

  if (jump)
  {
    instr_addr starting_addr = process.pc();
    instr_addr relative_addr =
      static_cast<instr_addr>(instr.oprd1);

    instr_addr addr = starting_addr + relative_addr;

    if (addr == NONESET_INSTR_ADDR)
    {
      THROW(invalid_instr_addr_error());
    }
    else if (addr < starting_addr)
    {
      THROW(invalid_instr_addr_error());
    }

    process.set_pc(addr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_exit::execute(
  const instr& /* instr */, process& /* process */,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  raise(SIGTERM);
}

// -----------------------------------------------------------------------------

void
instr_handler_putarg::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** invk_ctx_ptr)
{
  dyobj::dyobj_id id = process.pop_stack();
  invocation_ctx* invk_ctx = *invk_ctx_ptr;
  invk_ctx->put_param(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_putkwarg::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** invk_ctx_ptr)
{
  variable_key key = static_cast<variable_key>(instr.oprd1);
  dyobj::dyobj_id id = process.pop_stack();

  invocation_ctx* invk_ctx = *invk_ctx_ptr;
  invk_ctx->put_param_value_pair(key, id);
}

// -----------------------------------------------------------------------------

void
instr_handler_putargs::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** invk_ctx_ptr)
{
  invocation_ctx* invk_ctx = *invk_ctx_ptr;
  dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key key = obj.ntvhndl_key();
  types::native_type_handle& hndl = process.get_ntvhndl(key);

  types::native_array array =
    types::get_value_from_handle<types::native_array>(hndl);

  for (auto itr = array.begin(); itr != array.end(); ++itr)
  {
    dyobj::dyobj_id arg_id = static_cast<dyobj::dyobj_id>(*itr);
    invk_ctx->put_param(arg_id);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_putkwargs::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** invk_ctx_ptr)
{
  invocation_ctx* invk_ctx = *invk_ctx_ptr;
  dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key key = obj.ntvhndl_key();
  types::native_type_handle result = process.get_ntvhndl(key);

  types::native_map map =
    types::get_value_from_handle<types::native_map>(result);

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    variable_key arg_key = static_cast<variable_key>(itr->first);
    dyobj::dyobj_id arg_id = static_cast<dyobj::dyobj_id>(itr->second);

    invk_ctx->put_param_value_pair(arg_key, arg_id);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_getarg::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** invk_ctx_ptr)
{
  invocation_ctx* invk_ctx = *invk_ctx_ptr;
  dyobj::dyobj_id id = invk_ctx->pop_param();
  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
instr_handler_getkwarg::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  invocation_ctx* invk_ctx = *invk_ctx_ptr;
  variable_key key = static_cast<variable_key>(instr.oprd1);

  if (invk_ctx->has_param_value_pair_with_key(key))
  {
    dyobj::dyobj_id id = invk_ctx->pop_param_value_pair(key);

    frame* frame = *frame_ptr;
    frame->set_visible_var(key, id);

    instr_addr relative_addr = static_cast<instr_addr>(instr.oprd2);
    process.set_pc(process.pc() + relative_addr);
  }
}

// -----------------------------------------------------------------------------

void
instr_handler_getargs::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  frame* frame = *frame_ptr;
  invocation_ctx* invk_ctx = *invk_ctx_ptr;

  types::native_array array;

  while (invk_ctx->has_params())
  {
    dyobj::dyobj_id id = invk_ctx->pop_param();
    array.push_back(id);
  }

  types::native_type_handle hndl(std::move(array));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_getkwargs::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  frame* frame = *frame_ptr;
  invocation_ctx* invk_ctx = *invk_ctx_ptr;

  types::native_map map;

  std::vector<variable_key> params = invk_ctx->param_value_pair_keys();

  for (auto itr = params.begin(); itr != params.end(); ++itr)
  {
    variable_key key = static_cast<variable_key>(*itr);
    dyobj::dyobj_id id = invk_ctx->pop_param_value_pair(key);

    auto key_val = static_cast<typename types::native_map::key_type>(key);
    map[key_val] = id;
  }

  types::native_type_handle hndl(std::move(map));

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_hasargs::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** invk_ctx_ptr)
{
  frame* frame = *frame_ptr;
  invocation_ctx* invk_ctx = *invk_ctx_ptr;

  const bool result = invk_ctx->has_params();

  types::native_type_handle hndl( (types::boolean(result)) );

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_gc::execute(
  const instr& /* instr */, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  process.do_gc();
}

// -----------------------------------------------------------------------------

void
instr_handler_debug::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  const uint32_t opts = static_cast<uint32_t>(instr.oprd1);
  process_printer printer(process, opts);
  printer(std::cout) << std::endl;
}

// -----------------------------------------------------------------------------

void
instr_handler_dbgfrm::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  auto& frame = *frame_ptr;

  const uint32_t opts = static_cast<uint32_t>(instr.oprd1);
  frame_printer printer(*frame, opts);
  printer(std::cout) << std::endl;
}

// -----------------------------------------------------------------------------

void
instr_handler_print::execute(
  const instr& instr, process& process,
  frame** /* frame_ptr */, invocation_ctx** /* invk_ctx_ptr */)
{
  dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(native_type_handle_not_found_error());
  }

  types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  types::native_string native_str =
    types::get_value_from_handle<types::native_string>(hndl);

  const std::string& str = static_cast<std::string>(native_str);

  static const char* PRINT_FORMATS[2] = { "%s", "%s\n" };

  bool trailing_new_line = static_cast<bool>(instr.oprd1);
  size_t index = static_cast<size_t>(trailing_new_line) % 2;

  printf(PRINT_FORMATS[index], str.c_str());
}

// -----------------------------------------------------------------------------

void
instr_handler_swap2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;
  frame->swap_eval_stack();
}

// -----------------------------------------------------------------------------

void
instr_handler_pos::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_positive_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_neg::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_negation_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_inc::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_increment_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_dec::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_decrement_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_abs::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_abs_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_sqrt::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_sqrt_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_add::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_addition_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_sub::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_subtraction_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mul::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_multiplication_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_div::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_division_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mod::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_modulus_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_pow::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_pow_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_bnot::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_bitwise_not_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_band::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_bitwise_and_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_bor::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_bitwise_or_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_bxor::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_bitwise_xor_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_bls::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_bitwise_left_shift_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_brs::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_bitwise_right_shift_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_eq::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_eq_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_neq::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_neq_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_gt::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_gt_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_lt::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_lt_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_gte::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_gte_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_lte::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_lte_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_lnot::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_unary_operator_instr(
    *frame_ptr,
    types::interface_apply_logical_not_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_land::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_logical_and_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_lor::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_logical_or_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_cmp::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_binary_operator_instr(
    *frame_ptr,
    types::interface_apply_cmp_operator
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_int8::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::int8>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_uint8::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::uint8>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_int16::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::int16>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_uint16::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::uint16>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_int32::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::int32>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_uint32::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::uint32>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_int64::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::int64>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_uint64::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::uint64>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_bool::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_integer_type_creation_instr<types::boolean>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_dec1::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_floating_type_creation_instr<types::decimal>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_dec2::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_floating_type_creation_instr<types::decimal2>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_str::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  // String type is different than other complex types.
  frame* frame = *frame_ptr;

  std::string str;

  if (instr.oprd1 > 0)
  {
    auto encoding_key = static_cast<runtime::encoding_key>(instr.oprd1);

    const compartment* compartment = frame->compartment_ptr();

    str = compartment->get_encoding_string(encoding_key);
  }

  types::native_type_handle hndl = types::string(str);

  frame->push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
instr_handler_ary::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_complex_type_creation_instr<types::array>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_map::execute(
  const instr& instr, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_complex_type_creation_instr<types::map>(
    instr,
    *frame_ptr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2int8::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_int8
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint8::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_uint8
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2int16::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_int16
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint16::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_uint16
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2int32::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_int32
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint32::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_uint32
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2int64::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_int64
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2uint64::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_uint64
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2bool::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_bool
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2dec1::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_dec1
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2dec2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_dec2
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2str::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_str
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2ary::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_ary
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_2map::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_conversion_instr(
    *frame_ptr,
    types::interface_to_map
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_truthy::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  types::native_type_handle& oprd = frame->top_eval_stack();
  types::native_type_handle result =
    types::interface_compute_truthy_value(oprd);

  frame->push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
instr_handler_repr::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  types::native_type_handle& oprd = frame->top_eval_stack();
  types::native_type_handle result =
    types::interface_compute_repr_value(oprd);

  frame->push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
instr_handler_hash::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  frame* frame = *frame_ptr;

  types::native_type_handle& oprd = frame->top_eval_stack();
  types::native_type_handle result =
    types::interface_compute_hash_value(oprd);

  frame->push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
instr_handler_slice::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands(
    *frame_ptr,
    types::interface_compute_slice
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_stride::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_compute_stride
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_reverse::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_compute_reverse
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_round::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_apply_rounding
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strlen::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_string_get_size
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strat::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_string_at_2
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strclr::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand_in_place(
    *frame_ptr,
    types::interface_string_clear
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strapd::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_string_append
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strpsh::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_string_pushback
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strist::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands_in_place(
    *frame_ptr,
    types::interface_string_insert_str
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strist2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands_in_place(
    *frame_ptr,
    types::interface_string_insert_char
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strers::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_string_erase
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strers2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands_in_place(
    *frame_ptr,
    types::interface_string_erase2
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strrplc::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_four_operands_in_place(
    *frame_ptr,
    types::interface_string_replace_str
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strswp::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_string_swap
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strsub::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_string_substr
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strsub2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands(
    *frame_ptr,
    types::interface_string_substr2
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strfnd::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_string_find
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strfnd2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands(
    *frame_ptr,
    types::interface_string_find2
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strrfnd::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_string_rfind
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_strrfnd2::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands(
    *frame_ptr,
    types::interface_string_rfind2
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_arylen::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_array_size
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryemp::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_array_empty
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryat::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_array_at
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryfrt::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_array_front
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_arybak::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_array_back
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryput::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands_in_place(
    *frame_ptr,
    types::interface_array_put
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryapnd::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_array_append
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryers::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_array_erase
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_arypop::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_array_pop
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryswp::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_array_swap
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_aryclr::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand_in_place(
    *frame_ptr,
    types::interface_array_clear
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_arymrg::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_array_merge
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_maplen::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_map_size
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapemp::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_map_empty
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapat::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_map_at
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapfind::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_map_find
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapput::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_three_operands_in_place(
    *frame_ptr,
    types::interface_map_put
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapset::execute(
  const instr& instr, process& process,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  types::native_map_key_type key = static_cast<
    types::native_map_key_type>(instr.oprd1);

  dyobj::dyobj_id id = process.top_stack();

  frame* frame = *frame_ptr;

  types::native_type_handle& res = frame->top_eval_stack();

  types::native_map map = types::get_value_from_handle<
    types::native_map>(res);

  map[key] = id;

  res = map;
}

// -----------------------------------------------------------------------------

void
instr_handler_mapers::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_map_erase
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapclr::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand_in_place(
    *frame_ptr,
    types::interface_map_clear
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapswp::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands_in_place(
    *frame_ptr,
    types::interface_map_swap
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapkeys::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_map_keys
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapvals::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_single_operand(
    *frame_ptr,
    types::interface_map_vals
  );
}

// -----------------------------------------------------------------------------

void
instr_handler_mapmrg::execute(
  const instr& /* instr */, process& /* process */,
  frame** frame_ptr, invocation_ctx** /* invk_ctx_ptr */)
{
  instr_handler::execute_native_type_complex_instr_with_two_operands(
    *frame_ptr,
    types::interface_map_merge
  );
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
