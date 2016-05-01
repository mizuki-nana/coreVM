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
#ifndef COREVM_INSTRUCTION_H_
#define COREVM_INSTRUCTION_H_

#include "common.h"
#include "errors.h"
#include "instr_fwd.h"
#include "corevm/corevm_bytecode_schema.h" // Compiled.

#include <iosfwd>
#include <string>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

/**
 * Forward declaration of `Frame`.
 */
class Frame;

// -----------------------------------------------------------------------------

/**
 * Forward declaration of `InvocationCtx`.
 */
class InvocationCtx;

// -----------------------------------------------------------------------------

typedef corevm::instr Instr;

// -----------------------------------------------------------------------------

// Forward declaration of `Process`.
class Process;

// -----------------------------------------------------------------------------


/* ------------------------- Object instructions ---------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_new(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_ldobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_stobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_stobjn(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getattr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setattr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_delattr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_hasattr2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getattr2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setattr2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_delattr2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_pop(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_ldobj2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_stobj2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_delobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_delobj2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_gethndl(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_sethndl(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_gethndl2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_clrhndl(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_cpyhndl(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_cpyrepr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_istruthy(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_objeq(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_objneq(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setctx(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_cldobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setattrs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_rsetattrs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setattrs2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_putobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_swap(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setflgc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setfldel(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setflcall(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_setflmute(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ------------------------ Control instructions ---------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_pinvk(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_invk(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_rtrn(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_jmp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_jmpif(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_jmpr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_exc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_excobj(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_clrexc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_jmpexc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_exit(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ------------------------- Function instructions -------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_putarg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_putkwarg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_putargs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_putkwargs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getarg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getkwarg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getargs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_getkwargs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_hasargs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* -------------------------- Runtime instructions -------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_gc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_debug(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_dbgfrm(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_dbgmem(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_print(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_swap2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* -------------------- Arithmetic and logic instructions --------------------*/


// -----------------------------------------------------------------------------

void instr_handler_pos(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_neg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_inc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_dec(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_abs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_sqrt(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_add(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_sub(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mul(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_div(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mod(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_pow(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_bnot(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_band(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_bor(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_bxor(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_bls(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_brs(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_eq(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_neq(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_gt(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_lt(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_gte(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_lte(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_lnot(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_land(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_lor(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_cmp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/*--------------------- Native type creation instructions ------------------- */


// -----------------------------------------------------------------------------

void instr_handler_int8(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_uint8(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_int16(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_uint16(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_int32(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_uint32(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_int64(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_uint64(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_bool(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_dec1(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_dec2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_str(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_ary(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_map(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ------------------ Native type conversion instructions ------------------- */


// -----------------------------------------------------------------------------

void instr_handler_2int8(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2uint8(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2int16(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2uint16(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2int32(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2uint32(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2int64(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2uint64(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2bool(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2dec1(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2dec2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2str(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2ary(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_2map(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ------------------ Native type manipulation instructions ----------------- */


// -----------------------------------------------------------------------------

void instr_handler_truthy(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_repr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_hash(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_slice(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_stride(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_reverse(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_round(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ---------------------- String type instructions -------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_strlen(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strat(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strclr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strapd(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strpsh(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strist(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strist2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strers(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strers2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strrplc(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strswp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strsub(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strsub2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strfnd(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strfnd2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strrfnd(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_strrfnd2(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ----------------------- Array type instructions -------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_arylen(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryemp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryat(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryfrt(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_arybak(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryput(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryapnd(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryers(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_arypop(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryswp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_aryclr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_arymrg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------


/* ----------------------- Map type instructions ---------------------------- */


// -----------------------------------------------------------------------------

void instr_handler_maplen(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapemp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapfind(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapat(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapput(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapset(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapers(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapclr(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapswp(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapkeys(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapvals(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

void instr_handler_mapmrg(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

typedef void InstrHandler(const Instr&, Process&, Frame**, InvocationCtx**);

// -----------------------------------------------------------------------------

class InstrHandlerMeta
{
public:
  static InstrHandler* instr_handlers[INSTR_CODE_MAX];
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTRUCTION_H_ */
