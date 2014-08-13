#ifndef COREVM_INSTRUCTION_H_
#define COREVM_INSTRUCTION_H_

#include <string>
#include <unordered_map>
#include "common.h"
#include "errors.h"


namespace corevm {


namespace runtime {


/* All the instructions are listed here. */
enum instr_enum {
  /* Object instructions */
  NEW,            // <new,,> Creates a new object and place it on top of the stack.
  LDOBJ,          // <ldobj, key,> Load an object by its key and push it onto stack.
  STOBJ,          // <stobj, key,> Pops the object on top of the stack and stores it with a key into the frame.
  GETATTR,        // <getattr, attr,> Pop the object at the top of the stack, get its attribute and push it onto the stack.
  SETATTR,        // <setattr, attr,> Pop the object at the top of the stack as the attribute, pop the next object as the target, and sets the attribute on the target.
  DELATTR,        // <delattr, attr,> Pop the object at the top of the stack, and deletes its attribute and push it back onto the stack.
  POP,            // <pop,,> Pops the object on top of the stack.
  LDOBJ2,         // <ldobj2, key,> Load an invisible object by a key and push it onto the stack.
  STOBJ2,         // <stobj2, key,> Pops the object on top of the stack and stores it with a key into the frame as an invisible object.
  DELOBJ,         // <delobj, key,> Deletes an object from the current scope.
  DELOBJ2,        // <delobj2, key,> Deletes an invisible object from the current scope.
  GETHNDL,        // <gethndl,,> Copies the native handle of the top object of the stack and push it on top of the eval-stack.
  SETHNDL,        // <sethndl,,> Pops off the native handle off the eval-stack and assigns it to the top object of the stack.
  CLRHNDL,        // <clrhndl,,> Clears the native handle from the top object of the stack.
  OBJEQ,          // <objeq,,> Pops off the top two objects on the stack and tests if they are the same object.
  OBJNEQ,         // <objneq,,> Pops off the top two objects on the stack and tests if they are different objects.

  /* Control instructions */
  RTRN,           // <rtrn,,> Unwinds from the current call frame and jumps to the previous one.
  JMP,            // <jmp, #,> Unconditionally jumps to a particular instruction address.
  JMPIF,          // <jmpif, #,> Conditionally jumps to a particular instruction address only if the top element on the eval stacks evaluates to True.
  EXC,            // <exc,,> Raise an empty exception.
  EXC2,           // <exc2,,> Pop the object at the top and raise it as an exception.
  EXIT,           // <exit, code,> Halts the execution of instructions and exits the program (with an optional exit code).

  /* Function instructions */
  FRAME,          // <frm,,> Creates a new frame and place it on top of the current one.
  PUTARG,         // <putarg,,> Pops the top object off the stack and assign it as the next argument for the next call.
  PUTKWARG,       // <putkwarg, key,> Pops the top object off the stack and assign it as the next keyword-argument for the next call.
  GETARG,         // <getarg,,> Pops off the first argument for the current call and put it on top of the stack.
  GETKWARG,       // <getkwarg, key,> Pops off the keyword-argument for the current call with the specified key and put it on top of the stack.
  GETARGS,        // <getargs,,> Pops off all the arguments for the current call, insert them into a native-list and push it on top of eval-stack.
  GETKWARGS,      // <getkwargs,,> Pops off all the keyword-arguments for the current call, insert them into a native-map and push it on top of eval-stack.

