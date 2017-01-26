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
#ifndef COREVM_FORMAT_UTIL_H_
#define COREVM_FORMAT_UTIL_H_

#include "format.h"
#include <string>
#include <utility>
#include <vector>


/** Utilities for handling coreVM IR entities. */

namespace corevm {
namespace ir {

/**
 * Convert IR opcode string representation to enum value.
 */
corevm::IROpcode
string_to_IROpcode(const std::string& val);

/**
 * Convert IR opcode to string representation.
 */
const char* IROpcode_to_string(corevm::IROpcode);

/**
 * IR module metadata key-value pair.
 */
typedef std::pair<std::string, std::string> MetadataPair;

/**
 * Set a metadata key-value pair to a IR module.
 */
void set_metadata(const MetadataPair&, corevm::IRModule&);

/**
 * Set a set of metadata key-value pair to a IR module.
 */
void set_metadata(const std::vector<MetadataPair>&, corevm::IRModule&);

/**
 * Determines if two IR identifier types are compatible.
 */
bool are_compatible_types(const corevm::IRIdentifierType&,
  const corevm::IRIdentifierType&);

/**
 * Equality check between two IR identifier types.
 */
bool operator==(const corevm::IRIdentifierType&,
  const corevm::IRIdentifierType&);

/**
 * Equality check between two IR identifier types.
 */
bool operator!=(const corevm::IRIdentifierType&,
  const corevm::IRIdentifierType&);

/**
 * Equality check between two IR array types.
 */
bool operator==(const corevm::IRArrayType&, const corevm::IRArrayType&);

/**
 * Equality check between two IR array types.
 */
bool operator!=(const corevm::IRArrayType&, const corevm::IRArrayType&);

/**
 * Determines if the specified IR value type is of 'boolean' value type.
 */
bool is_ir_value_boolean_type(const corevm::IRValueType&);

/**
 * Determines if the specified IR value type is of integer value type.
 */
bool is_ir_value_integer_type(const corevm::IRValueType&);

/**
 * Determines if the specified IR value type is of integer or float-point
 * value type.
 */
bool is_ir_value_numeric_type(const corevm::IRValueType&);

/**
 * Determines if the specified IR value type is of numeric or 'boolean'
 * value type.
 */
bool is_ir_value_numeric_or_boolean_type(const corevm::IRValueType&);

/**
 * Determines if the specified IR value type is of 'string' value type.
 */
bool is_ir_value_string_type(const corevm::IRValueType&);

/**
 * Determines if the specified IR value type is of 'object' value type.
 */
bool is_ir_value_object_type(const corevm::IRValueType&);

/**
 * Get the target identifier type of the specified instruction.
 */
corevm::IRIdentifierType get_type_of_instr(const corevm::IRInstruction&);

/**
 * Creates an instance of identifier type of the specified value type.
 */
corevm::IRIdentifierType create_ir_value_type(IRValueType);

/**
 * Creates an instance of identifier type of 'boolean' value type.
 */
corevm::IRIdentifierType create_ir_boolean_value_type();

/**
 * Creates an instance of identifier type of 'i32' value type.
 */
corevm::IRIdentifierType create_ir_i32_value_type();

/**
 * Creates an instance of identifier type of 'void' value type.
 */
corevm::IRIdentifierType create_ir_void_value_type();

/**
 * Determines whether a type declaration has the specified field.
 */
bool type_decl_has_field(const corevm::IRTypeDecl&, const std::string&);

/**
 * Get the identifier type of a type field.
 */
corevm::IRIdentifierType
get_type_decl_field_type(const corevm::IRTypeDecl&, const std::string&);

} /* end namespace ir */
} /* end namespace corevm */

#endif /* COREVM_FORMAT_UTIL_H_ */
