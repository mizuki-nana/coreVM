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
   * Checks whether an identifier type is valid.
   */
  bool check_identifier_type(const IRIdentifierType& type);

  /**
   * Checks whether the use of a custom type is valid.
   */
  bool check_type_string(const std::string&);

  const IRModule& m_module;
  std::unique_ptr<IRModuleIndex> m_index;
  std::string m_msg;
};

} /* end namespace ir */
} /* end namespace corevm */

#endif /* COREVM_IR_VERIFIER_H_ */
