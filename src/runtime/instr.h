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

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>


namespace corevm {


namespace runtime {


/*
 * coreVM instruction set.
 */
enum instr_enum : uint32_t
{

  /* -------------------------- Object instructions ------------------------- */

  /**
   * <new, _, _>
   * Creates a new object and place it on top of the stack.
   */
  NEW,

  /**
   * <ldobj, key , _>
   * Load an object by its key and push it onto stack.
   */
  LDOBJ,

  /**
   * <stobj, key, _>
   * Pops the object on top of the stack and stores it with a key into
   * the frame.
   */
  STOBJ,

  /**
   * <getattr, attr, _>
   * Pop the object at the top of the stack, get its attribute and push it
   * onto the stack.
   */
  GETATTR,

  /**
   * <setattr, attr, _>
   * Pop the object at the top of the stack as the attribute, pop the next
   * object as the target, and sets the attribute on the target.
   */
  SETATTR,

  /**
   * <delattr, attr, _>
   * Pop the object at the top of the stack, and deletes its attribute and
   * push it back onto the stack.
   */
  DELATTR,

  /**
   * <mute, _, _>
   * Clears the `IS_IMMUTABLE` flag on the object on top of the stack.
   */
  MUTE,

  /**
   * <unmute, _, _>
   * Sets the `IS_IMMUTABLE` flag on the object on top of the stack.
   */
  UNMUTE,

  /**
   * <pop, _, _>
   * Pops the object on top of the stack.
   */
  POP,

  /**
   * <ldobj2, key, _>
   * Load an invisible object by a key and push it onto the stack.
   */
  LDOBJ2,

  /**
   * <stobj2, key, _>
   * Pops the object on top of the stack and stores it with a key into the
   * frame as an invisible object.
   */
  STOBJ2,

  /**
   * <delobj, key, _>
   * Deletes an object from the current scope.
   */
  DELOBJ,

  /**
   * <delobj2, key, _>
   * Deletes an invisible object from the current scope.
   */
  DELOBJ2,

  /**
   * <gethndl, _, _>
   * Copies the native handle of the top object of the stack and push it
   * on top of the eval-stack.
   */
  GETHNDL,

  /**
   * <sethndl, _, _>
   * Pops off the native handle off the eval-stack and assigns it to the top
   * object of the stack.
   */
  SETHNDL,

  /**
   * <clrhndl, _, _>
   * Clears the native handle from the top object of the stack.
   */
  CLRHNDL,

  /**
   * <objeq, _, _>
   * Pops off the top two objects on the stack and tests if they are the
   * same object.
   */
  OBJEQ,

  /**
   * <objneq, _, _>
   * Pops off the top two objects on the stack and tests if they are
   * different objects.
   */
  OBJNEQ,

  /**
   * <setctx, oprd1, _>
   * Sets the closure context of the object.
   * The first operand is the closure ID.
   */
  SETCTX,

  /**
   * <cldobj, oprd1, oprd2>
   * Conditionally loads an object associated with the variable key value
   * represented by either `oprd1` or `oprd2`, by evaluating the boolean
   * equivalent of the object on top of the evaluation stack. Loads `oprd1`
   * if the value evaluates to true, `oprd2` otherwise.
   */
  CLDOBJ,

  /**
   * <setattrs, #, #>
   * Converts the native type handle on top of the eval stack to a native map,
   * and use its key-value pairs as attribute name-value pairs to set on the
   * object on the top of the stack. The first operand is a boolean value
   * specifying whether each mapped object should be cloned before set on the
   * target object. The second operand is a boolean value indicating if
   * the native map values should be overriden with the cloned object IDs.
   */
  SETATTRS,

  /**
   * <rsetattrs, attr, _>
   * Reverse set attributes. Set the object on top of stack as the attribute
   * values onto the objects pointed to as values in the native map equivalent
   * on top of the eval stack.
   */
  RSETATTRS,

  /* ------------------------ Control instructions -------------------------- */

