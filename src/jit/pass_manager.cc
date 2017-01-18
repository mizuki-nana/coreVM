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
#include "pass_manager.h"
#include "verification_pass.h"
#include "transform_pass.h"


namespace corevm {
namespace jit {

// -----------------------------------------------------------------------------

PassManager::PassManager()
  :
  m_run_result({"", true, nullptr})
{
}

// -----------------------------------------------------------------------------

bool
PassManager::has_failed() const
{
  return !m_run_result.success;
}

// -----------------------------------------------------------------------------

bool PassManager::success() const
{
  return m_run_result.success;
}

// -----------------------------------------------------------------------------

std::shared_ptr<const AnalysisResult>
PassManager::get_analysis_result() const
{
  return m_run_result.analysis_result;
}

// -----------------------------------------------------------------------------

const std::string&
PassManager::msg() const
{
  return m_run_result.msg;
}

// -----------------------------------------------------------------------------

} /* end namespace jit */
} /* end namespace corevm */
