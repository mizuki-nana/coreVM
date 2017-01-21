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
#ifndef COREVM_JIT_COMPILER_H_
#define COREVM_JIT_COMPILER_H_

#include "pass_manager.h"
#include "verification_pass.h"
#include "constant_folding_pass.h"
#include "cse_pass.h"
#include "runtime_value.h"
#include <string>
#include <memory>
#include <vector>


namespace corevm {
namespace jit {

/**
 * Per-function based Just-in-Time compiler.
 *
 * The JIT compiler works by knowing the type of the source and target module,
 * as well as the specific type of compilation backend used.
 *
 * Prior to compilation, it invokes a set of verification, optimization and
 * lowering passes on the input module. All passes need to succeed before
 * compilation can begin.
 */
template <typename JITScheme>
class JITCompiler
{
public:
  JITCompiler()
    :
    m_target_backend(),
    m_target_module()
  {
  }

  ~JITCompiler()
  {
    release();
  }

  /**
   * Public interface to invoke JIT compilation.
   * Invokes the compilation by supply the input module, as well as the name
   * of the function to be compiled.
   *
   * Returns a value indicating whether the compilation is successful.
   */
  bool run(typename JITScheme::InputModuleType& input_module,
    const std::string& func_name, std::string& msg)
  {
    PassManager pass_manager;
    pass_manager
      .run_pass<VerificationPass>(input_module)
      .template run_pass<ConstantFoldingPass>(input_module)
      .template run_pass<CSEPass>(input_module)
      .template run_pass<typename JITScheme::TargetLoweringPassType>(input_module)
      .template run_pass<VerificationPass>(input_module);

    if (!pass_manager.success())
    {
      msg.assign("Pass failed: " + pass_manager.msg());
      return false;
    }

    std::shared_ptr<const AnalysisResult> analysis_result = pass_manager.get_analysis_result();

    if (!analysis_result)
    {
      msg.assign("Target module lowering failed.");
      return false;
    }

    auto casted_analysis_result = std::dynamic_pointer_cast<
      const typename JITScheme::TargetLoweringPassAnalysisResultType>(analysis_result);

    if (!casted_analysis_result)
    {
      msg.assign("Cannot get result of target module lowering.");
      return false;
    }

    // JIT compiler takes over ownership of target module.
    m_target_module = std::move(casted_analysis_result->target_module);

    if (!m_target_module)
    {
      msg.assign("Target module absent for JIT");
      return false;
    }

    std::string backend_msg;
    if (!compile(func_name, backend_msg))
    {
      msg.assign("Target JIT compilation failed: " + backend_msg);
      release();
      return false;
    }

    return true;
  }

  /**
   * Releases the compiled artifact, and consequently sets the compiler to a
   * "void" state such that subsequent calls to `eval_func` are not allowed
   * until another succesful compilation.
   */
  void release()
  {
    m_target_backend.reset();
    m_target_module.reset();
  }

  /**
   * Returns a boolean indicating whether the compilation has been successful.
   */
  bool compiled() const
  {
    return m_target_backend && m_target_module;
  }

  /**
   * Evaluates the compiled function by specifying a set of arguments, as well
   * as pointer to the value that is to be returned from the function call,
   * if one exists.
   *
   * Returns a boolean value indicating whether the evaluation is successful.
   */
  bool eval_func(const std::vector<RuntimeValue>& args, RuntimeValue& result)
  {
    if (!compiled())
    {
      return false;
    }
    return m_target_backend->eval_func(args, result);
  }

private:
  /**
   * Performs the actual JIT compilation by invoking the compilation backend.
   *
   * Returns a boolean value indicating whether the compilation is successful.
   */
  bool compile(const std::string& func_name, std::string& msg)
  {
    m_target_backend.reset(new typename JITScheme::BackendType(*m_target_module));
    if (!m_target_backend->init())
    {
      msg.assign("Target backend initialization failed");
      return false;
    }
    if (!m_target_backend->run(func_name))
    {
      msg.assign("Target backend compilation failed");
      return false;
    }

    m_target_backend->finalize();

    return true;
  }

private:
  std::unique_ptr<typename JITScheme::BackendType> m_target_backend;
  std::unique_ptr<typename JITScheme::TargetModuleType> m_target_module;
};

} /* end namespace jit */
} /* end namespace corevm */

#endif /* COREVM_JIT_COMPILER_H_ */