  /**
   * <pinvk, _, _>
   * Prepares the invocation of a function.
   * Creates a new frame on top of the call stack, and sets its closure context
   * using the context of the object on top of the stack.
   */
  PINVK,

  /**
   * <invk, _, _>
   * Invokes the vector of the object on top of the stack.
   */
  INVK,

  /**
   * <rtrn, _, _>
   * Unwinds from the current call frame and jumps to the previous one.
   */
  RTRN,

  /**
   * <jmp, #, _>
   * Unconditionally jumps to a particular instruction address.
   */
  JMP,

  /**
   * <jmpif, #, _>
   * Conditionally jumps to a particular instruction address only if the top
   * element on the eval stacks evaluates to True.
   */
  JMPIF,

  /**
   * <exc, _, _>
   * Raise an empty exception.
   */
  EXC,

  /**
   * <exc2, _, _>
   * Pop the object at the top and raise it as an exception.
   */
  EXC2,

  /**
   * <exit, code, _>
   * Halts the execution of instructions and exits the program
   * (with an optional exit code).
   */
  EXIT,

  /* ----------------------- Function instructions -------------------------- */

  /**
   * <putarg, _, _>
   * Pops the top object off the stack and assign it as the next argument
   * for the next call.
   */
  PUTARG,

  /**
   * <putkwarg, key, _>
   * Pops the top object off the stack and assign it as the next
   * keyword-argument for the next call.
   */
  PUTKWARG,

  /**
   * <putargs, _, _>
   * Pops the top object off the stack, retrieves its native type handle
   * as a native type array, and then iterate through each array element,
   * use it as an object ID to retrieve an object from the heap, and assigns
   * it as the next argument for the next call.
   */
  PUTARGS,

  /**
   * <putkwargs, _, _>
   * Pops the top object off the stack, retrieves its native type handle
   * as a native type map, and then iterate through each key-value pair,
   * use the value as an object ID to retrieve an object from the heap,
   * and use the key as an encoding ID to assign the object as the next
   * keyword-argument for the next call.
   */
  PUTKWARGS,

  /**
   * <getarg, _, _>
   * Pops off the first argument for the current call and put it on
   * top of the stack.
   */
  GETARG,

  /**
   * <getkwarg, key, addr>
   * If the top frame has the keyword-argument pair with the key specified
   * as the first operand, pops off the pair and put the value on top of the
   * stack. Otherwise, advance the program counter by the value specified in the
   * second operand.
   */
  GETKWARG,

  /**
   * <getargs, _, _>
   * Pops off all the arguments for the current call, insert them into a
   * native-list and push it on top of eval-stack.
   */
  GETARGS,

  /**
   * <getkwargs, _, _>
   * Pops off all the keyword-arguments for the current call, insert them into
   * a native-map and push it on top of eval-stack.
   */
  GETKWARGS,

  /* ------------------------- Runtime instructions ------------------------- */

  /**
   * <gc, _, _>
   * Manually performs garbage collection.
   */
  GC,

  /**
   * <debug, _, _>
   * Show debug information.
   */
  DEBUG,

  /**
   * <print, _, _>
   * Converts the native type handle associated with the object on top of the
   * stack into a native string, and prints it to std output.
   */
  PRINT,

  /* ------------------ Arithmetic and logic instructions ------------------- */

  /**
   * <pos, _, _>
   * Apply the positive operation on the top element on the evaluation stack.
   */
  POS,

  /**
   * <neg, _, _>
   * Apply the negation operation on the top element on the evaluation stack.
   */
  NEG,

  /**
   * <inc, _, _>
   * Apply the increment operation on the top element on the evaluation stack.
   */
  INC,

  /**
   * <dec, _, _>
   * Apply the decrement operation on the top element on the evaluation stack.
   */
  DEC,

  /**
   * <add, _, _>
   * Pops the top two elements on the eval stack, applies the addition
   * operation and push result onto eval stack.
   */
  ADD,

  /**
   * <sub, _, _>
   * Pops the top two elements on the eval stack, applies the subtraction
   * operation and push result onto eval stack.
   */
  SUB,

