/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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

#include <string>
#include <unordered_map>
#include "common.h"
#include "errors.h"


namespace corevm {


namespace runtime {


/*
 * All the instructions are listed here.
 */
enum instr_enum {

  /**************************** Object instructions ***************************/

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

  /************************** Control instructions ****************************/

  RTRN,           // <rtrn,,> Unwinds from the current call frame and jumps to the previous one.
  JMP,            // <jmp, #,> Unconditionally jumps to a particular instruction address.
  JMPIF,          // <jmpif, #,> Conditionally jumps to a particular instruction address only if the top element on the eval stacks evaluates to True.
  EXC,            // <exc,,> Raise an empty exception.
  EXC2,           // <exc2,,> Pop the object at the top and raise it as an exception.
  EXIT,           // <exit, code,> Halts the execution of instructions and exits the program (with an optional exit code).

  /************************* Function instructions ****************************/

  FRAME,          // <frm,,> Creates a new frame and place it on top of the current one.
  PUTARG,         // <putarg,,> Pops the top object off the stack and assign it as the next argument for the next call.
  PUTKWARG,       // <putkwarg, key,> Pops the top object off the stack and assign it as the next keyword-argument for the next call.
  GETARG,         // <getarg,,> Pops off the first argument for the current call and put it on top of the stack.
  GETKWARG,       // <getkwarg, key,> Pops off the keyword-argument for the current call with the specified key and put it on top of the stack.
  GETARGS,        // <getargs,,> Pops off all the arguments for the current call, insert them into a native-list and push it on top of eval-stack.
  GETKWARGS,      // <getkwargs,,> Pops off all the keyword-arguments for the current call, insert them into a native-map and push it on top of eval-stack.

  /******************** Arithmetic and logic instructions *********************/

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

  /******************** Native type creation instructions *********************/

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

  /******************** Native type conversion instructions *******************/

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

  /************************* String type instructions *************************/

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

  /************************* Array type instructions **************************/

  ARYLEN,         // <arylen,,> Pops the top element on the eval stack, and performs the "array size" operation.
  ARYEMP,         // <aryemp,,> Pops the top element on the eval stack, and performs the "array empty" operation.
  ARYAT,          // <aryat,,> Pops the top two elements on the eval stack, and performs the "array at" operation.
  ARYFRT,         // <aryfrt,,> Pops the top element on the eval stack, and performs the "array front" operation.
  ARYBAK,         // <arybak,,> Pops the top element on the eval stack, and performs the "array back" operation.
  ARYAPND,        // <aryapnd,,> Pops the top two elements on the eval stack, and performs the "array append" operation.
  ARYPOP,         // <arypop,, > Pops the top element on the eval stack, and performs the "array pop" operation.
  ARYSWP,         // <aryswp,,> Pops the top two elements on the eval stack, and performs the "array swap" operation.
  ARYCLR,         // <aryclr,,> Pops the top element on the eval stack, and performs the "array clear" operation.

  /*************************** Map type instructions **************************/

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
  void execute_unary_operator_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_binary_operator_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename NativeType>
  void execute_native_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::process&);

  template<typename InterfaceFunc>
  void execute_native_type_conversion_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_1(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_2(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_3(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_4(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);
};


/*************************** Object instructions ******************************/


class instr_handler_new : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_lbobj : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_stobj : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_getattr : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_setattr : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_delattr : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_pop : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_ldobj2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_stobj2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_delobj : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_delobj2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_gethndl : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_sethndl : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_clrhndl : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_objeq : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_objneq : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/************************ Control instructions ********************************/


class instr_handler_rtrn : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_jmp : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_jmpif : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_exc : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_exc2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_exit: public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/*************************** Function instructions ****************************/


class instr_handler_frm : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_putarg : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_putkwarg : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_getarg : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_getkwarg : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_getargs : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_getkwargs : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/********************** Arithmetic and logic instructions *********************/


class instr_handler_pos : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_neg : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_inc : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_dec : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_add : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_sub : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mul : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_div : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mod : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_pow : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_bnot : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_band : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_bor : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_bxor : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_bls : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_brs : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_eq : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_neq : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_gt : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_lt : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_gte : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_lte : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_lnot : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_land : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_lor : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/********************** Native type creation instructions *********************/


class instr_handler_int8 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_uint8 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_int16 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_uint16 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_int32 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_uint32 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_int64 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_uint64 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_bool : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_dec1 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_dec2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_str : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_ary : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_map : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/******************** Native type conversion instructions *********************/


class instr_handler_2int8 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2uint8 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2int16 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2uint16 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2int32 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2uint32 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2int64 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2uint64 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2bool : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2dec1 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2dec2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2str : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2ary : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_2map : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/*********************** String type instructions ******************************/


class instr_handler_strlen : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strclr : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strapd : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strpsh : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strist : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strist2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strers : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strers2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strrplc : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strswp : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strsub : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strsub2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strfnd : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strfnd2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strrfnd : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_strrfnd2 : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/************************* Array type instructions ****************************/


class instr_handler_arylen : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_aryemp : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_aryat : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_aryfrt : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_arybak : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_aryapnd : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_arypop : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_aryswp : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_aryclr : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


/************************* Map type instructions ******************************/


class instr_handler_maplen : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mapemp : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mapat : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mapput : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mapers : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mapclr : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


class instr_handler_mapswp : public instr_handler {
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};


typedef struct instr_info {
  uint8_t num_oprd;
  const char* str;
  const instr_handler* handler;
} instr_info;


class instr_handler_meta {
public:
  using map_type = typename std::unordered_map<corevm::runtime::instr_code, corevm::runtime::instr_info>;

  static std::string instr_to_string(const corevm::runtime::instr&);

  static corevm::runtime::instr_info find(corevm::runtime::instr_code instr_code)
    throw(corevm::runtime::invalid_instr_error);

  static corevm::runtime::instr_info validate_instr(const corevm::runtime::instr&)
    throw(corevm::runtime::invalid_instr_error);

private:
  static const map_type instr_info_map;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTRUCTION_H_ */