  /* Arithmetic and logic instructions */
  POS,            // <pos,,> Apply the positive operation on the top element on the evaluation stack.
  NEG,            // <neg,,> Apply the negation operation on the top element on the evaluation stack. 
  INC,            // <inc,,> Apply the increment operation on the top element on the evaluation stack.
  DEC,            // <dec,,> Apply the decrement operation on the top element on the evaluation stack.
  ADD,            // <add,,> Pops the top two elements on the eval stack, applies the addition operation and push result onto eval stack.
  SUB,            // <sub,,> Pops the top two elements on the eval stack, applies the subtraction operation and push result onto eval stack.
  MUL,            // <mul,,> Pops the top two elements on the eval stack, applies the multiplication operation and push result onto eval stack.
  DIV,            // <div,,> Pops the top two elements on the eval stack, applies the division operation and push result onto eval stack.
  MOD,            // <mod,,> Pops the top two elements on the eval stack, applies the modulus operation and push result onto eval stack.
  POW,            // <pow,,> Pops the top two elements on the eval stack, applies the power operation and push result onto eval stack.
  BNOT,           // <bnot,,> Applies the bitwise NOT operation on the top element on the evaluation stack.
  BAND,           // <band,,> Pops the top two elements on the eval stack, applies the bitwise AND operation and push result onto eval stack.
  BOR,            // <bor,,> Pops the top two elements on the eval stack, applies the bitwise OR operation and push result onto eval stack.
  BXOR,           // <bxor,,> Pops the top two elements on the eval stack, applies the bitwise XOR operation and push result onto eval stack.
  BLS,            // <bls,,> Pops the top two elements on the eval stack, applies the bitwise left shift operation and push result onto eval stack.
  BRS,            // <brs,,> Pops the top two elements on the eval stack, applies the bitwise right shift operation and push result onto eval stack.
  EQ,             // <eq,,> Pops the top two elements on the eval stack, applies the equality operation and push result onto eval stack.
  NEQ,            // <neq,,> Pops the top two elements on the eval stack, applies the inequality operation and push result onto eval stack.
  GT,             // <gt,,> Pops the top two elements on the eval stack, applies the greater than operation and push result onto eval stack.
  LT,             // <lt,,> Pops the top two elements on the eval stack, applies the less than operation and push result onto eval stack.
  GTE,            // <gte,,> Pops the top two elements on the eval stack, applies the greater or equal to operation and push result onto eval stack.
  LTE,            // <lts,,> Pops the top two elements on the eval stack, applies the less or equal to operation and push result onto eval stack.
  LNOT,           // <lnot,,> Apply the logic NOT operation on the top element on the evaluation stack.        
  LAND,           // <land,,> Pops the top two elements on the eval stack, applies the logical AND operation and push result onto eval stack.
  LOR,            // <lor,,> Pops the top two elements on the eval stack, applies the logical OR operation and push result onto eval stack.

  /* Native type creation instructions */
  INT8,           // <int8, #,> Creates an instance of type `int8` and place it on top of eval stack.
  UINT8,          // <uint8, #,> Creates an instance of type `uint8` and place it on top of eval stack.
  INT16,          // <int16, #,> Creates an instance of type `int16` and place it on top of eval stack.
  UINT16,         // <uint16, #> Creates an instance of type `uint16` and place it on top of eval stack.
  INT32,          // <int32, #> Creates an instance of type `int32` and place it on top of eval stack.
  UINT32,         // <uint32, #> Creates an instance of type `uint32` and place it on top of eval stack.
  INT64,          // <int64, #> Creates an instance of type `int64` and place it on top of eval stack.
  UINT64,         // <uint64, #> Creates an instance of type `uint64` and place it on top of eval stack.
  BOOL,           // <bool, #> Creates an instance of type `bool` and place it on top of eval stack.
  DEC1,           // <dec1, #> Creates an instance of type `dec` and place it on top of eval stack.
  DEC2,           // <dec2, #> Creates an instance of type `dec2` and place it on top of eval stack.
  STR,            // <str, #> Creates an instance of type `str` and place it on top of eval stack.
  ARY,            // <ary,,> Creates an instance of type `array` and place it on top of eval stack.
  MAP,            // <map,,> Creates an instance of type `map` and place it on top of eval stack.