  /**
   * <mul, _, _>
   * Pops the top two elements on the eval stack, applies the multiplication
   * operation and push result onto eval stack.
   */
  MUL,

  /**
   * <div, _, _>
   * Pops the top two elements on the eval stack, applies the division
   * operation and push result onto eval stack.
   */
  DIV,

  /**
   * <mod, _, _>
   * Pops the top two elements on the eval stack, applies the modulus
   * operation and push result onto eval stack.
   */
  MOD,

  /**
   * <pow, _, _>
   * Pops the top two elements on the eval stack, applies the power
   * operation and push result onto eval stack.
   */
  POW,

  /**
   * <bnot, _, _>
   * Applies the bitwise NOT operation on the top element on the evaluation
   * stack.
   */
  BNOT,

  /**
   * <band, _, _>
   * Pops the top two elements on the eval stack, applies the bitwise AND
   * operation and push result onto eval stack.
   */
  BAND,

  /**
   * <bor, _, _>
   * Pops the top two elements on the eval stack, applies the bitwise OR
   * operation and push result onto eval stack.
   */
  BOR,

  /**
   * <bxor, _, _>
   * Pops the top two elements on the eval stack, applies the bitwise XOR
   * operation and push result onto eval stack.
   */
  BXOR,

  /**
   * <bls, _, _>
   * Pops the top two elements on the eval stack, applies the bitwise left shift
   * operation and push result onto eval stack.
   */
  BLS,

  /**
   * <brs, _, _>
   * Pops the top two elements on the eval stack, applies the bitwise right
   * shift operation and push result onto eval stack.
   */
  BRS,

  /**
   * <eq, _, _>
   * Pops the top two elements on the eval stack, applies the equality operation
   * and push result onto eval stack.
   */
  EQ,

  /**
   * <neq, _, _>
   * Pops the top two elements on the eval stack, applies the inequality
   * operation and push result onto eval stack.
   */
  NEQ,

  /**
   * <gt, _, _>
   * Pops the top two elements on the eval stack, applies the greater than
   * operation and push result onto eval stack.
   */
  GT,

  /**
   * <lt, _, _>
   * Pops the top two elements on the eval stack, applies the less than
   * operation and push result onto eval stack.
   */
  LT,

  /**
   * <gte, _, _>
   * Pops the top two elements on the eval stack, applies the greater or
   * equality operation and push result onto eval stack.
   */
  GTE,

  /**
   * <lte, _, _>
   * Pops the top two elements on the eval stack, applies the less or equality
   * operation and push result onto eval stack.
   */
  LTE,

  /**
   * <lnot, _, _>
   * Apply the logic NOT operation on the top element on the evaluation stack.
   */
  LNOT,

  /**
   * <land, _, _>
   * Pops the top two elements on the eval stack, applies the logical AND
   * operation and push result onto eval stack.
   */
  LAND,

  /**
   * <lor, _, _>
   * Pops the top two elements on the eval stack, applies the logical OR
   * operation and push result onto eval stack.
   */
  LOR,

  /* ------------------ Native type creation instructions ------------------- */

  /**
   * <int8, #, _>
   * Creates an instance of type `int8` and place it on top of eval stack.
   */
  INT8,

  /**
   * <uint8, #, _>
   * Creates an instance of type `uint8` and place it on top of eval stack.
   */
  UINT8,

  /**
   * <int16, #, _>
   * Creates an instance of type `int16` and place it on top of eval stack.
   */
  INT16,

  /**
   * <uint16, #, _>
   * Creates an instance of type `uint16` and place it on top of eval stack.
   */
  UINT16,

  /**
   * <int32, #, _>
   * Creates an instance of type `int32` and place it on top of eval stack.
   */
  INT32,

  /**
   * <uint32, #, _>
   * Creates an instance of type `uint32` and place it on top of eval stack.
   */
  UINT32,

  /**
   * <int64, #, _>
   * Creates an instance of type `int64` and place it on top of eval stack.
   */
  INT64,

