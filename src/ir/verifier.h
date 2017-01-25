/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2017 Yanzheng Li

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
#ifndef COREVM_IR_VERIFIER_H_
#define COREVM_IR_VERIFIER_H_

#include "fwd.h"
#include <string>
#include <memory>
#include <unordered_set>

namespace corevm {
namespace ir {

/* Forward declaration. */
struct IRModuleIndex;

/**
 * Class encapsulating logic that validates the well-formedness and correctness
 * of an IR module.
 */
class Verifier
{
public:
  explicit Verifier(const IRModule&);

  Verifier(const Verifier&) = delete;

  /**
   * Performs verification on the input IR module.
   * Returns a boolean value indicating if the input is valid.
   * Updates the reason of failure to the supplied argument if necessary.
   */
  bool run(std::string&);

  /**
   * Relinquish ownership of the module index to caller.
   */
  void get_index(std::unique_ptr<IRModuleIndex>&);

private:
  /**
   * Checks type definitions in the module. 
   */
  bool check_type_decls();

  /**
   * Checks function definitions in the module.
   */
  bool check_func_defs();

  /**
   * Check whether a function definition is valid in the module.
   */
  bool check_func_def(const IRClosure&);

  struct FuncDefCheckContext
  {
    const IRClosure* closure;
    const IRBasicBlock* bb;
    std::unordered_set<std::string> target_set;
  };

  /**
   * Check whether a basic block is valid in the module.
   */
  bool check_basic_block(const IRBasicBlock&, FuncDefCheckContext&);

  /**
   * Check whether an instruction is valid in the context of its basic block
   * and function definition.
   */
  bool check_instruction(const IRInstruction&, FuncDefCheckContext&);

  /**
   * Check whether an instruction is valid in the context of its basic block
   * and function definition.
   */
  bool check_instruction_dispatch(const IRInstruction&,
    const FuncDefCheckContext&);

  /**
   * Check whether an instruction has the correct number of options.
   */
  bool check_instruction_options_count(
    const IRInstruction&, size_t, const FuncDefCheckContext&);

  /**
   * Check whether an instruction has the correct number of operands.
   */
  bool check_instruction_operands_count(
    const IRInstruction&, size_t, const FuncDefCheckContext&);

  /**
   * Check whether an instruction has the correct number of labels.
   */
  bool check_instruction_labels_count(
    const IRInstruction&, size_t, const FuncDefCheckContext&);

  /**
   * Check whether an instruction operand is valid.
   */
  bool check_operand(const IROperand&, const IRInstruction&,
    FuncDefCheckContext&);

  /**
   * Checks whether an identifier type is valid.
   */
  bool check_identifier_type(const IRIdentifierType& type);

  /**
   * Checks whether the use of a custom type is valid.
   */
  bool check_type_string(const std::string&);

private:
  /** Opcode specific checks. */
  bool check_instr_with_OPCODE_ALLOCA(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_LOAD(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_STORE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_GETATTR(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_SETATTR(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_DELATTR(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_GETELEMENT(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_PUTELEMENT(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_LEN(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_RET(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_BR(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_SWITCH2(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_UNARY_ARITHMETIC_OPCODE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_BINARY_ARITHMETIC_OPCODE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_BNOT(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_BINARY_BITWISE_OPCODE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_BITSHIFT_OPCODE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_EQUALITY_OPCODE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_LNOT(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_BINARY_LOGICAL_OPCODE(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_CMP(const IRInstruction&, const FuncDefCheckContext&);
  bool check_instr_with_OPCODE_CALL(const IRInstruction&, const FuncDefCheckContext&);

private:
  const IRModule& m_module;
  std::unique_ptr<IRModuleIndex> m_index;
  std::string m_msg;
};

} /* end namespace ir */
} /* end namespace corevm */

#endif /* COREVM_IR_VERIFIER_H_ */
