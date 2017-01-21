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
#ifndef COREVM_PASS_MANAGER_H_
#define COREVM_PASS_MANAGER_H_

#include "analysis_pass.h"
#include "ir/fwd.h"
#include <memory>
#include <string>
#include <type_traits>


namespace corevm {
namespace jit {

/**
 * A "static" pass manager responsible for managing the operations of a set
 * of "pass"es in serial order.
 */
class PassManager
{
public:
  PassManager();

  PassManager(const PassManager&) = delete;

  /**
   * Run sequences of passes in series.
   *
   * If a previous pass failed, then no further passes shall be run.
   */
  template<typename T>
  PassManager& run_pass(IRModule& module)
  {
    if (has_failed())
    {
      return *this;
    }

    T pass;
    pass.init(module);
    const bool res = pass.run(module, m_run_result.analysis_result.get());
    if (!res)
    {
      m_run_result.success = false;
      m_run_result.msg.assign(T::Name);
      return *this;
    }
    handle_pass(pass);
    pass.finalize(module);
    return *this;
  }

  /**
   * Returns whether the sequence of passes have all completely successfully.
   * Returns false if any of the passes fail.
   */
  bool success() const;

  /**
   * Gets the last successfully generated analysis result.
   */
  std::shared_ptr<const AnalysisResult> get_analysis_result() const;

  /**
   * Gets the error message if any of the passes failed.
   */
  const std::string& msg() const;

private:
  template<class T>
  void handle_pass(
    typename std::enable_if<std::is_base_of<AnalysisPass, T>::value, T>::type& pass)
  {
    if (auto analysis_result = pass.get_analysis_result())
    {
      m_run_result.analysis_result = analysis_result;
    }
  }

  template<class T>
  void handle_pass(T&)
  {
  }

  bool has_failed() const;

  struct RunResult
  {
    std::string msg;
    bool success;
    std::shared_ptr<const AnalysisResult> analysis_result; 
  };

  RunResult m_run_result;
};

} /* end namespace jit */
} /* end namespace corevm */

#endif /* COREVM_PASS_MANAGER_H_ */