  /**
   * <uint64, #, _>
   * Creates an instance of type `uint64` and place it on top of eval stack.
   */
  UINT64,

  /**
   * <bool, #, _>
   * Creates an instance of type `bool` and place it on top of eval stack.
   */
  BOOL,

  /**
   * <dec1, #, #>
   * Creates an instance of type `dec` and place it on top of eval stack.
   * The first operand represents the whole number part, while the second
   * operand represents the decimal part, expressed as integer in reverse
   * order.
   */
  DEC1,

  /**
   * <dec2, #, #>
   * Creates an instance of type `dec2` and place it on top of eval stack.
   * The first operand represents the whole number part, while the second
   * operand represents the decimal part, expressed as integer in reverse
   * order.
   */
  DEC2,

  /**
   * <str, #, _>
   * Creates an instance of type `str` and place it on top of eval stack.
   */
  STR,

  /**
   * <ary, _, _>
   * Creates an instance of type `array` and place it on top of eval stack.
   */
  ARY,

  /**
   * <map, _, _>
   * Creates an instance of type `map` and place it on top of eval stack.
   */
  MAP,

  /* ------------------ Native type conversion instructions ----------------- */

  /**
   * <2int8, _, _>
   * Converts the element on top of the eval stack to type `int8`.
   */
  TOINT8,

  /**
   * <2uint8, _, _>
   * Converts the element on top of the eval stack to type `uint8`.
   */
  TOUINT8,

  /**
   * <2int16, _, _>
   * Converts the element on top of the eval stack to type `int16`.
   */
  TOINT16,

  /**
   * <2uint16, _, _>
   * Converts the element on top of the eval stack to type `uint16`.
   */
  TOUINT16,

  /**
   * <2int32, _, _>
   * Converts the element on top of the eval stack to type `int32`.
   */
  TOINT32,

  /**
   * <2uint32, _, _>
   * Converts the element on top of the eval stack to type `uint32`.
   */
  TOUINT32,

  /**
   * <2int64, _, _>
   * Converts the element on top of the eval stack to type `int64`.
   */
  TOINT64,

  /**
   * <2uint64, _, _>
   * Converts the element on top of the eval stack to type `uint64`.
   */
  TOUINT64,

  /**
   * <2bool, _, _>
   * Converts the element on top of the eval stack to type `bool`.
   */
  TOBOOL,

  /**
   * <2dec1,, >
   * Converts the element on top of the eval stack to type `dec`.
   */
  TODEC1,

  /**
   * <2dec2, _, _>
   * Converts the element on top of the eval stack to type `dec2`
   */
  TODEC2,

  /**
   * <2str, _, _>
   * Converts the element on top of the eval stack to type `string`.
   */
  TOSTR,

  /**
   * <2ary, _, _>
   * Converts the element on top of the eval stack to type `array`.
   */
  TOARY,

  /**
   * <2map, _, _>
   * Converts the element on top of the eval stack to type `map`.
   */
  TOMAP,

  /* ----------------- Native type manipulation instructions ---------------- */

  /**
   * <truthy, _, _>
   * Computes a boolean truthy value based on the top element on the eval stack,
   * and puts it on top of the stack.
   */
  TRUTHY,

  /**
   * <repr, _, _>
   * Computes the string equivalent representation of the element on top of the
   * eval stack, and push it on top of the stack.
   */
  REPR,

  /* ---------------------- String type instructions ------------------------ */

  /**
   * <strlen, _, _>
   * Pops the top element on the eval stack, and performs the "string size"
   * operation.
   */
  STRLEN,

  /**
   * <strclr, _, _>
   * Pops the top element on the eval stack, and performs the "string clear"
   * operation.
   */
  STRCLR,

  /**
   * <strapd, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string append" operation.
   */
  STRAPD,

  /**
   * <strpsh, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string pushback" operation.
   */
  STRPSH,

  /**
   * <strist, _, _>
   * Pops the top three elements on the eval stack, and performs the
   * "string insertion" operation.
   */
  STRIST,

