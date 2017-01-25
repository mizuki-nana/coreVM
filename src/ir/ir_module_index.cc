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
#include "ir_module_index.h"
#include "format.h"
#include "format_util.h"


namespace corevm {
namespace ir {

// -----------------------------------------------------------------------------

IRModuleIndex::IRModuleIndex(const IRModule& module)
  :
  module_indexed(&module),
  type_index(),
  function_index()
{
  init(module);
}

// -----------------------------------------------------------------------------

void
IRModuleIndex::init(const IRModule& module)
{
  init_type_index(module);
  init_function_index(module);
}

// -----------------------------------------------------------------------------

void
IRModuleIndex::init_type_index(const IRModule& module)
{
  for (const auto& type_decl : module.types)
  {
    type_index.insert(std::make_pair(type_decl.name, &type_decl));
  } 
}

// -----------------------------------------------------------------------------

void
IRModuleIndex::init_function_index(const IRModule& module)
{
  for (const auto& closure : module.closures)
  {
    function_index.insert(
      std::make_pair(closure.name, create_func_def_index(closure)));
  }
}

// -----------------------------------------------------------------------------

IRModuleIndex::FunctionDefIndex
IRModuleIndex::create_func_def_index(const IRClosure& closure)
{
  IRModuleIndex::FunctionDefIndex func_def_index;
  func_def_index.closure = &closure;
  func_def_index.identifier_type_index = create_identifier_type_index(closure);
  func_def_index.parameter_index = create_parameter_index(closure);
  func_def_index.bb_index = create_bb_index(closure);
  return func_def_index;
}

// -----------------------------------------------------------------------------

IRModuleIndex::FunctionDefIndex::IdentifierTypeIndex
IRModuleIndex::create_identifier_type_index(const IRClosure& closure)
{
  FunctionDefIndex::IdentifierTypeIndex identifier_type_index;

  for (const auto& parameter : closure.parameters)
  {
    identifier_type_index.insert(
      std::make_pair(parameter.identifier, parameter.type));
  }

  for (const auto& bb : closure.blocks)
  {
    for (const auto& instr : bb.body)
    {
      if (!instr.target.is_null())
      {
        identifier_type_index.insert(std::make_pair(instr.target.get_string(),
          get_type_of_instr(instr)));
      }
    }
  }

  return identifier_type_index;
}

// -----------------------------------------------------------------------------

IRModuleIndex::FunctionDefIndex::ParameterIndex
IRModuleIndex::create_parameter_index(const IRClosure& closure)
{
  FunctionDefIndex::ParameterIndex parameter_index;

  for (const auto& parameter : closure.parameters)
  {
    parameter_index.insert(
      std::make_pair(parameter.identifier, &parameter));
  }

  return parameter_index;
}

// -----------------------------------------------------------------------------

IRModuleIndex::FunctionDefIndex::BasicBlockIndex
IRModuleIndex::create_bb_index(const IRClosure& closure)
{
  FunctionDefIndex::BasicBlockIndex bb_index;

  for (const auto& bb : closure.blocks)
  {
    bb_index.insert(
      std::make_pair(bb.label, create_instr_index(bb)));
  }

  return bb_index;
}

// -----------------------------------------------------------------------------

IRModuleIndex::FunctionDefIndex::InstructionIndex
IRModuleIndex::create_instr_index(const IRBasicBlock& bb)
{
  IRModuleIndex::FunctionDefIndex::InstructionIndex index;

  for (const auto& instr : bb.body)
  {
    if (!instr.target.is_null())
    {
      index.insert(std::make_pair(instr.target.get_string(), &instr));
    }
  }

  return index;
}

// -----------------------------------------------------------------------------

} /* end namespace ir */
} /* end namespace corevm */
