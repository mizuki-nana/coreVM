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

#include "process.h"
#include "corevm/macros.h"
#include "dyobj/util.h"
#include "types/interfaces.h"
#include "types/types.h"

#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


// -----------------------------------------------------------------------------


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

std::ostream& operator<<(
  std::ostream& ost, const corevm::runtime::instr& instr)
{
  ost << std::hex << std::showbase;
  ost << std::setiosflags(std::ios::left);
  ost << std::setw(6) << instr.code << " ";
  ost << std::setw(6) << instr.oprd1 << " ";
  ost << std::setw(6) << instr.oprd2;
  ost << std::resetiosflags(std::ios::adjustfield);
  ost << std::noshowbase << std::dec;
  return ost;
}

// -----------------------------------------------------------------------------

static corevm::dyobj::attr_key
get_attr_key(
  corevm::runtime::process& process,
  corevm::runtime::compartment* compartment,
  corevm::runtime::encoding_key str_key)
{
  std::string attr_str;
  compartment->get_encoding_string(str_key, &attr_str);

  corevm::dyobj::attr_key attr_key = corevm::dyobj::hash_attr_str(attr_str);

  return attr_key;
}

// -----------------------------------------------------------------------------

static corevm::dyobj::attr_key
get_attr_key_from_current_compartment(
  corevm::runtime::process& process,
  corevm::runtime::encoding_key str_key)
{
  const corevm::runtime::frame& frame = process.top_frame();

  return get_attr_key(
    process, frame.compartment_ptr(), str_key);
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */


} /* end namespace corevm */


// -----------------------------------------------------------------------------