  /**
   * <strist2, _, _>
   * Pops the top three elements on the eval stack, and performs the
   * "string insertion" operation.
   */
  STRIST2,

  /**
   * <strers, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string erase" operation.
   */
  STRERS,

  /**
   * <strers2, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string erase" operation.
   */
  STRERS2,

  /**
   * <strrplc, _, _>
   * Pops the top four elements on the eval stack, and performs the
   * "string replace" operation.
   */
  STRRPLC,

  /**
   * <strswp, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string swap" operation.
   */
  STRSWP,

  /**
   * <strsub, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string substring" operation.
   */
  STRSUB,

  /**
   * <strsub2, _, _>
   * Pops the top three elements on the eval stack, and performs the
   * "string substring" operation.
   */
  STRSUB2,

  /**
   * <strfnd, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string find" operation.
   */
  STRFND,

  /**
   * <strfnd2, _, _>
   * Pops the top three elements on the eval stack, and performs the
   * "string find" operation.
   */
  STRFND2,

  /**
   * <strrfnd, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "string rfind" operation.
   */
  STRRFND,

  /**
   * <strrfnd2, _, _>
   * Pops the top three elements on the eval stack, and performs the
   * "string rfind2" operation.
   */
  STRRFND2,

  /* ----------------------- Array type instructions ------------------------ */

  /**
   * <arylen, _, _>
   * Pops the top element on the eval stack, and performs the "array size"
   * operation.
   */
  ARYLEN,

  /**
   * <aryemp, _, _>
   * Pops the top element on the eval stack, and performs the "array empty"
   * operation.
   */
  ARYEMP,

  /**
   * <aryat, _, _>
   * Pops the top two elements on the eval stack, and performs the "array at"
   * operation.
   */
  ARYAT,

  /**
   * <aryfrt, _, _>
   * Pops the top element on the eval stack, and performs the "array front"
   * operation.
   */
  ARYFRT,

  /**
   * <arybak, _, _>
   * Pops the top element on the eval stack, and performs the "array back"
   * operation.
   */
  ARYBAK,

  /**
   * <aryapnd, _, _>
   * Pops the top two elements on the eval stack, and performs the
   * "array append" operation.
   */
  ARYAPND,

  /**
   * <arypop,, >
   * Pops the top element on the eval stack, and performs the "array pop"
   * operation.
   */
  ARYPOP,

  /**
   * <aryswp, _, _>
   * Pops the top two elements on the eval stack, and performs the "array swap"
   * operation.
   */
  ARYSWP,

  /**
   * <aryclr, _, _>
   * Pops the top element on the eval stack, and performs the "array clear"
   * operation.
   */
  ARYCLR,

  /* ------------------------- Map type instructions ------------------------ */

  /**
   * <maplen, _, _>
   * Pops the top element on the eval stack, and performs the "map size"
   * operation.
   */
  MAPLEN,

  /**
   * <mapemp, _, _>
   * Pops the top element on the eval stack, and performs the "map empty"
   * operation.
   */
  MAPEMP,

  /**
   * <mapat, _, _>
   * Pops the top two elements on the eval stack, and performs the "map at"
   * operation.
   */
  MAPAT,

  /**
   * <mapput, _, _>
   * Pops the top three elements on the eval stack, and performs the "map put"
   * operation.
   */
  MAPPUT,

  /**
   * <mapset, key, _>
   * Converts the top element on the eval stack to a native map, and insert a
   * key-value pair into it, with the key represented as the first operand,
   * and the value as the object on top of the stack.
   */
  MAPSET,

  /**
   * <mapers, _, _>
   * Pops the top element on the eval stack, and performs the "map erase"
   * operation.
   */
  MAPERS,

  /**
   * <mapclr, _, _>
   * Pops the top element on the eval stack, and performs the "map clear"
   * operation.
   */
  MAPCLR,

