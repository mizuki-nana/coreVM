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
#include "jit_compiler_llvmmcjit_backend.h"

namespace corevm {
namespace jit {

// -----------------------------------------------------------------------------

JITCompilerLLVMMCJITBackend::JITCompilerLLVMMCJITBackend(
  const ModuleType& module)
  :
  JITCompilerBackend(),
  m_module(module)
{
}

// -----------------------------------------------------------------------------

JITCompilerLLVMMCJITBackend::~JITCompilerLLVMMCJITBackend()
{
}

// -----------------------------------------------------------------------------

/* virtual */
bool JITCompilerLLVMMCJITBackend::init()
{
  // TODO: [COREVM-583] Implement JIT compiler backend based on LLVM's llvm::MCJIT framework
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool JITCompilerLLVMMCJITBackend::run(const std::string& /* func_name */)
{
  // TODO: [COREVM-583] Implement JIT compiler backend based on LLVM's llvm::MCJIT framework
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool JITCompilerLLVMMCJITBackend::eval_func(
  const std::vector<RuntimeValue>& /* args */, RuntimeValue& /* result */)
{
  // TODO: [COREVM-583] Implement JIT compiler backend based on LLVM's llvm::MCJIT framework
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool JITCompilerLLVMMCJITBackend::finalize()
{
  // TODO: [COREVM-583] Implement JIT compiler backend based on LLVM's llvm::MCJIT framework
  return true;
}

// -----------------------------------------------------------------------------

} /* end namespace jit */
} /* end namespace corevm */