const corevm::runtime::instr_info
corevm::runtime::instr_handler_meta::instr_set[INSTR_CODE_MAX] {

  /* -------------------------- Object instructions ------------------------- */

  /* NEW       */    { .num_oprd=0, .str="new",       .handler=std::make_shared<corevm::runtime::instr_handler_new>()       },
  /* LDOBJ     */    { .num_oprd=1, .str="ldobj",     .handler=std::make_shared<corevm::runtime::instr_handler_ldobj>()     },
  /* STOBJ     */    { .num_oprd=1, .str="stobj",     .handler=std::make_shared<corevm::runtime::instr_handler_stobj>()     },
  /* GETATTR   */    { .num_oprd=1, .str="getattr",   .handler=std::make_shared<corevm::runtime::instr_handler_getattr>()   },
  /* SETATTR   */    { .num_oprd=1, .str="setattr",   .handler=std::make_shared<corevm::runtime::instr_handler_setattr>()   },
  /* DELATTR   */    { .num_oprd=1, .str="delattr",   .handler=std::make_shared<corevm::runtime::instr_handler_delattr>()   },
  /* POP       */    { .num_oprd=0, .str="pop",       .handler=std::make_shared<corevm::runtime::instr_handler_pop>()       },
  /* LDOBJ2    */    { .num_oprd=1, .str="ldobj2",    .handler=std::make_shared<corevm::runtime::instr_handler_ldobj2>()    },
  /* STOBJ2    */    { .num_oprd=1, .str="stobj2",    .handler=std::make_shared<corevm::runtime::instr_handler_stobj2>()    },
  /* DELOBJ    */    { .num_oprd=1, .str="delobj",    .handler=std::make_shared<corevm::runtime::instr_handler_delobj>()    },
  /* DELOBJ2   */    { .num_oprd=1, .str="delobj2",   .handler=std::make_shared<corevm::runtime::instr_handler_delobj2>()   },
  /* GETHNDL   */    { .num_oprd=0, .str="gethndl",   .handler=std::make_shared<corevm::runtime::instr_handler_gethndl>()   },
  /* SETHNDL   */    { .num_oprd=0, .str="sethndl",   .handler=std::make_shared<corevm::runtime::instr_handler_sethndl>()   },
  /* GETHNDL2  */    { .num_oprd=0, .str="gethndl2",  .handler=std::make_shared<corevm::runtime::instr_handler_gethndl2>()  },
  /* CLRHNDL   */    { .num_oprd=0, .str="clrhndl",   .handler=std::make_shared<corevm::runtime::instr_handler_clrhndl>()   },
  /* CPYHNDL   */    { .num_oprd=1, .str="cpyhndl",   .handler=std::make_shared<corevm::runtime::instr_handler_cpyhndl>()   },
  /* CPYREPR   */    { .num_oprd=0, .str="cpyrepr",   .handler=std::make_shared<corevm::runtime::instr_handler_cpyrepr>()   },
  /* ISTRUTHY  */    { .num_oprd=0, .str="istruthy",  .handler=std::make_shared<corevm::runtime::instr_handler_istruthy>()  },
  /* OBJEQ     */    { .num_oprd=0, .str="objeq",     .handler=std::make_shared<corevm::runtime::instr_handler_objeq>()     },
  /* OBJNEQ    */    { .num_oprd=0, .str="objneq",    .handler=std::make_shared<corevm::runtime::instr_handler_objneq>()    },
  /* SETCTX    */    { .num_oprd=1, .str="setctx",    .handler=std::make_shared<corevm::runtime::instr_handler_setctx>()    },
  /* CLDOBJ    */    { .num_oprd=2, .str="cldobj",    .handler=std::make_shared<corevm::runtime::instr_handler_cldobj>()    },
  /* SETATTRS  */    { .num_oprd=2, .str="setattrs",  .handler=std::make_shared<corevm::runtime::instr_handler_setattrs>()  },
  /* RSETATTRS */    { .num_oprd=1, .str="rsetattrs", .handler=std::make_shared<corevm::runtime::instr_handler_rsetattrs>() },
  /* SETATTRS2  */   { .num_oprd=1, .str="setattrs2", .handler=std::make_shared<corevm::runtime::instr_handler_setattrs2>() },
  /* PUTOBJ    */    { .num_oprd=0, .str="putobj",    .handler=std::make_shared<corevm::runtime::instr_handler_putobj>()    },
  /* GETOBJ    */    { .num_oprd=0, .str="getobj",    .handler=std::make_shared<corevm::runtime::instr_handler_getobj>()    },
  /* SWAP      */    { .num_oprd=0, .str="swap",      .handler=std::make_shared<corevm::runtime::instr_handler_swap>()      },
  /* SETFLGC   */    { .num_oprd=1, .str="setflgc",   .handler=std::make_shared<corevm::runtime::instr_handler_setflgc>()   },
  /* SETFLDEL  */    { .num_oprd=1, .str="setfldel",  .handler=std::make_shared<corevm::runtime::instr_handler_setfldel>()  },
  /* SETFLCALL */    { .num_oprd=1, .str="setflcall", .handler=std::make_shared<corevm::runtime::instr_handler_setflcall>() },
  /* SETFLMUTE */    { .num_oprd=1, .str="setflmute", .handler=std::make_shared<corevm::runtime::instr_handler_setflmute>() },

  /* -------------------------- Control instructions ------------------------ */

  /* PINVK     */    { .num_oprd=0, .str="pinvk",     .handler=std::make_shared<corevm::runtime::instr_handler_pinvk>()     },
  /* INVK      */    { .num_oprd=0, .str="invk",      .handler=std::make_shared<corevm::runtime::instr_handler_invk>()      },
  /* RTRN      */    { .num_oprd=0, .str="rtrn",      .handler=std::make_shared<corevm::runtime::instr_handler_rtrn>()      },
  /* JMP       */    { .num_oprd=1, .str="jmp",       .handler=std::make_shared<corevm::runtime::instr_handler_jmp>()       },
  /* JMPIF     */    { .num_oprd=1, .str="jmpif",     .handler=std::make_shared<corevm::runtime::instr_handler_jmpif>()     },
  /* JMPR      */    { .num_oprd=1, .str="jmpr",      .handler=std::make_shared<corevm::runtime::instr_handler_jmpr>()      },
  /* EXC       */    { .num_oprd=0, .str="exc",       .handler=std::make_shared<corevm::runtime::instr_handler_exc>()       },
  /* EXCOBJ    */    { .num_oprd=0, .str="excobj",    .handler=std::make_shared<corevm::runtime::instr_handler_excobj>()    },
  /* CLREXC    */    { .num_oprd=0, .str="clrexc",    .handler=std::make_shared<corevm::runtime::instr_handler_clrexc>()    },
  /* JMPEXC    */    { .num_oprd=2, .str="jmpexc",    .handler=std::make_shared<corevm::runtime::instr_handler_jmpexc>()    },
  /* EXIT      */    { .num_oprd=1, .str="exit",      .handler=std::make_shared<corevm::runtime::instr_handler_exit>()      },

  /* ------------------------- Function instructions ------------------------ */

  /* PUTARG    */    { .num_oprd=0, .str="putarg",    .handler=std::make_shared<corevm::runtime::instr_handler_putarg>()    },
  /* PUTKWARG  */    { .num_oprd=1, .str="putkwarg",  .handler=std::make_shared<corevm::runtime::instr_handler_putkwarg>()  },
  /* PUTARGS   */    { .num_oprd=0, .str="putargs",   .handler=std::make_shared<corevm::runtime::instr_handler_putargs>()   },
  /* PUTKWARGS */    { .num_oprd=0, .str="putkwargs", .handler=std::make_shared<corevm::runtime::instr_handler_putkwargs>() },
  /* GETARG    */    { .num_oprd=0, .str="getarg",    .handler=std::make_shared<corevm::runtime::instr_handler_getarg>()    },
  /* GETKWARG  */    { .num_oprd=1, .str="getkwarg",  .handler=std::make_shared<corevm::runtime::instr_handler_getkwarg>()  },
  /* GETARGS   */    { .num_oprd=0, .str="getargs",   .handler=std::make_shared<corevm::runtime::instr_handler_getargs>()   },
  /* GETKWARGS */    { .num_oprd=0, .str="getkwargs", .handler=std::make_shared<corevm::runtime::instr_handler_getkwargs>() },

  /* ------------------------- Runtime instructions ------------------------- */

  /* GC        */    { .num_oprd=0, .str="gc",        .handler=std::make_shared<corevm::runtime::instr_handler_gc>()        },
  /* DEBUG     */    { .num_oprd=0, .str="debug",     .handler=std::make_shared<corevm::runtime::instr_handler_debug>()     },
  /* PRINT     */    { .num_oprd=0, .str="print",     .handler=std::make_shared<corevm::runtime::instr_handler_print>()     },

  /* ---------------- Arithmetic and logic instructions --------------------- */

  /* POS      */     { .num_oprd=0, .str="pos",       .handler=std::make_shared<corevm::runtime::instr_handler_pos>()       },
  /* NEG      */     { .num_oprd=0, .str="neg",       .handler=std::make_shared<corevm::runtime::instr_handler_neg>()       },
  /* INC      */     { .num_oprd=0, .str="inc",       .handler=std::make_shared<corevm::runtime::instr_handler_inc>()       },
  /* DEC      */     { .num_oprd=0, .str="dec",       .handler=std::make_shared<corevm::runtime::instr_handler_dec>()       },
  /* ADD      */     { .num_oprd=0, .str="add",       .handler=std::make_shared<corevm::runtime::instr_handler_add>()       },
  /* SUB      */     { .num_oprd=0, .str="sub",       .handler=std::make_shared<corevm::runtime::instr_handler_sub>()       },
  /* MUL      */     { .num_oprd=0, .str="mul",       .handler=std::make_shared<corevm::runtime::instr_handler_mul>()       },
  /* DIV      */     { .num_oprd=0, .str="div",       .handler=std::make_shared<corevm::runtime::instr_handler_div>()       },
  /* MOD      */     { .num_oprd=0, .str="mod",       .handler=std::make_shared<corevm::runtime::instr_handler_mod>()       },
  /* POW      */     { .num_oprd=0, .str="pow",       .handler=std::make_shared<corevm::runtime::instr_handler_pow>()       },
  /* BNOT     */     { .num_oprd=0, .str="bnot",      .handler=std::make_shared<corevm::runtime::instr_handler_bnot>()      },
  /* BAND     */     { .num_oprd=0, .str="band",      .handler=std::make_shared<corevm::runtime::instr_handler_band>()      },
  /* BOR      */     { .num_oprd=0, .str="bor",       .handler=std::make_shared<corevm::runtime::instr_handler_bor>()       },
  /* BXOR     */     { .num_oprd=0, .str="bxor",      .handler=std::make_shared<corevm::runtime::instr_handler_bxor>()      },
  /* BLS      */     { .num_oprd=0, .str="bls",       .handler=std::make_shared<corevm::runtime::instr_handler_bls>()       },
  /* BRS      */     { .num_oprd=0, .str="brs",       .handler=std::make_shared<corevm::runtime::instr_handler_brs>()       },
  /* EQ       */     { .num_oprd=0, .str="eq",        .handler=std::make_shared<corevm::runtime::instr_handler_eq>()        },
  /* NEQ      */     { .num_oprd=0, .str="neq",       .handler=std::make_shared<corevm::runtime::instr_handler_neq>()       },
  /* GT       */     { .num_oprd=0, .str="gt",        .handler=std::make_shared<corevm::runtime::instr_handler_gt>()        },
  /* LT       */     { .num_oprd=0, .str="lt",        .handler=std::make_shared<corevm::runtime::instr_handler_lt>()        },
  /* GTE      */     { .num_oprd=0, .str="gte",       .handler=std::make_shared<corevm::runtime::instr_handler_gte>()       },
  /* LTE      */     { .num_oprd=0, .str="lte",       .handler=std::make_shared<corevm::runtime::instr_handler_lte>()       },
  /* LNOT     */     { .num_oprd=0, .str="lnot",      .handler=std::make_shared<corevm::runtime::instr_handler_lnot>()      },
  /* LAND     */     { .num_oprd=0, .str="land",      .handler=std::make_shared<corevm::runtime::instr_handler_land>()      },
  /* LOR      */     { .num_oprd=0, .str="lor",       .handler=std::make_shared<corevm::runtime::instr_handler_lor>()       },

  /* ----------------- Native type creation instructions -------------------- */

  /* INT8     */     { .num_oprd=1, .str="int8",      .handler=std::make_shared<corevm::runtime::instr_handler_int8>()      },
  /* UINT8    */     { .num_oprd=1, .str="uint8",     .handler=std::make_shared<corevm::runtime::instr_handler_uint8>()     },
  /* INT16    */     { .num_oprd=1, .str="int16",     .handler=std::make_shared<corevm::runtime::instr_handler_int16>()     },
  /* UINT16   */     { .num_oprd=1, .str="uint16",    .handler=std::make_shared<corevm::runtime::instr_handler_uint16>()    },
  /* INT32    */     { .num_oprd=1, .str="int32",     .handler=std::make_shared<corevm::runtime::instr_handler_int32>()     },
  /* UINT32   */     { .num_oprd=1, .str="uint32",    .handler=std::make_shared<corevm::runtime::instr_handler_uint32>()    },
  /* INT64    */     { .num_oprd=1, .str="int64",     .handler=std::make_shared<corevm::runtime::instr_handler_int64>()     },
  /* UINT64   */     { .num_oprd=1, .str="uint64",    .handler=std::make_shared<corevm::runtime::instr_handler_uint64>()    },
  /* BOOL     */     { .num_oprd=1, .str="bool",      .handler=std::make_shared<corevm::runtime::instr_handler_bool>()      },
  /* DEC1     */     { .num_oprd=1, .str="dec1",      .handler=std::make_shared<corevm::runtime::instr_handler_dec1>()      },
  /* DEC2     */     { .num_oprd=1, .str="dec2",      .handler=std::make_shared<corevm::runtime::instr_handler_dec2>()      },
  /* STR      */     { .num_oprd=1, .str="str",       .handler=std::make_shared<corevm::runtime::instr_handler_str>()       },
  /* ARY      */     { .num_oprd=0, .str="ary",       .handler=std::make_shared<corevm::runtime::instr_handler_ary>()       },
  /* MAP      */     { .num_oprd=0, .str="map",       .handler=std::make_shared<corevm::runtime::instr_handler_map>()       },

  /* ----------------- Native type conversion instructions ------------------ */

  /* TOINT8   */     { .num_oprd=0, .str="2int8",     .handler=std::make_shared<corevm::runtime::instr_handler_2int8>()     },
  /* TOUINT8  */     { .num_oprd=0, .str="2uint8",    .handler=std::make_shared<corevm::runtime::instr_handler_2uint8>()    },
  /* TOINT16  */     { .num_oprd=0, .str="2int16",    .handler=std::make_shared<corevm::runtime::instr_handler_2int16>()    },
  /* TOUINT16 */     { .num_oprd=0, .str="2uint16",   .handler=std::make_shared<corevm::runtime::instr_handler_2uint16>()   },
  /* TOINT32  */     { .num_oprd=0, .str="2int32",    .handler=std::make_shared<corevm::runtime::instr_handler_2int32>()    },
  /* TOUINT32 */     { .num_oprd=0, .str="2uint32",   .handler=std::make_shared<corevm::runtime::instr_handler_2uint32>()   },
  /* TOINT64  */     { .num_oprd=1, .str="2int64",    .handler=std::make_shared<corevm::runtime::instr_handler_2int64>()    },
  /* TOUINT64 */     { .num_oprd=1, .str="2uint64",   .handler=std::make_shared<corevm::runtime::instr_handler_2uint64>()   },
  /* TOBOOL   */     { .num_oprd=0, .str="2bool",     .handler=std::make_shared<corevm::runtime::instr_handler_2bool>()     },
  /* TODEC1   */     { .num_oprd=0, .str="2dec1",     .handler=std::make_shared<corevm::runtime::instr_handler_2dec1>()     },
  /* TODEC2   */     { .num_oprd=0, .str="2dec2",     .handler=std::make_shared<corevm::runtime::instr_handler_2dec2>()     },
  /* TOSTR    */     { .num_oprd=0, .str="2str",      .handler=std::make_shared<corevm::runtime::instr_handler_2str>()      },
  /* TOARY    */     { .num_oprd=0, .str="2ary",      .handler=std::make_shared<corevm::runtime::instr_handler_2ary>()      },
  /* TOMAP    */     { .num_oprd=0, .str="2map",      .handler=std::make_shared<corevm::runtime::instr_handler_2map>()      },

  /* ----------------- Native type manipulation instructions ---------------- */

  /* TRUTHY   */     { .num_oprd=0, .str="truthy",    .handler=std::make_shared<corevm::runtime::instr_handler_truthy>()    },
  /* REPR     */     { .num_oprd=0, .str="repr",      .handler=std::make_shared<corevm::runtime::instr_handler_repr>()      },
  /* HASH     */     { .num_oprd=0, .str="hash",      .handler=std::make_shared<corevm::runtime::instr_handler_hash>()      },

  /* --------------------- String type instructions ------------------------- */

  /* STRLEN   */     { .num_oprd=0, .str="strlen",    .handler=std::make_shared<corevm::runtime::instr_handler_strlen>()    },
  /* STRCLR   */     { .num_oprd=0, .str="strclr",    .handler=std::make_shared<corevm::runtime::instr_handler_strclr>()    },
  /* STRAPD   */     { .num_oprd=0, .str="strapd",    .handler=std::make_shared<corevm::runtime::instr_handler_strapd>()    },
  /* STRPSH   */     { .num_oprd=0, .str="strpsh",    .handler=std::make_shared<corevm::runtime::instr_handler_strpsh>()    },
  /* STRIST   */     { .num_oprd=0, .str="strist",    .handler=std::make_shared<corevm::runtime::instr_handler_strist>()    },
  /* STRIST2  */     { .num_oprd=0, .str="strist2",   .handler=std::make_shared<corevm::runtime::instr_handler_strist2>()   },
  /* STRERS   */     { .num_oprd=0, .str="strers",    .handler=std::make_shared<corevm::runtime::instr_handler_strers>()    },
  /* STRERS2  */     { .num_oprd=0, .str="strers2",   .handler=std::make_shared<corevm::runtime::instr_handler_strers2>()   },
  /* STRRPLC  */     { .num_oprd=0, .str="strrplc",   .handler=std::make_shared<corevm::runtime::instr_handler_strrplc>()   },
  /* STRSWP   */     { .num_oprd=0, .str="strswp",    .handler=std::make_shared<corevm::runtime::instr_handler_strswp>()    },
  /* STRSUB   */     { .num_oprd=0, .str="strsub",    .handler=std::make_shared<corevm::runtime::instr_handler_strsub>()    },
  /* STRSUB2  */     { .num_oprd=0, .str="strsub2",   .handler=std::make_shared<corevm::runtime::instr_handler_strsub2>()   },
  /* STRFND   */     { .num_oprd=0, .str="strfnd",    .handler=std::make_shared<corevm::runtime::instr_handler_strfnd>()    },
  /* STRFND2  */     { .num_oprd=0, .str="strfnd2",   .handler=std::make_shared<corevm::runtime::instr_handler_strfnd2>()   },
  /* STRRFND  */     { .num_oprd=0, .str="strrfnd",   .handler=std::make_shared<corevm::runtime::instr_handler_strrfnd>()   },
  /* STRRFND2 */     { .num_oprd=0, .str="strrfnd2",  .handler=std::make_shared<corevm::runtime::instr_handler_strrfnd2>()  },

  /* --------------------- Array type instructions -------------------------- */

  /* ARYLEN   */     { .num_oprd=0, .str="arylen",    .handler=std::make_shared<corevm::runtime::instr_handler_arylen>()    },
  /* ARYEMP   */     { .num_oprd=0, .str="aryemp",    .handler=std::make_shared<corevm::runtime::instr_handler_aryemp>()    },
  /* ARYAT    */     { .num_oprd=0, .str="aryat",     .handler=std::make_shared<corevm::runtime::instr_handler_aryat>()     },
  /* ARYFRT   */     { .num_oprd=0, .str="aryfrt",    .handler=std::make_shared<corevm::runtime::instr_handler_aryfrt>()    },
  /* ARYBAK   */     { .num_oprd=0, .str="arybak",    .handler=std::make_shared<corevm::runtime::instr_handler_arybak>()    },
  /* ARYPUT   */     { .num_oprd=0, .str="aryput",    .handler=std::make_shared<corevm::runtime::instr_handler_aryput>()    },
  /* ARYAPND  */     { .num_oprd=0, .str="aryapnd",   .handler=std::make_shared<corevm::runtime::instr_handler_aryapnd>()   },
  /* ARYPOP   */     { .num_oprd=0, .str="arypop",    .handler=std::make_shared<corevm::runtime::instr_handler_arypop>()    },
  /* ARYSWP   */     { .num_oprd=0, .str="aryswp",    .handler=std::make_shared<corevm::runtime::instr_handler_aryswp>()    },
  /* ARYCLR   */     { .num_oprd=0, .str="aryclr",    .handler=std::make_shared<corevm::runtime::instr_handler_aryclr>()    },
  /* ARYMRG   */     { .num_oprd=0, .str="arymrg",    .handler=std::make_shared<corevm::runtime::instr_handler_arymrg>()    },

  /* --------------------- Map type instructions ---------------------------- */

  /* MAPLEN   */     { .num_oprd=0, .str="maplen",    .handler=std::make_shared<corevm::runtime::instr_handler_maplen>()    },
  /* MAPEMP   */     { .num_oprd=0, .str="mapemp",    .handler=std::make_shared<corevm::runtime::instr_handler_mapemp>()    },
  /* MAPFIND  */     { .num_oprd=0, .str="mapfind",   .handler=std::make_shared<corevm::runtime::instr_handler_mapfind>()   },
  /* MAPAT    */     { .num_oprd=0, .str="mapat",     .handler=std::make_shared<corevm::runtime::instr_handler_mapat>()     },
  /* MAPPUT   */     { .num_oprd=0, .str="mapput",    .handler=std::make_shared<corevm::runtime::instr_handler_mapput>()    },
  /* MAPSET   */     { .num_oprd=1, .str="mapset",    .handler=std::make_shared<corevm::runtime::instr_handler_mapset>()    },
  /* MAPERS   */     { .num_oprd=0, .str="mapers",    .handler=std::make_shared<corevm::runtime::instr_handler_mapers>()    },
  /* MAPCLR   */     { .num_oprd=0, .str="mapclr",    .handler=std::make_shared<corevm::runtime::instr_handler_mapclr>()    },
  /* MAPSWP   */     { .num_oprd=0, .str="mapswp",    .handler=std::make_shared<corevm::runtime::instr_handler_mapswp>()    },
  /* MAPKEYS  */     { .num_oprd=0, .str="mapkeys",   .handler=std::make_shared<corevm::runtime::instr_handler_mapkeys>()   },
  /* MAPVALS  */     { .num_oprd=0, .str="mapvals",   .handler=std::make_shared<corevm::runtime::instr_handler_mapvals>()   },
  /* MAPMRG   */     { .num_oprd=0, .str="mapmrg",    .handler=std::make_shared<corevm::runtime::instr_handler_mapmrg>()    },

};