  /**
   * <mapswp, _, _>
   * Pops the top two elements on the eval stack, and performs the "map swap"
   * operation.
   */
  MAPSWP,

};

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

bool operator==(const instr&, const instr&);

// -----------------------------------------------------------------------------

// Forward declaration of `corevm::runtime::process`.
class process;

// -----------------------------------------------------------------------------

class instr_handler
{
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
  void execute_native_integer_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::process&);

  template<typename NativeType>
  void execute_native_floating_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::process&);

  template<typename NativeType>
  void execute_native_complex_type_creation_instr(
    const corevm::runtime::instr&, corevm::runtime::process&);

  template<typename InterfaceFunc>
  void execute_native_type_conversion_instr(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_with_single_operand(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_with_two_operands(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_with_three_operands(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);

  template<typename InterfaceFunc>
  void execute_native_type_complex_instr_with_four_operands(
    const corevm::runtime::instr&, corevm::runtime::process&, InterfaceFunc);
};

// -----------------------------------------------------------------------------


/* ------------------------- Object instructions ---------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_new : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_ldobj : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_stobj : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_getattr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_setattr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_delattr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mute : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_unmute : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_pop : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_ldobj2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_stobj2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_delobj : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_delobj2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_gethndl : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_sethndl : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_clrhndl : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_objeq : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_objneq : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_setctx : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_cldobj : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_setattrs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_rsetattrs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ------------------------ Control instructions ---------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_pinvk : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_invk : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_rtrn : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_jmp : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_jmpif : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_exc : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_exc2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_exit: public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ------------------------- Function instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_putarg : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_putkwarg : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_putargs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_putkwargs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_getarg : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_getkwarg : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_getargs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_getkwargs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* -------------------------- Runtime instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_gc : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_debug : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_print: public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* -------------------- Arithmetic and logic instructions --------------------*/


// -----------------------------------------------------------------------------

class instr_handler_pos : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_neg : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_inc : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_dec : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_add : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_sub : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mul : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_div : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mod : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_pow : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_bnot : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_band : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_bor : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_bxor : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_bls : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_brs : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_eq : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_neq : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_gt : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_lt : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_gte : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_lte : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_lnot : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_land : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_lor : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/*--------------------- Native type creation instructions ------------------- */


// -----------------------------------------------------------------------------

class instr_handler_int8 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_uint8 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_int16 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_uint16 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_int32 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_uint32 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_int64 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_uint64 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_bool : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_dec1 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_dec2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_str : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_ary : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_map : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ------------------ Native type conversion instructions ------------------- */


// -----------------------------------------------------------------------------

class instr_handler_2int8 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint8 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2int16 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint16 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2int32 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint32 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2int64 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2uint64 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2bool : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2dec1 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2dec2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2str : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2ary : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_2map : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ------------------ Native type manipulation instructions ----------------- */


// -----------------------------------------------------------------------------

class instr_handler_truthy : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_repr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ---------------------- String type instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_strlen : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strclr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strapd : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strpsh : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strist : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strist2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strers : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strers2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strrplc : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strswp : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strsub : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strsub2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strfnd : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strfnd2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strrfnd : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_strrfnd2 : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ----------------------- Array type instructions -------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_arylen : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_aryemp : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_aryat : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_aryfrt : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_arybak : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_aryapnd : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_arypop : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_aryswp : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_aryclr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


/* ----------------------- Map type instructions ---------------------------- */


// -----------------------------------------------------------------------------

class instr_handler_maplen : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapemp : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapat : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapput : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapset : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapers : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapclr : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

class instr_handler_mapswp : public instr_handler
{
public:
  virtual void execute(const corevm::runtime::instr&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------

typedef struct instr_info
{
  const uint8_t num_oprd;
  const std::string str;
  const std::shared_ptr<instr_handler> handler;
} instr_info;

// -----------------------------------------------------------------------------

class instr_handler_meta
{
public:
  using map_type = typename std::unordered_map<
    corevm::runtime::instr_code, corevm::runtime::instr_info>;

  static corevm::runtime::instr_info find(corevm::runtime::instr_code instr_code)
    throw(corevm::runtime::invalid_instr_error);

  static const map_type instr_info_map;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTRUCTION_H_ */
