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

namespace corevm {
namespace ir {

corevm::IROpcode
string_to_IROpcode(const std::string& val);

const char* IROpcode_to_string(corevm::IROpcode);

typedef std::pair<std::string, std::string> MetadataPair;

void set_metadata(const MetadataPair&, corevm::IRModule&);

void set_metadata(const std::vector<MetadataPair>&, corevm::IRModule&);

bool operator==(const corevm::IRIdentifierType& lhs, const corevm::IRIdentifierType& rhs);

bool operator!=(const corevm::IRIdentifierType& lhs, const corevm::IRIdentifierType& rhs);

bool operator==(const corevm::IRArrayType& lhs, const corevm::IRArrayType& rhs);

bool operator!=(const corevm::IRArrayType& lhs, const corevm::IRArrayType& rhs);

bool is_integer_type(const corevm::IRValueType&);

corevm::IRIdentifierType get_type_of_instr(const corevm::IRInstruction&);

corevm::IRIdentifierType create_ir_value_type(IRValueType);

corevm::IRIdentifierType create_ir_boolean_value_type();

corevm::IRIdentifierType create_ir_i32_value_type();

corevm::IRIdentifierType create_ir_void_value_type();

} /* end namespace ir */
} /* end namespace corevm */

#endif /* COREVM_FORMAT_UTIL_H_ */