// -----------------------------------------------------------------------------


/* --------------------------- INSTRUCTION HANDLERS ------------------------- */


// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_unary_operator_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& oprd = frame.top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_binary_operator_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  size_t eval_stack_size = frame.eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(corevm::runtime::evaluation_stack_empty_error());
  }

  corevm::types::native_type_handle& lhs = frame.eval_stack_element(eval_stack_size - 1);
  corevm::types::native_type_handle& rhs = frame.eval_stack_element(eval_stack_size - 2);

  interface_func(lhs, rhs);

  std::swap(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename NativeType>
void
corevm::runtime::instr_handler::execute_native_integer_type_creation_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = NativeType(instr.oprd1);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename NativeType>
void
corevm::runtime::instr_handler::execute_native_floating_type_creation_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  std::stringstream oprd2_ss;
  oprd2_ss << instr.oprd2;
  std::string oprd2_str = oprd2_ss.str();

  std::reverse(oprd2_str.begin(), oprd2_str.end());

  std::stringstream ss;
  ss << instr.oprd1 << "." << oprd2_str;

  corevm::types::native_type_handle hndl = NativeType(stod(ss.str()));

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename NativeType>
void
corevm::runtime::instr_handler::execute_native_complex_type_creation_instr(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = NativeType();

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_native_type_conversion_instr(
  const corevm::runtime::instr& instr,
  corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& oprd = frame.top_eval_stack();

  interface_func(oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& oprd = frame.top_eval_stack();

  interface_func(oprd, oprd);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  size_t eval_stack_size = frame.eval_stack_size();

  if (eval_stack_size < 2)
  {
    THROW(corevm::runtime::evaluation_stack_empty_error());
  }

  corevm::types::native_type_handle& oprd1 = frame.eval_stack_element(eval_stack_size - 1);
  corevm::types::native_type_handle& oprd2 = frame.eval_stack_element(eval_stack_size - 2);

  interface_func(oprd2, oprd1, oprd1);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  size_t eval_stack_size = frame.eval_stack_size();

  if (eval_stack_size < 3)
  {
    THROW(corevm::runtime::evaluation_stack_empty_error());
  }

  corevm::types::native_type_handle& oprd1 = frame.eval_stack_element(eval_stack_size - 1);
  corevm::types::native_type_handle& oprd2 = frame.eval_stack_element(eval_stack_size - 2);
  corevm::types::native_type_handle& oprd3 = frame.eval_stack_element(eval_stack_size - 3);

  interface_func(oprd3, oprd2, oprd1, oprd1);
}

// -----------------------------------------------------------------------------

template<typename InterfaceFunc>
void
corevm::runtime::instr_handler::execute_native_type_complex_instr_with_four_operands(
  const corevm::runtime::instr& instr, corevm::runtime::process& process, InterfaceFunc interface_func)
{
  corevm::runtime::frame& frame = process.top_frame();

  size_t eval_stack_size = frame.eval_stack_size();

  if (eval_stack_size < 4)
  {
    THROW(corevm::runtime::evaluation_stack_empty_error());
  }

  corevm::types::native_type_handle& oprd1 = frame.eval_stack_element(eval_stack_size - 1);
  corevm::types::native_type_handle& oprd2 = frame.eval_stack_element(eval_stack_size - 2);
  corevm::types::native_type_handle& oprd3 = frame.eval_stack_element(eval_stack_size - 3);
  corevm::types::native_type_handle& oprd4 = frame.eval_stack_element(eval_stack_size - 4);

  interface_func(oprd4, oprd3, oprd2, oprd1, oprd1);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_new::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  auto id = process.create_dyobj();

  process.push_stack(id);
}

// -----------------------------------------------------------------------------


void
corevm::runtime::instr_handler_ldobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);
  corevm::runtime::frame& frame = process.top_frame();

  corevm::runtime::frame* frame_ptr = &frame;

  while (!frame_ptr->has_visible_var(key))
  {
    frame_ptr = corevm::runtime::process::find_parent_frame_in_process(
      frame_ptr, process);

    if (!frame_ptr)
    {
      THROW(corevm::runtime::name_not_found_error());
    }
  }

  auto id = frame_ptr->get_visible_var(key);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_stobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);

  corevm::runtime::frame& frame = process.top_frame();
  corevm::dyobj::dyobj_id id = process.pop_stack();

  frame.set_visible_var(key, id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_getattr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  auto str_key = static_cast<corevm::runtime::encoding_key>(instr.oprd1);
  corevm::dyobj::attr_key attr_key = get_attr_key_from_current_compartment(
    process, str_key);

  corevm::dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);
  corevm::dyobj::dyobj_id attr_id = obj.getattr(attr_key);

  process.push_stack(attr_id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setattr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  auto str_key = static_cast<corevm::runtime::encoding_key>(instr.oprd1);
  corevm::dyobj::attr_key attr_key = get_attr_key_from_current_compartment(
    process, str_key);

  corevm::dyobj::dyobj_id attr_id= process.pop_stack();
  corevm::dyobj::dyobj_id target_id = process.pop_stack();

  auto &obj = process.get_dyobj(target_id);

  if (obj.get_flag(corevm::dyobj::flags::DYOBJ_IS_IMMUTABLE))
  {
    THROW(corevm::runtime::invalid_operation_error(
      str(format("cannot mutate immutable object 0x%08x") % target_id)));
  }

  auto &attr_obj = process.get_dyobj(attr_id);
  obj.putattr(attr_key, attr_id);
  attr_obj.manager().on_setattr();

  process.push_stack(target_id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_delattr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::attr_key attr_key = static_cast<corevm::dyobj::attr_key>(instr.oprd1);

  corevm::dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  if (obj.get_flag(corevm::dyobj::flags::DYOBJ_IS_IMMUTABLE))
  {
    THROW(corevm::runtime::invalid_operation_error(
      str(format("cannot mutate immutable object 0x%08x") % id)));
  }

  corevm::dyobj::dyobj_id attr_id = obj.getattr(attr_key);
  auto &attr_obj = process.get_dyobj(attr_id);
  attr_obj.manager().on_delattr();
  obj.delattr(attr_key);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_pop::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  process.pop_stack();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_ldobj2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);
  corevm::runtime::frame& frame = process.top_frame();

  corevm::runtime::frame* frame_ptr = &frame;

  while (!frame_ptr->has_invisible_var(key))
  {
    frame_ptr = corevm::runtime::process::find_parent_frame_in_process(
      frame_ptr, process);

    if (!frame_ptr)
    {
      THROW(corevm::runtime::name_not_found_error());
    }
  }

  auto id = frame_ptr->get_invisible_var(key);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_stobj2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);

  corevm::runtime::frame& frame = process.top_frame();
  corevm::dyobj::dyobj_id id = process.pop_stack();

  frame.set_invisible_var(key, id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_delobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);
  corevm::runtime::frame& frame = process.top_frame();

  corevm::dyobj::dyobj_id id = frame.pop_visible_var(key);
  auto &obj = process.get_dyobj(id);

  if (obj.get_flag(corevm::dyobj::flags::DYOBJ_IS_INDELIBLE))
  {
    THROW(corevm::runtime::object_deletion_error(id));
  }

  obj.manager().on_delete();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_delobj2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);
  corevm::runtime::frame& frame = process.top_frame();

  corevm::dyobj::dyobj_id id = frame.pop_invisible_var(key);
  auto &obj = process.get_dyobj(id);

  if (obj.get_flag(corevm::dyobj::flags::DYOBJ_IS_INDELIBLE))
  {
    THROW(corevm::runtime::object_deletion_error(id));
  }

  obj.manager().on_delete();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_gethndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_not_found_error());
  }

  corevm::types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  frame.push_eval_stack(hndl);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_sethndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = std::move(frame.pop_eval_stack());

  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key key = obj.ntvhndl_key();

  if (key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    corevm::dyobj::ntvhndl_key new_ntvhndl_key = process.insert_ntvhndl(hndl);

    obj.set_ntvhndl_key(new_ntvhndl_key);
  }
  else
  {
    process.get_ntvhndl(key) = std::move(hndl);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_gethndl2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);

  auto id = frame.get_visible_var(key);
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_not_found_error());
  }

  corevm::types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  frame.push_eval_stack(hndl);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_clrhndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_deletion_error());
  }

  process.erase_ntvhndl(ntvhndl_key);
  obj.clear_ntvhndl_key();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_cpyhndl::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id src_obj_id= process.pop_stack();
  corevm::dyobj::dyobj_id target_obj_id = process.pop_stack();

  auto &src_obj = process.get_dyobj(src_obj_id);
  auto &target_obj = process.get_dyobj(target_obj_id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = src_obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_deletion_error());
  }

  corevm::types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);
  corevm::types::native_type_handle res = hndl;

  uint32_t type = static_cast<uint32_t>(instr.oprd1);

  switch (type)
  {
    case 1:
    {
      corevm::types::interface_to_int8(res);
      break;
    }
    case 2:
    {
      corevm::types::interface_to_uint8(res);
      break;
    }
    case 3:
    {
      corevm::types::interface_to_int16(res);
      break;
    }
    case 4:
    {
      corevm::types::interface_to_uint16(res);
      break;
    }
    case 5:
    {
      corevm::types::interface_to_int32(res);
      break;
    }
    case 6:
    {
      corevm::types::interface_to_uint32(res);
      break;
    }
    case 7:
    {
      corevm::types::interface_to_int64(res);
      break;
    }
    case 8:
    {
      corevm::types::interface_to_uint64(res);
      break;
    }
    case 9:
    {
      corevm::types::interface_to_bool(res);
      break;
    }
    case 10:
    {
      corevm::types::interface_to_dec1(res);
      break;
    }
    case 11:
    {
      corevm::types::interface_to_dec2(res);
      break;
    }
    case 12:
    {
      corevm::types::interface_to_str(res);
      break;
    }
    case 13:
    {
      corevm::types::interface_to_ary(res);
      break;
    }
    case 14:
    {
      corevm::types::interface_to_map(res);
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
corevm::runtime::instr_handler_cpyrepr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id src_obj_id= process.pop_stack();
  corevm::dyobj::dyobj_id target_obj_id = process.pop_stack();

  auto &src_obj = process.get_dyobj(src_obj_id);
  auto &target_obj = process.get_dyobj(target_obj_id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = src_obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_deletion_error());
  }

  corevm::types::native_type_handle hndl = process.get_ntvhndl(ntvhndl_key);
  corevm::types::native_type_handle res;

  corevm::types::interface_compute_repr_value(hndl, res);

  auto new_key = process.insert_ntvhndl(res);
  target_obj.set_ntvhndl_key(new_key);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_istruthy::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::dyobj::dyobj_id obj_id= process.top_stack();
  auto &obj = process.get_dyobj(obj_id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_not_found_error());
  }

  corevm::types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  corevm::types::native_type_handle res;

  corevm::types::interface_compute_truthy_value(hndl ,res);

  frame.push_eval_stack(std::move(res));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_objeq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::dyobj::dyobj_id id1 = process.pop_stack();
  corevm::dyobj::dyobj_id id2 = process.pop_stack();

  corevm::types::native_type_handle hndl = corevm::types::boolean(id1 == id2);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_objneq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::dyobj::dyobj_id id1 = process.pop_stack();
  corevm::dyobj::dyobj_id id2 = process.pop_stack();

  corevm::types::native_type_handle hndl = corevm::types::boolean(id1 != id2);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setctx::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  corevm::runtime::closure_ctx frame_cls = frame.closure_ctx();

  corevm::runtime::closure_ctx ctx {
    .compartment_id = frame_cls.compartment_id,
    .closure_id = static_cast<corevm::runtime::closure_id>(instr.oprd1)
  };

  obj.set_closure_ctx(ctx);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_cldobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = frame.pop_eval_stack();

  bool value = corevm::types::get_value_from_handle<bool>(hndl);

  corevm::runtime::variable_key key1 = static_cast<corevm::runtime::variable_key>(instr.oprd1);
  corevm::runtime::variable_key key2 = static_cast<corevm::runtime::variable_key>(instr.oprd2);

  corevm::runtime::variable_key key = value ? key1 : key2;

  corevm::runtime::frame* frame_ptr = &frame;

  while (!frame_ptr->has_visible_var(key))
  {
    frame_ptr = corevm::runtime::process::find_parent_frame_in_process(
      frame_ptr, process);

    if (!frame_ptr)
    {
      THROW(corevm::runtime::name_not_found_error());
    }
  }

  auto id = frame_ptr->get_visible_var(key);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setattrs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id src_id = process.pop_stack();
  auto& src_obj = process.get_dyobj(src_id);

  corevm::dyobj::dyobj_id id = process.top_stack();
  auto& obj = process.get_dyobj(id);

  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = process.get_ntvhndl(src_obj.ntvhndl_key());
  corevm::types::native_type_handle res = hndl;

  corevm::types::native_map map = corevm::types::get_value_from_handle<
    corevm::types::native_map>(res);

  // If we should clone each mapped object before setting it.
  bool should_clone = static_cast<bool>(instr.oprd1);
  bool should_override_map_values = static_cast<bool>(instr.oprd2);

  auto compartment_id = src_obj.closure_ctx().compartment_id;

  corevm::runtime::compartment* compartment = nullptr;
  process.get_compartment(compartment_id, &compartment);

  if (!compartment)
  {
    THROW(corevm::runtime::compartment_not_found_error(compartment_id));
  }

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    uint64_t str_key = static_cast<uint64_t>(itr->first);

    corevm::dyobj::attr_key attr_key = get_attr_key(
      process, compartment, str_key);

    corevm::dyobj::dyobj_id attr_id = static_cast<corevm::dyobj::dyobj_id>(itr->second);

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
        itr->second = static_cast<corevm::types::native_map_mapped_type>(cloned_attr_id);
      }
    }
    else
    {
      attr_obj.manager().on_setattr();
      obj.putattr(attr_key, attr_id);
    }
  }

  res = map;

  frame.push_eval_stack(res);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_rsetattrs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  auto str_key = static_cast<corevm::runtime::encoding_key>(instr.oprd1);
  corevm::dyobj::attr_key attr_key = get_attr_key_from_current_compartment(
    process, str_key);

  corevm::runtime::frame& frame = process.top_frame();

  corevm::dyobj::dyobj_id attr_id = process.top_stack();
  auto& attr_obj = process.get_dyobj(attr_id);

  corevm::types::native_type_handle& hndl = frame.top_eval_stack();

  corevm::types::native_map map = corevm::types::get_value_from_handle<
    corevm::types::native_map>(hndl);

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    corevm::dyobj::dyobj_id id = static_cast<corevm::dyobj::dyobj_id>(itr->second);

    auto &obj = process.get_dyobj(id);
    attr_obj.manager().on_setattr();
    obj.putattr(attr_key, attr_id);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setattrs2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  auto self_str_key = static_cast<corevm::runtime::encoding_key>(instr.oprd1);
  corevm::dyobj::attr_key self_attr_key = get_attr_key_from_current_compartment(
    process, self_str_key);

  corevm::dyobj::dyobj_id src_id = process.pop_stack();
  auto& src_obj = process.get_dyobj(src_id);

  corevm::dyobj::dyobj_id target_id = process.top_stack();
  auto& target_obj = process.get_dyobj(target_id);

  for (auto itr = src_obj.begin(); itr != src_obj.end(); ++itr)
  {
    corevm::dyobj::attr_key attr_key = static_cast<corevm::dyobj::attr_key>(itr->first);

    corevm::dyobj::dyobj_id attr_id = static_cast<corevm::dyobj::dyobj_id>(itr->second);

    auto &attr_obj = process.get_dyobj(attr_id);

    auto cloned_attr_id = process.create_dyobj();
    auto& cloned_attr_obj = process.get_dyobj(cloned_attr_id);

    cloned_attr_obj.copy_from(attr_obj);
    cloned_attr_obj.manager().on_setattr();
    cloned_attr_obj.putattr(self_attr_key, target_id);

    target_obj.putattr(attr_key, cloned_attr_id);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_putobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle hndl = corevm::types::uint64(id);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_getobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  auto hndl = frame.pop_eval_stack();

  corevm::dyobj::dyobj_id id = corevm::types::get_value_from_handle<
    corevm::dyobj::dyobj_id>(hndl);

  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_swap::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  process.swap_stack();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setflgc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(corevm::dyobj::flags::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }
  else
  {
    obj.clear_flag(corevm::dyobj::flags::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setfldel::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(corevm::dyobj::flags::DYOBJ_IS_INDELIBLE);
  }
  else
  {
    obj.clear_flag(corevm::dyobj::flags::DYOBJ_IS_INDELIBLE);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setflcall::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(corevm::dyobj::flags::DYOBJ_IS_NON_CALLABLE);
  }
  else
  {
    obj.clear_flag(corevm::dyobj::flags::DYOBJ_IS_NON_CALLABLE);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_setflmute::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  bool on_off = static_cast<bool>(instr.oprd1);

  if (on_off)
  {
    obj.set_flag(corevm::dyobj::flags::DYOBJ_IS_IMMUTABLE);
  }
  else
  {
    obj.clear_flag(corevm::dyobj::flags::DYOBJ_IS_IMMUTABLE);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_pinvk::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto& obj = process.get_dyobj(id);

  if (obj.get_flag(corevm::dyobj::flags::DYOBJ_IS_NON_CALLABLE))
  {
    THROW(corevm::runtime::invocation_error(id));
  }

  const corevm::runtime::closure_ctx& ctx = obj.closure_ctx();

  if (ctx.compartment_id == corevm::runtime::NONESET_COMPARTMENT_ID)
  {
    THROW(corevm::runtime::compartment_not_found_error(ctx.compartment_id));
  }

  if (ctx.closure_id == corevm::runtime::NONESET_CLOSURE_ID)
  {
    THROW(corevm::runtime::closure_not_found_error(ctx.closure_id));
  }

  corevm::runtime::compartment* compartment = nullptr;
  process.get_compartment(ctx.compartment_id, &compartment);

  corevm::runtime::closure *closure = nullptr;
  compartment->get_closure_by_id(ctx.closure_id, &closure);

#if __DEBUG__
  ASSERT(compartment);
  ASSERT(closure);
#endif

  process.emplace_invocation_ctx(ctx, compartment, closure);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_invk::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  auto& invk_ctx = process.top_invocation_ctx();

  const corevm::runtime::closure_ctx& ctx = invk_ctx.closure_ctx();
  corevm::runtime::compartment* compartment = invk_ctx.compartment_ptr();
  corevm::runtime::closure* closure = invk_ctx.closure_ptr();

  process.emplace_frame(ctx, compartment, closure, process.pc());
  process.insert_vector(closure->vector);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_rtrn::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::runtime::instr_addr return_addr = frame.return_addr();

  if (return_addr == corevm::runtime::NONESET_INSTR_ADDR)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }

  process.pop_frame();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_jmp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_addr starting_addr = process.pc();
  corevm::runtime::instr_addr relative_addr = static_cast<corevm::runtime::instr_addr>(instr.oprd1);

  corevm::runtime::instr_addr addr = starting_addr + relative_addr;

  if (addr == corevm::runtime::NONESET_INSTR_ADDR)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }
  else if (addr < starting_addr)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }

  process.set_pc(addr);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_jmpif::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::runtime::instr_addr starting_addr = process.pc();
  corevm::runtime::instr_addr relative_addr = static_cast<corevm::runtime::instr_addr>(instr.oprd1);

  corevm::runtime::instr_addr addr = starting_addr + relative_addr;

  if (addr == corevm::runtime::NONESET_INSTR_ADDR)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }
  else if (addr < starting_addr)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }

  corevm::types::native_type_handle& hndl = frame.top_eval_stack();

  bool value = corevm::types::get_value_from_handle<bool>(hndl);

  if (value)
  {
    process.set_pc(addr);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_jmpr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::runtime::instr_addr starting_addr = frame.return_addr() + 1;

  corevm::runtime::instr_addr relative_addr =
    static_cast<corevm::runtime::instr_addr>(instr.oprd1);

  corevm::runtime::instr_addr addr = starting_addr + relative_addr;

  if (addr == corevm::runtime::NONESET_INSTR_ADDR)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }
  else if (addr < starting_addr)
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }

  process.set_pc(addr);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_exc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  bool search_catch_sites = static_cast<bool>(instr.oprd1);

  while (process.has_frame())
  {
    corevm::runtime::frame& frame = process.top_frame();
    corevm::dyobj::dyobj_id exc_obj_id = process.pop_stack();
    corevm::runtime::instr_addr starting_addr = frame.return_addr() + 1;
    uint32_t dst = 0;

    if (search_catch_sites)
    {
      const corevm::runtime::closure *closure = frame.closure_ptr();

#if __DEBUG__
      ASSERT(closure);
#endif

      uint32_t index = process.pc() - starting_addr;

      if (search_catch_sites)
      {
        const auto& catch_sites = closure->catch_sites;

        auto itr = std::find_if(
          catch_sites.begin(),
          catch_sites.end(),
          [&index](const corevm::runtime::catch_site& catch_site) -> bool {
            return index >= catch_site.from && index <= catch_site.to;
          }
        );

        if (itr != catch_sites.end())
        {
          const corevm::runtime::catch_site& catch_site = *itr;

          dst = catch_site.dst;
        }
      }
    }

    if (dst)
    {
      // A catch site found in the current frame. Jump to its destination.
      frame.set_exc_obj(exc_obj_id);

      corevm::runtime::instr_addr addr =
        starting_addr + static_cast<corevm::runtime::instr_addr>(dst);

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
        corevm::runtime::frame& previous_frame = process.top_frame();
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
corevm::runtime::instr_handler_excobj::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  const corevm::runtime::frame& frame = process.top_frame();

  corevm::dyobj::dyobj_id exc_obj_id = frame.exc_obj();

  if (!exc_obj_id)
  {
    THROW(corevm::runtime::invalid_operation_error("No exception raised"));
  }
  else
  {
    process.push_stack(exc_obj_id);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_clrexc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  frame.clear_exc_obj();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_jmpexc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  const corevm::runtime::frame& frame = process.top_frame();
  corevm::dyobj::dyobj_id exc_obj_id = frame.exc_obj();

  bool jump_on_exc = static_cast<bool>(instr.oprd2);

  bool jump = jump_on_exc ? exc_obj_id : !exc_obj_id;

  if (jump)
  {
    corevm::runtime::instr_addr starting_addr = process.pc();
    corevm::runtime::instr_addr relative_addr =
      static_cast<corevm::runtime::instr_addr>(instr.oprd1);

    corevm::runtime::instr_addr addr = starting_addr + relative_addr;

    if (addr == corevm::runtime::NONESET_INSTR_ADDR)
    {
      THROW(corevm::runtime::invalid_instr_addr_error());
    }
    else if (addr < starting_addr)
    {
      THROW(corevm::runtime::invalid_instr_addr_error());
    }

    process.set_pc(addr);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_exit::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  raise(SIGTERM);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_putarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.pop_stack();
  process.top_invocation_ctx().put_param(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_putkwarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);
  corevm::dyobj::dyobj_id id = process.pop_stack();

  process.top_invocation_ctx().put_param_value_pair(key, id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_putargs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::invocation_ctx& invk_ctx = process.top_invocation_ctx();
  corevm::dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key key = obj.ntvhndl_key();
  corevm::types::native_type_handle& hndl = process.get_ntvhndl(key);

  corevm::types::native_array array =
    corevm::types::get_value_from_handle<corevm::types::native_array>(hndl);

  for (auto itr = array.begin(); itr != array.end(); ++itr)
  {
    corevm::dyobj::dyobj_id id = static_cast<corevm::dyobj::dyobj_id>(*itr);
    invk_ctx.put_param(id);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_putkwargs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::invocation_ctx& invk_ctx = process.top_invocation_ctx();
  corevm::dyobj::dyobj_id id = process.pop_stack();
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key key = obj.ntvhndl_key();
  corevm::types::native_type_handle result = process.get_ntvhndl(key);

  corevm::types::native_map map =
    corevm::types::get_value_from_handle<corevm::types::native_map>(result);

  for (auto itr = map.begin(); itr != map.end(); ++itr)
  {
    corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(itr->first);
    corevm::dyobj::dyobj_id id = static_cast<corevm::dyobj::dyobj_id>(itr->second);

    invk_ctx.put_param_value_pair(key, id);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_getarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_invocation_ctx().pop_param();
  process.push_stack(id);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_getkwarg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::invocation_ctx& invk_ctx = process.top_invocation_ctx();
  corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(instr.oprd1);

  if (invk_ctx.has_param_value_pair_with_key(key))
  {
    corevm::dyobj::dyobj_id id = invk_ctx.pop_param_value_pair(key);
    process.push_stack(id);
  }
  else
  {
    instr_addr relative_addr = static_cast<instr_addr>(instr.oprd2);
    process.set_pc(process.pc() + relative_addr);
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_getargs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::runtime::invocation_ctx& invk_ctx = process.top_invocation_ctx();
  corevm::types::native_array array;

  while (invk_ctx.has_params())
  {
    corevm::dyobj::dyobj_id id = invk_ctx.pop_param();
    array.push_back(id);
  }

  corevm::types::native_type_handle hndl = std::move(array);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_getkwargs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();
  corevm::runtime::invocation_ctx& invk_ctx = process.top_invocation_ctx();
  corevm::types::native_map map;

  std::vector<corevm::runtime::variable_key> params = invk_ctx.param_value_pair_keys();

  for (auto itr = params.begin(); itr != params.end(); ++itr)
  {
    corevm::runtime::variable_key key = static_cast<corevm::runtime::variable_key>(*itr);
    corevm::dyobj::dyobj_id id = invk_ctx.pop_param_value_pair(key);

    map[key] = id;
  }

  corevm::types::native_type_handle hndl = std::move(map);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_gc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  process.do_gc();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_debug::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  std::cout << process << std::endl;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_print::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::dyobj::dyobj_id id = process.top_stack();
  auto &obj = process.get_dyobj(id);

  corevm::dyobj::ntvhndl_key ntvhndl_key = obj.ntvhndl_key();

  if (ntvhndl_key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    THROW(corevm::runtime::native_type_handle_not_found_error());
  }

  corevm::types::native_type_handle& hndl = process.get_ntvhndl(ntvhndl_key);

  corevm::types::native_string native_str =
    corevm::types::get_value_from_handle<corevm::types::native_string>(hndl);

  const std::string& str = static_cast<std::string>(native_str);

  puts(str.c_str());
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_pos::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_positive_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_neg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_negation_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_inc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_increment_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_dec::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_decrement_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_add::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_addition_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_sub::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_subtraction_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mul::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_multiplication_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_div::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_division_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mod::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_modulus_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_pow::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_pow_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_bnot::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_not_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_band::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_and_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_bor::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_or_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_bxor::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_xor_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_bls::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_left_shift_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_brs::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_bitwise_right_shift_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_eq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_eq_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_neq::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_neq_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_gt::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_gt_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_lt::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_lt_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_gte::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_gte_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_lte::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_lte_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_lnot::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_unary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_logical_not_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_land::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_logical_and_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_lor::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_binary_operator_instr(
    instr,
    process,
    corevm::types::interface_apply_logical_or_operator
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_int8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::int8>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_uint8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::uint8>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_int16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::int16>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_uint16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::uint16>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_int32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::int32>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_uint32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::uint32>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_int64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::int64>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_uint64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::uint64>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_bool::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_integer_type_creation_instr<corevm::types::boolean>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_dec1::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_floating_type_creation_instr<corevm::types::decimal>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_dec2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_floating_type_creation_instr<corevm::types::decimal2>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_str::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  // String type is different than other complex types.
  corevm::runtime::frame& frame = process.top_frame();

  std::string str;

  if (instr.oprd1 > 0)
  {
    auto encoding_key = static_cast<corevm::runtime::encoding_key>(instr.oprd1);

    const corevm::runtime::compartment* compartment = frame.compartment_ptr();

    str = compartment->get_encoding_string(encoding_key);
  }

  corevm::types::native_type_handle hndl = corevm::types::string(str);

  frame.push_eval_stack(std::move(hndl));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_ary::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_complex_type_creation_instr<corevm::types::array>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_map::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_complex_type_creation_instr<corevm::types::map>(
    instr,
    process
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2int8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int8
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2uint8::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint8
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2int16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int16
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2uint16::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint16
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2int32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int32
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2uint32::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint32
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2int64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_int64
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2uint64::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_uint64
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2bool::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_bool
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2dec1::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_dec1
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2dec2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_dec2
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2str::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_str
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2ary::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_ary
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_2map::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_conversion_instr(
    instr,
    process,
    corevm::types::interface_to_map
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_truthy::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& oprd = frame.top_eval_stack();
  corevm::types::native_type_handle result;

  corevm::types::interface_compute_truthy_value(oprd, result);

  frame.push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_repr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& oprd = frame.top_eval_stack();
  corevm::types::native_type_handle result;

  corevm::types::interface_compute_repr_value(oprd, result);

  frame.push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_hash::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& oprd = frame.top_eval_stack();
  corevm::types::native_type_handle result;

  corevm::types::interface_compute_hash_value(oprd, result);

  frame.push_eval_stack(std::move(result));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strlen::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_string_get_size
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strclr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_string_clear
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strapd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_append
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strpsh::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_pushback
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strist::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_string_insert_str
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strist2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_string_insert_char
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strers::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_erase
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strers2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_string_erase2
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strrplc::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_four_operands(
    instr,
    process,
    corevm::types::interface_string_replace_str
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strswp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_swap
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strsub::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_substr
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strsub2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_string_substr2
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strfnd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_find
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strfnd2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_string_find2
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strrfnd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_string_rfind
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_strrfnd2::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_string_rfind2
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_arylen::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_array_size
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryemp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_array_empty
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryat::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_array_at
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryfrt::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_array_front
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_arybak::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_array_back
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryput::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_array_put
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryapnd::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_array_append
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_arypop::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_array_pop
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryswp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_array_swap
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_aryclr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_array_clear
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_arymrg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_array_merge
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_maplen::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_map_size
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapemp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_map_empty
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapat::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_map_at
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapfind::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_map_find
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapput::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_three_operands(
    instr,
    process,
    corevm::types::interface_map_put
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapset::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::types::native_map_key_type key = static_cast<
    corevm::types::native_map_key_type>(instr.oprd1);

  corevm::dyobj::dyobj_id id = process.top_stack();

  corevm::runtime::frame& frame = process.top_frame();

  corevm::types::native_type_handle& res = frame.top_eval_stack();

  corevm::types::native_map map = corevm::types::get_value_from_handle<
    corevm::types::native_map>(res);

  map[key] = id;

  res = map;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapers::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_map_erase
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapclr::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_map_clear
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapswp::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_map_swap
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapkeys::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_map_keys
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapvals::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_single_operand(
    instr,
    process,
    corevm::types::interface_map_vals
  );
}

// -----------------------------------------------------------------------------

void
corevm::runtime::instr_handler_mapmrg::execute(
  const corevm::runtime::instr& instr, corevm::runtime::process& process)
{
  corevm::runtime::instr_handler::execute_native_type_complex_instr_with_two_operands(
    instr,
    process,
    corevm::types::interface_map_merge
  );
}

// -----------------------------------------------------------------------------
