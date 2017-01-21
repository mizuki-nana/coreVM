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
#ifndef COREVM_JIT_COMPILER_BACKEND_H_
#define COREVM_JIT_COMPILER_BACKEND_H_

#include "runtime_value.h"

#include <string>
#include <vector>


namespace corevm {
namespace jit {

/**
 * Abstract representation of a JIT compilation backend.
 */
class JITCompilerBackend
{
public:
  JITCompilerBackend();
  
  virtual ~JITCompilerBackend();

  /**
   * Performs the necessary initialization work before compilation.
   * Return a boolean value indicating whether the initialization is successful.
   */
  virtual bool init() = 0;

  /**
   * Performs the actual compilation operation of the specified function.
   * Return a boolean value indicating whether the compilation is successful.
   */
  virtual bool run(const std::string& func_name) = 0;

  /**
   * Evaluates a compiled function by specifying a set of arguments, as well as
   * pointer to the value that is to be returned from the function call,
   * if one exists.
   *
   * Returns a boolean value indicating whether the evaluation is successful.
   */
  virtual bool eval_func(const std::vector<RuntimeValue>& args,
    RuntimeValue& result) = 0;

  /**
   * Performs the necessary clean-up work after compilation.
   * Return a boolean value indicating whether the clean-up work is successful.
   */
  virtual bool finalize() = 0;
};

} /* end namespace jit */
} /* end namespace corevm */

#endif /* COREVM_JIT_COMPILER_BACKEND_H_ */
