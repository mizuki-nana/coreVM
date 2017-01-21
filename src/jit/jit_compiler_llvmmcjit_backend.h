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
#ifndef COREVM_JIT_COMPILER_LLVM_MCJIT_BACKEND_H_
#define COREVM_JIT_COMPILER_LLVM_MCJIT_BACKEND_H_

#include "jit_compiler_backend.h"
#include <llvm/IR/Module.h>


namespace corevm {
namespace jit {

/**
 * JIT backend based on LLVM's MCJIT framework.
 */
class JITCompilerLLVMMCJITBackend : public JITCompilerBackend
{
public:
  typedef llvm::Module ModuleType;

  explicit JITCompilerLLVMMCJITBackend(const ModuleType&);

  virtual ~JITCompilerLLVMMCJITBackend();

  /**
   * Implements `JITCompilerBackend::init()`.
   */
  virtual bool init();

  /**
   * Implements `JITCompilerBackend::run()`.
   */
  virtual bool run(const std::string& func_name);

  /**
   * Implements `JITCompilerBackend::eval_func()`.
   */
  virtual bool eval_func(const std::vector<RuntimeValue>& args,
    RuntimeValue& result);

  /**
   * Implements `JITCompilerBackend::finalize()`.
   */
  virtual bool finalize();

private:
  const ModuleType& m_module;
};

} /* end namespace jit */
} /* end namespace corevm */

#endif /* COREVM_JIT_COMPILER_LLVM_MCJIT_BACKEND_H_ */
