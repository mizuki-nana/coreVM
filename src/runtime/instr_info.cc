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
#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc99-extensions"
#endif

#include "instr_info.h"


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

const InstrInfo InstrSetInfo::instr_infos[INSTR_CODE_MAX] {

  /* -------------------------- Object instructions ------------------------- */

  /* NEW       */    { .name="new"       },
  /* LDOBJ     */    { .name="ldobj"     },
  /* STOBJ     */    { .name="stobj"     },
  /* STOBJN    */    { .name="stobjn"    },
  /* GETATTR   */    { .name="getattr"   },
  /* SETATTR   */    { .name="setattr"   },
  /* DELATTR   */    { .name="delattr"   },
  /* HASATTR2  */    { .name="hasattr2"  },
  /* GETATTR2  */    { .name="getattr2"  },
  /* SETATTR2  */    { .name="setattr2"  },
  /* DELATTR2  */    { .name="delattr2"  },
  /* POP       */    { .name="pop"       },
  /* LDOBJ2    */    { .name="ldobj2"    },
  /* STOBJ2    */    { .name="stobj2"    },
  /* DELOBJ    */    { .name="delobj"    },
  /* DELOBJ2   */    { .name="delobj2"   },
  /* GETHNDL   */    { .name="gethndl"   },
  /* SETHNDL   */    { .name="sethndl"   },
  /* GETHNDL2  */    { .name="gethndl2"  },
  /* CLRHNDL   */    { .name="clrhndl"   },
  /* CPYHNDL   */    { .name="cpyhndl"   },
  /* CPYREPR   */    { .name="cpyrepr"   },
  /* ISTRUTHY  */    { .name="istruthy"  },
  /* OBJEQ     */    { .name="objeq"     },
  /* OBJNEQ    */    { .name="objneq"    },
  /* SETCTX    */    { .name="setctx"    },
  /* CLDOBJ    */    { .name="cldobj"    },
  /* SETATTRS  */    { .name="setattrs"  },
  /* RSETATTRS */    { .name="rsetattrs" },
  /* SETATTRS2  */   { .name="setattrs2" },
  /* PUTOBJ    */    { .name="putobj"    },
  /* GETOBJ    */    { .name="getobj"    },
  /* SWAP      */    { .name="swap"      },
  /* SETFLGC   */    { .name="setflgc"   },
  /* SETFLDEL  */    { .name="setfldel"  },
  /* SETFLCALL */    { .name="setflcall" },
  /* SETFLMUTE */    { .name="setflmute" },

  /* -------------------------- Control instructions ------------------------ */

  /* PINVK     */    { .name="pinvk"     },
  /* INVK      */    { .name="invk"      },
  /* RTRN      */    { .name="rtrn"      },
  /* JMP       */    { .name="jmp"       },
  /* JMPIF     */    { .name="jmpif"     },
  /* JMPR      */    { .name="jmpr"      },
  /* EXC       */    { .name="exc"       },
  /* EXCOBJ    */    { .name="excobj"    },
  /* CLREXC    */    { .name="clrexc"    },
  /* JMPEXC    */    { .name="jmpexc"    },
  /* EXIT      */    { .name="exit"      },

  /* ------------------------- Function instructions ------------------------ */

  /* PUTARG    */    { .name="putarg"    },
  /* PUTKWARG  */    { .name="putkwarg"  },
  /* PUTARGS   */    { .name="putargs"   },
  /* PUTKWARGS */    { .name="putkwargs" },
  /* GETARG    */    { .name="getarg"    },
  /* GETKWARG  */    { .name="getkwarg"  },
  /* GETARGS   */    { .name="getargs"   },
  /* GETKWARGS */    { .name="getkwargs" },
  /* HASARGS   */    { .name="hasargs"   },

  /* ------------------------- Runtime instructions ------------------------- */

  /* GC        */    { .name="gc"        },
  /* DEBUG     */    { .name="debug"     },
  /* DBGFRM    */    { .name="dbgfrm"    },
  /* DBGMEM    */    { .name="dbgmem"    },
  /* DBGVAR    */    { .name="dbgvar"    },
  /* PRINT     */    { .name="print"     },
  /* SWAP2     */    { .name="swap2"     },

  /* ---------------- Arithmetic and logic instructions --------------------- */

  /* POS      */     { .name="pos"       },
  /* NEG      */     { .name="neg"       },
  /* INC      */     { .name="inc"       },
  /* DEC      */     { .name="dec"       },
  /* ABS      */     { .name="abs"       },
  /* SQRT     */     { .name="sqrt"      },
  /* ADD      */     { .name="add"       },
  /* SUB      */     { .name="sub"       },
  /* MUL      */     { .name="mul"       },
  /* DIV      */     { .name="div"       },
  /* MOD      */     { .name="mod"       },
  /* POW      */     { .name="pow"       },
  /* BNOT     */     { .name="bnot"      },
  /* BAND     */     { .name="band"      },
  /* BOR      */     { .name="bor"       },
  /* BXOR     */     { .name="bxor"      },
  /* BLS      */     { .name="bls"       },
  /* BRS      */     { .name="brs"       },
  /* EQ       */     { .name="eq"        },
  /* NEQ      */     { .name="neq"       },
  /* GT       */     { .name="gt"        },
  /* LT       */     { .name="lt"        },
  /* GTE      */     { .name="gte"       },
  /* LTE      */     { .name="lte"       },
  /* LNOT     */     { .name="lnot"      },
  /* LAND     */     { .name="land"      },
  /* LOR      */     { .name="lor"       },
  /* CMP      */     { .name="cmp"       },

  /* ----------------- Native type creation instructions -------------------- */

  /* INT8     */     { .name="int8"      },
  /* UINT8    */     { .name="uint8"     },
  /* INT16    */     { .name="int16"     },
  /* UINT16   */     { .name="uint16"    },
  /* INT32    */     { .name="int32"     },
  /* UINT32   */     { .name="uint32"    },
  /* INT64    */     { .name="int64"     },
  /* UINT64   */     { .name="uint64"    },
  /* BOOL     */     { .name="bool"      },
  /* DEC1     */     { .name="dec1"      },
  /* DEC2     */     { .name="dec2"      },
  /* STR      */     { .name="str"       },
  /* ARY      */     { .name="ary"       },
  /* MAP      */     { .name="map"       },

  /* ----------------- Native type conversion instructions ------------------ */

  /* TOINT8   */     { .name="2int8"     },
  /* TOUINT8  */     { .name="2uint8"    },
  /* TOINT16  */     { .name="2int16"    },
  /* TOUINT16 */     { .name="2uint16"   },
  /* TOINT32  */     { .name="2int32"    },
  /* TOUINT32 */     { .name="2uint32"   },
  /* TOINT64  */     { .name="2int64"    },
  /* TOUINT64 */     { .name="2uint64"   },
  /* TOBOOL   */     { .name="2bool"     },
  /* TODEC1   */     { .name="2dec1"     },
  /* TODEC2   */     { .name="2dec2"     },
  /* TOSTR    */     { .name="2str"      },
  /* TOARY    */     { .name="2ary"      },
  /* TOMAP    */     { .name="2map"      },

  /* ----------------- Native type manipulation instructions ---------------- */

  /* TRUTHY   */     { .name="truthy"    },
  /* REPR     */     { .name="repr"      },
  /* HASH     */     { .name="hash"      },
  /* SLICE    */     { .name="slice"     },
  /* STRIDE   */     { .name="stride"    },
  /* REVERSE  */     { .name="reverse"   },
  /* ROUND    */     { .name="round"     },

  /* --------------------- String type instructions ------------------------- */

  /* STRLEN   */     { .name="strlen"    },
  /* STRAT    */     { .name="strat"     },
  /* STRCLR   */     { .name="strclr"    },
  /* STRAPD   */     { .name="strapd"    },
  /* STRPSH   */     { .name="strpsh"    },
  /* STRIST   */     { .name="strist"    },
  /* STRIST2  */     { .name="strist2"   },
  /* STRERS   */     { .name="strers"    },
  /* STRERS2  */     { .name="strers2"   },
  /* STRRPLC  */     { .name="strrplc"   },
  /* STRSWP   */     { .name="strswp"    },
  /* STRSUB   */     { .name="strsub"    },
  /* STRSUB2  */     { .name="strsub2"   },
  /* STRFND   */     { .name="strfnd"    },
  /* STRFND2  */     { .name="strfnd2"   },
  /* STRRFND  */     { .name="strrfnd"   },
  /* STRRFND2 */     { .name="strrfnd2"  },

  /* --------------------- Array type instructions -------------------------- */

  /* ARYLEN   */     { .name="arylen"    },
  /* ARYEMP   */     { .name="aryemp"    },
  /* ARYAT    */     { .name="aryat"     },
  /* ARYFRT   */     { .name="aryfrt"    },
  /* ARYBAK   */     { .name="arybak"    },
  /* ARYPUT   */     { .name="aryput"    },
  /* ARYAPND  */     { .name="aryapnd"   },
  /* ARYERS   */     { .name="aryers"    },
  /* ARYPOP   */     { .name="arypop"    },
  /* ARYSWP   */     { .name="aryswp"    },
  /* ARYCLR   */     { .name="aryclr"    },
  /* ARYMRG   */     { .name="arymrg"    },

  /* --------------------- Map type instructions ---------------------------- */

  /* MAPLEN   */     { .name="maplen"    },
  /* MAPEMP   */     { .name="mapemp"    },
  /* MAPFIND  */     { .name="mapfind"   },
  /* MAPAT    */     { .name="mapat"     },
  /* MAPPUT   */     { .name="mapput"    },
  /* MAPSET   */     { .name="mapset"    },
  /* MAPERS   */     { .name="mapers"    },
  /* MAPCLR   */     { .name="mapclr"    },
  /* MAPSWP   */     { .name="mapswp"    },
  /* MAPKEYS  */     { .name="mapkeys"   },
  /* MAPVALS  */     { .name="mapvals"   },
  /* MAPMRG   */     { .name="mapmrg"    },

};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif  /* #if defined(__clang__) and __clang__ */
