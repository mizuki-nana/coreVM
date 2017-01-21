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
#include "constant_folding_pass.h"


namespace corevm {
namespace jit {

// -----------------------------------------------------------------------------

const char* ConstantFoldingPass::Name = "Constant Folding";

// -----------------------------------------------------------------------------

const Pass::PassType ConstantFoldingPass::Type = PassType_Module;

// -----------------------------------------------------------------------------

/* virtual */
bool ConstantFoldingPass::run(IRModule&, const AnalysisResult*)
{
  // TODO: [COREVM-582] Implement IR strength reduction passes
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool ConstantFoldingPass::run(IRTypeDecl&, const AnalysisResult*)
{
  // TODO: [COREVM-582] Implement IR strength reduction passes
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool ConstantFoldingPass::run(IRClosure&, const AnalysisResult*)
{
  // TODO: [COREVM-582] Implement IR strength reduction passes
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool ConstantFoldingPass::run(IRBasicBlock&, const AnalysisResult*)
{
  // TODO: [COREVM-582] Implement IR strength reduction passes
  return true;
}

// -----------------------------------------------------------------------------

/* virtual */
bool ConstantFoldingPass::run(IRInstruction&, const AnalysisResult*)
{
  // TODO: [COREVM-582] Implement IR strength reduction passes
  return true;
}

// -----------------------------------------------------------------------------

} /* end namespace jit */
} /* end namespace corevm */