  /* Native type conversion instructions */
  TOINT8,         // <2int8,,> Converts the element on top of the eval stack to type `int8`. 
  TOUINT8,        // <2uint8,,> Converts the element on top of the eval stack to type `uint8`.
  TOINT16,        // <2int16,,> Converts the element on top of the eval stack to type `int16`.
  TOUINT16,       // <2uint16,,> Converts the element on top of the eval stack to type `uint16`.
  TOINT32,        // <2int32,,> Converts the element on top of the eval stack to type `int32`.
  TOUINT32,       // <2uint32,,> Converts the element on top of the eval stack to type `uint32`.
  TOINT64,        // <2int64,,> Converts the element on top of the eval stack to type `int64`.
  TOUINT64,       // <2uint64,,> Converts the element on top of the eval stack to type `uint64`.
  TOBOOL,         // <2bool,,> Converts the element on top of the eval stack to type `bool`.
  TODEC1,         // <2dec1,, > Converts the element on top of the eval stack to type `dec`.
  TODEC2,         // <2dec2,,> Converts the element on top of the eval stack to type `dec2`
  TOSTR,          // <2str,,> Converts the element on top of the eval stack to type `string`.
  TOARY,          // <2ary,,> Converts the element on top of the eval stack to type `array`.
  TOMAP,          // <2map,,> Converts the element on top of the eval stack to type `map`.

