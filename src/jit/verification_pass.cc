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
#include "verification_pass.h"
#include "ir/verifier.h"
#include "ir/ir_module_index.h"
#include "ir/format.h"
#include <string>


namespace corevm {
namespace jit {

// -----------------------------------------------------------------------------

IRModuleIndexAnalysisResult::~IRModuleIndexAnalysisResult()
{
}

// -----------------------------------------------------------------------------

const char* VerificationPass::Name = "IR Verification";

// -----------------------------------------------------------------------------

const Pass::PassType VerificationPass::Type = PassType_Module;

// -----------------------------------------------------------------------------

VerificationPass::VerificationPass()
  :
  m_analysis_result(new IRModuleIndexAnalysisResult())
{
}

// -----------------------------------------------------------------------------

/* virtual */
std::shared_ptr<const AnalysisResult>
VerificationPass::get_analysis_result() const
{
  return m_analysis_result;
}

// -----------------------------------------------------------------------------

/* virtual */
bool VerificationPass::run(const IRModule& module, const AnalysisResult*)
{
  ir::Verifier verifier(module);
  std::string msg;
  if (!verifier.run(msg))
  {
    return false;
  }

  verifier.get_index(m_analysis_result->module_index);

  return true;
}

// -----------------------------------------------------------------------------

} /* end namespace jit */
} /* end namespace corevm */