  /* Complex type instructions */
  // string type instructions ...
  STRLEN,         // <strlen,,> Pops the top element on the eval stack, and performs the "string size" operation.
  STRCLR,         // <strclr,,> Pops the top element on the eval stack, and performs the "string clear" operation.
  STRAPD,         // <strapd,,> Pops the top two elements on the eval stack, and performs the "string append" operation.
  STRPSH,         // <strpsh,,> Pops the top two elements on the eval stack, and performs the "string pushback" operation.
  STRIST,         // <strist,,> Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  STRIST2,        // <strist2,,> Pops the top three elements on the eval stack, and performs the "string insertion" operation.
  STRERS,         // <strers,,> Pops the top two elements on the eval stack, and performs the "string erase" operation.
  STRERS2,        // <strers2,,> Pops the top two elements on the eval stack, and performs the "string erase" operation.
  STRRPLC,        // <strrplc,,> Pops the top four elements on the eval stack, and performs the "string replace" operation.
  STRSWP,         // <strswp,,> Pops the top two elements on the eval stack, and performs the "string swap" operation.
  STRSUB,         // <strsub,,> Pops the top two elements on the eval stack, and performs the "string substring" operation.
  STRSUB2,        // <strsub2,,> Pops the top three elements on the eval stack, and performs the "string substring" operation.
  STRFND,         // <strfnd,,> Pops the top two elements on the eval stack, and performs the "string find" operation.
  STRFND2,        // <strfnd2,,> Pops the top three elements on the eval stack, and performs the "string find" operation.
  STRRFND,        // <strrfnd,,> Pops the top two elements on the eval stack, and performs the "string rfind" operation.
  STRRFND2,       // <strrfnd2,,> Pops the top three elements on the eval stack, and performs the "string rfind" operation.
  // array type instructions ...
  ARYLEN,         // <arylen,,> Pops the top element on the eval stack, and performs the "array size" operation.
  ARYEMP,         // <aryemp,,> Pops the top element on the eval stack, and performs the "array empty" operation.
  ARYAT,          // <aryat,,> Pops the top two elements on the eval stack, and performs the "array at" operation.
  ARYFRT,         // <aryfrt,,> Pops the top element on the eval stack, and performs the "array front" operation.
  ARYBAK,         // <arybak,,> Pops the top element on the eval stack, and performs the "array back" operation.
  ARYAPND,        // <aryapnd,,> Pops the top two elements on the eval stack, and performs the "array append" operation.
  ARYPOP,         // <arypop,, > Pops the top element on the eval stack, and performs the "array pop" operation.
  ARYSWP,         // <aryswp,,> Pops the top two elements on the eval stack, and performs the "array swap" operation.
  ARYCLR,         // <aryclr,,> Pops the top element on the eval stack, and performs the "array clear" operation.
  // map type instructions ...
  MAPLEN,         // <maplen,,> Pops the top element on the eval stack, and performs the "map size" operation. 
  MAPEMP,         // <mapemp,,> Pops the top element on the eval stack, and performs the "map empty" operation.
  MAPAT,          // <mapat,,> Pops the top two elements on the eval stack, and performs the "map at" operation.
  MAPPUT,         // <mapput,,> Pops the top three elements on the eval stack, and performs the "map put" operation.
  MAPERS,         // <mapers,,> Pops the top element on the eval stack, and performs the "map erase" operation.
  MAPCLR,         // <mapclr,,> Pops the top element on the eval stack, and performs the "map clear" operation.
  MAPSWP,         // <mapswp,,> Pops the top two elements on the eval stack, and performs the "map swap" operation.
};


typedef struct instr {
  corevm::runtime::instr_code code;
  corevm::runtime::instr_oprd oprd1;
  corevm::runtime::instr_oprd oprd2;
} instr;


// Forward declaration of `corevm::runtime::process`.
class process;


class instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&) = 0;
protected:
  template<typename InterfaceFunc>
  void _execute_unary_operator_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void _execute_binary_operator_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename NativeType>
  void _execute_native_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::process&);

  template<typename InterfaceFunc>
  void _execute_native_type_conversion_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void _execute_native_type_complex_instr_1(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void _execute_native_type_complex_instr_2(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void _execute_native_type_complex_instr_3(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void _execute_native_type_complex_instr_4(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);
};


/* Object instructions */
class instr_handler_new : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_lbobj : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_stobj : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_getattr : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_setattr : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_delattr : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_pop : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_ldobj2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_stobj2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_delobj : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_delobj2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_gethndl : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_sethndl : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_clrhndl : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_objeq : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_objneq : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

/* Control instructions */
class instr_handler_rtrn : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_jmp : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_jmpif : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_exc : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_exc2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_exit: public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

/* Function instructions */
class instr_handler_frm : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_putarg : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_putkwarg : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_getarg : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_getkwarg : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_getargs : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_getkwargs : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

/* Arithmetic and logic instructions */
class instr_handler_pos : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_neg : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_inc : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_dec : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_add : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_sub : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mul : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_div : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mod : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_pow : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_bnot : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_band : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_bor : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_bxor : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_bls : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_brs : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_eq : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_neq : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_gt : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_lt : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_gte : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_lte : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_lnot : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_land : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_lor : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

/* Native type creation instructions */
class instr_handler_int8 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_uint8 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_int16 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_uint16 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_int32 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_uint32 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_int64 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_uint64 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_bool : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_dec1 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_dec2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_str : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_ary : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_map : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

/* Native type conversion instructions */
class instr_handler_2int8 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2uint8 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2int16 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2uint16 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2int32 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2uint32 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2int64 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2uint64 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2bool : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2dec1 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2dec2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2str : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2ary : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_2map : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

/* Complex type instructions */
class instr_handler_strlen : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strclr : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strapd : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strpsh : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strist : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strist2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strers : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strers2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strrplc : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strswp : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strsub : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strsub2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strfnd : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strfnd2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strrfnd : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_strrfnd2 : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

class instr_handler_arylen : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_aryemp : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_aryat : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_aryfrt : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_arybak : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_aryapnd : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_arypop : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_aryswp : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_aryclr : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };

class instr_handler_maplen : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mapemp : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mapat : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mapput : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mapers : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mapclr : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };
class instr_handler_mapswp : public instr_handler { public: virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&); };


typedef struct instr_info {
  uint8_t num_oprd;
  const char* str;
  const instr_handler* handler;
} instr_info;


class instr_handler_meta {
public:
  using map_type = typename std::unordered_map<corevm::runtime::instr_code, corevm::runtime::instr_info>;

  std::string instr_to_string(const corevm::runtime::instr&);

  corevm::runtime::instr_info find(corevm::runtime::instr_code instr_code)
    throw(corevm::runtime::invalid_instr_error)
  {
    auto itr = _instr_info_map.find(instr_code);

    if(itr == _instr_info_map.end()) {
      throw corevm::runtime::invalid_instr_error();
    }

    return _instr_info_map.at(instr_code); 
  }

private:
  corevm::runtime::instr_info _validate_instr(const corevm::runtime::instr&) throw(corevm::runtime::invalid_instr_error);

  map_type _instr_info_map {
    /* Object instructions */
    {corevm::runtime::instr_enum::NEW, {.num_oprd=0, .str="new", .handler=new instr_handler_new()}},
    {corevm::runtime::instr_enum::LDOBJ, {.num_oprd=1, .str="ldobj", .handler=new instr_handler_lbobj()}},
    {corevm::runtime::instr_enum::STOBJ, {.num_oprd=1, .str="stobj", .handler=new instr_handler_stobj()}},
    {corevm::runtime::instr_enum::GETATTR, {.num_oprd=1, .str="getattr", .handler=new instr_handler_getattr()}},
    {corevm::runtime::instr_enum::SETATTR, {.num_oprd=1, .str="setattr", .handler=new instr_handler_setattr()}},
    {corevm::runtime::instr_enum::POP, {.num_oprd=0, .str="pop", .handler=new instr_handler_pop()}},
    {corevm::runtime::instr_enum::LDOBJ2, {.num_oprd=1, .str="ldobj2", .handler=new instr_handler_ldobj2()}},
    {corevm::runtime::instr_enum::STOBJ2, {.num_oprd=1, .str="stobj2", .handler=new instr_handler_stobj2()}},
    {corevm::runtime::instr_enum::DELOBJ, {.num_oprd=1, .str="delobj", .handler=new instr_handler_delobj()}},
    {corevm::runtime::instr_enum::DELOBJ2, {.num_oprd=1, .str="delobj2", .handler=new instr_handler_delobj2()}},
    {corevm::runtime::instr_enum::GETHNDL, {.num_oprd=0, .str="gethndl", .handler=new instr_handler_gethndl()}},
    {corevm::runtime::instr_enum::SETHNDL, {.num_oprd=0, .str="sethndl", .handler=new instr_handler_sethndl()}},
    {corevm::runtime::instr_enum::CLRHNDL, {.num_oprd=0, .str="clrhndl", .handler=new instr_handler_clrhndl()}},
    {corevm::runtime::instr_enum::OBJEQ, {.num_oprd=0, .str="objeq", .handler=new instr_handler_objeq()}},
    {corevm::runtime::instr_enum::OBJNEQ, {.num_oprd=0, .str="objneq", .handler=new instr_handler_objneq()}},

    /* Control instructions */
    {corevm::runtime::instr_enum::RTRN, {.num_oprd=0, .str="rtrn", .handler=new instr_handler_rtrn()}},
    {corevm::runtime::instr_enum::JMP, {.num_oprd=1, .str="jmp", .handler=new instr_handler_jmp()}},
    {corevm::runtime::instr_enum::JMPIF, {.num_oprd=1, .str="jmpif", .handler=new instr_handler_jmpif()}},
    {corevm::runtime::instr_enum::EXC, {.num_oprd=0, .str="exc", .handler=new instr_handler_exc()}},
    {corevm::runtime::instr_enum::EXC2, {.num_oprd=0, .str="exc2", .handler=new instr_handler_exc2()}},
    {corevm::runtime::instr_enum::EXIT, {.num_oprd=1, .str="exit", .handler=new instr_handler_exit()}},

    /* Function instructions */
    {corevm::runtime::instr_enum::PUTARG, {.num_oprd=0, .str="putarg", .handler=new instr_handler_putarg()}},
    {corevm::runtime::instr_enum::PUTKWARG, {.num_oprd=1, .str="putkwarg", .handler=new instr_handler_putkwarg()}},
    {corevm::runtime::instr_enum::GETARG, {.num_oprd=0, .str="getarg", .handler=new instr_handler_getarg()}},
    {corevm::runtime::instr_enum::GETKWARG, {.num_oprd=1, .str="getkwarg", .handler=new instr_handler_getkwarg()}},
    {corevm::runtime::instr_enum::GETARGS, {.num_oprd=0, .str="getargs", .handler=new instr_handler_getargs()}},
    {corevm::runtime::instr_enum::GETKWARGS, {.num_oprd=0, .str="getkwargs", .handler=new instr_handler_getkwargs()}},

    /* Arithmetic and logic instructions */
    {corevm::runtime::instr_enum::POS, {.num_oprd=0, .str="pos", .handler=new instr_handler_pos()}},
    {corevm::runtime::instr_enum::NEG, {.num_oprd=0, .str="neg", .handler=new instr_handler_neg()}},
    {corevm::runtime::instr_enum::INC, {.num_oprd=0, .str="inc", .handler=new instr_handler_inc()}},
    {corevm::runtime::instr_enum::DEC, {.num_oprd=0, .str="dec", .handler=new instr_handler_dec()}},
    {corevm::runtime::instr_enum::ADD, {.num_oprd=0, .str="add", .handler=new instr_handler_add()}},
    {corevm::runtime::instr_enum::SUB, {.num_oprd=0, .str="sub", .handler=new instr_handler_sub()}},
    {corevm::runtime::instr_enum::MUL, {.num_oprd=0, .str="mul", .handler=new instr_handler_mul()}},
    {corevm::runtime::instr_enum::DIV, {.num_oprd=0, .str="div", .handler=new instr_handler_div()}},
    {corevm::runtime::instr_enum::MOD, {.num_oprd=0, .str="mod", .handler=new instr_handler_mod()}},
    {corevm::runtime::instr_enum::POW, {.num_oprd=0, .str="pow", .handler=new instr_handler_pow()}},
    {corevm::runtime::instr_enum::BNOT, {.num_oprd=0, .str="bnot", .handler=new instr_handler_bnot()}},
    {corevm::runtime::instr_enum::BAND, {.num_oprd=0, .str="band", .handler=new instr_handler_band()}},
    {corevm::runtime::instr_enum::BOR, {.num_oprd=0, .str="bor", .handler=new instr_handler_bor()}},
    {corevm::runtime::instr_enum::BXOR, {.num_oprd=0, .str="bxor", .handler=new instr_handler_bxor()}},
    {corevm::runtime::instr_enum::BLS, {.num_oprd=0, .str="bls", .handler=new instr_handler_bls()}},
    {corevm::runtime::instr_enum::BRS, {.num_oprd=0, .str="brs", .handler=new instr_handler_brs()}},
    {corevm::runtime::instr_enum::EQ, {.num_oprd=0, .str="eq", .handler=new instr_handler_eq()}},
    {corevm::runtime::instr_enum::NEQ, {.num_oprd=0, .str="neq", .handler=new instr_handler_neq()}},
    {corevm::runtime::instr_enum::GT, {.num_oprd=0, .str="gt", .handler=new instr_handler_gt()}},
    {corevm::runtime::instr_enum::LT, {.num_oprd=0, .str="lt", .handler=new instr_handler_lt()}},
    {corevm::runtime::instr_enum::GTE, {.num_oprd=0, .str="gte", .handler=new instr_handler_gte()}},
    {corevm::runtime::instr_enum::LTE, {.num_oprd=0, .str="lte", .handler=new instr_handler_lte()}},
    {corevm::runtime::instr_enum::LNOT, {.num_oprd=0, .str="lnot", .handler=new instr_handler_lnot()}},
    {corevm::runtime::instr_enum::LAND, {.num_oprd=0, .str="land", .handler=new instr_handler_land()}},
    {corevm::runtime::instr_enum::LOR, {.num_oprd=0, .str="lor", .handler=new instr_handler_lor()}},

    /* Native type creation instructions */
    {corevm::runtime::instr_enum::INT8, {.num_oprd=1, .str="int8", .handler=new instr_handler_int8()}},
    {corevm::runtime::instr_enum::UINT8, {.num_oprd=1, .str="uint8", .handler=new instr_handler_uint8()}},
    {corevm::runtime::instr_enum::INT16, {.num_oprd=1, .str="int16", .handler=new instr_handler_int16()}},
    {corevm::runtime::instr_enum::UINT16, {.num_oprd=1, .str="uint16", .handler=new instr_handler_uint16()}},
    {corevm::runtime::instr_enum::INT32, {.num_oprd=1, .str="int32", .handler=new instr_handler_int32()}},
    {corevm::runtime::instr_enum::UINT32, {.num_oprd=1, .str="uint32", .handler=new instr_handler_uint32()}},
    {corevm::runtime::instr_enum::INT64, {.num_oprd=1, .str="int64", .handler=new instr_handler_int64()}},
    {corevm::runtime::instr_enum::UINT64, {.num_oprd=1, .str="uint64", .handler=new instr_handler_uint64()}},
    {corevm::runtime::instr_enum::BOOL, {.num_oprd=1, .str="bool", .handler=new instr_handler_bool()}},
    {corevm::runtime::instr_enum::DEC1, {.num_oprd=1, .str="dec1", .handler=new instr_handler_dec1()}},
    {corevm::runtime::instr_enum::DEC2, {.num_oprd=1, .str="dec2", .handler=new instr_handler_dec2()}},
    {corevm::runtime::instr_enum::STR, {.num_oprd=1, .str="str", .handler=new instr_handler_str()}},
    {corevm::runtime::instr_enum::ARY, {.num_oprd=0, .str="ary", .handler=new instr_handler_ary()}},
    {corevm::runtime::instr_enum::MAP, {.num_oprd=0, .str="map", .handler=new instr_handler_map()}},

    /* Native type conversion instructions */
    {corevm::runtime::instr_enum::TOINT8, {.num_oprd=0, .str="2int8", .handler=new instr_handler_2int8()}},
    {corevm::runtime::instr_enum::TOUINT8, {.num_oprd=0, .str="2uint8", .handler=new instr_handler_2uint8()}},
    {corevm::runtime::instr_enum::TOINT16, {.num_oprd=0, .str="2int16", .handler=new instr_handler_2int16()}},
    {corevm::runtime::instr_enum::TOUINT16, {.num_oprd=0, .str="2uint16", .handler=new instr_handler_2uint16()}},
    {corevm::runtime::instr_enum::TOINT32, {.num_oprd=0, .str="2int32", .handler=new instr_handler_2int32()}},
    {corevm::runtime::instr_enum::TOUINT32, {.num_oprd=0, .str="2uint32", .handler=new instr_handler_2uint32()}},
    {corevm::runtime::instr_enum::TOINT64, {.num_oprd=1, .str="2int64", .handler=new instr_handler_2int64()}},
    {corevm::runtime::instr_enum::TOUINT64, {.num_oprd=1, .str="2uint64", .handler=new instr_handler_2uint64()}},
    {corevm::runtime::instr_enum::TOBOOL, {.num_oprd=0, .str="2bool", .handler=new instr_handler_2bool()}},
    {corevm::runtime::instr_enum::TODEC1, {.num_oprd=0, .str="2dec1", .handler=new instr_handler_2dec1()}},
    {corevm::runtime::instr_enum::TODEC2, {.num_oprd=0, .str="2dec2", .handler=new instr_handler_2dec2()}},
    {corevm::runtime::instr_enum::TOSTR, {.num_oprd=0, .str="2str", .handler=new instr_handler_2str()}},
    {corevm::runtime::instr_enum::TOARY, {.num_oprd=0, .str="2ary", .handler=new instr_handler_2ary()}},
    {corevm::runtime::instr_enum::TOMAP, {.num_oprd=0, .str="2map", .handler=new instr_handler_2map()}},

    /* Complex type instructions */
    // string type instructions ...
    {corevm::runtime::instr_enum::STRLEN, {.num_oprd=0, .str="strlen", .handler=new instr_handler_strlen()}},
    {corevm::runtime::instr_enum::STRCLR, {.num_oprd=0, .str="strclr", .handler=new instr_handler_strclr()}},
    {corevm::runtime::instr_enum::STRAPD, {.num_oprd=0, .str="strapd", .handler=new instr_handler_strapd()}},
    {corevm::runtime::instr_enum::STRPSH, {.num_oprd=0, .str="strpsh", .handler=new instr_handler_strpsh()}},
    {corevm::runtime::instr_enum::STRIST, {.num_oprd=0, .str="strist", .handler=new instr_handler_strist()}},
    {corevm::runtime::instr_enum::STRIST2, {.num_oprd=0, .str="strist2", .handler=new instr_handler_strist2()}},
    {corevm::runtime::instr_enum::STRERS, {.num_oprd=0, .str="strers", .handler=new instr_handler_strers()}},
    {corevm::runtime::instr_enum::STRERS2, {.num_oprd=0, .str="strers2", .handler=new instr_handler_strers2()}},
    {corevm::runtime::instr_enum::STRRPLC, {.num_oprd=0, .str="strrplc", .handler=new instr_handler_strrplc()}},
    {corevm::runtime::instr_enum::STRSWP, {.num_oprd=0, .str="strswp", .handler=new instr_handler_strswp()}},
    {corevm::runtime::instr_enum::STRSUB, {.num_oprd=0, .str="strsub", .handler=new instr_handler_strsub()}},
    {corevm::runtime::instr_enum::STRSUB2, {.num_oprd=0, .str="strsub2", .handler=new instr_handler_strsub2()}},
    {corevm::runtime::instr_enum::STRFND, {.num_oprd=0, .str="strfnd", .handler=new instr_handler_strfnd()}},
    {corevm::runtime::instr_enum::STRFND2, {.num_oprd=0, .str="strfnd2", .handler=new instr_handler_strfnd2()}},
    {corevm::runtime::instr_enum::STRRFND, {.num_oprd=0, .str="strrfnd", .handler=new instr_handler_strrfnd()}},
    {corevm::runtime::instr_enum::STRRFND2, {.num_oprd=0, .str="strrfnd2", .handler=new instr_handler_strrfnd2()}},
    // array type instructions ...
    {corevm::runtime::instr_enum::ARYLEN, {.num_oprd=0, .str="arylen", .handler=new instr_handler_arylen()}},
    {corevm::runtime::instr_enum::ARYEMP, {.num_oprd=0, .str="aryemp", .handler=new instr_handler_aryemp()}},
    {corevm::runtime::instr_enum::ARYAT, {.num_oprd=0, .str="aryat", .handler=new instr_handler_aryat()}},
    {corevm::runtime::instr_enum::ARYFRT, {.num_oprd=0, .str="aryfrt", .handler=new instr_handler_aryfrt()}},
    {corevm::runtime::instr_enum::ARYBAK, {.num_oprd=0, .str="arybak", .handler=new instr_handler_arybak()}},
    {corevm::runtime::instr_enum::ARYAPND, {.num_oprd=0, .str="aryapnd", .handler=new instr_handler_aryapnd()}},
    {corevm::runtime::instr_enum::ARYPOP, {.num_oprd=0, .str="arypop", .handler=new instr_handler_arypop()}},
    {corevm::runtime::instr_enum::ARYSWP, {.num_oprd=0, .str="aryswp", .handler=new instr_handler_aryswp()}},
    {corevm::runtime::instr_enum::ARYCLR, {.num_oprd=0, .str="aryclr", .handler=new instr_handler_aryclr()}},
    // map type instructions ...
    {corevm::runtime::instr_enum::MAPLEN, {.num_oprd=0, .str="maplen", .handler=new instr_handler_maplen()}},
    {corevm::runtime::instr_enum::MAPEMP, {.num_oprd=0, .str="mapemp", .handler=new instr_handler_mapemp()}},
    {corevm::runtime::instr_enum::MAPAT, {.num_oprd=0, .str="mapat", .handler=new instr_handler_mapat()}},
    {corevm::runtime::instr_enum::MAPPUT, {.num_oprd=0, .str="mapput", .handler=new instr_handler_mapput()}},
    {corevm::runtime::instr_enum::MAPERS, {.num_oprd=0, .str="mapers", .handler=new instr_handler_mapers()}},
    {corevm::runtime::instr_enum::MAPCLR, {.num_oprd=0, .str="mapclr", .handler=new instr_handler_mapclr()}},
    {corevm::runtime::instr_enum::MAPSWP, {.num_oprd=0, .str="mapswp", .handler=new instr_handler_mapswp()}},
  };
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTRUCTION_H_ */
