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
#ifndef COREVM_TRANSFORM_PASS_H_
#define COREVM_TRANSFORM_PASS_H_

#include "pass.h"
#include "analysis_result.h"
#include "ir/fwd.h"


namespace corevm {
namespace jit {

class TransformPass : public Pass
{
public:
  virtual ~TransformPass() {}

  /**
   * Performs transform operation on the specified IR module.
   *
   * Returns a boolean indicating whether the operation is successful.
   */
  virtual bool run(IRModule&, const AnalysisResult* = nullptr);

  /**
   * Performs transform operation on the specified IR type definition.
   *
   * Returns a boolean indicating whether the operation is successful.
   */
  virtual bool run(IRTypeDecl&, const AnalysisResult* = nullptr);

  /**
   * Performs transform operation on the specified IR function.
   *
   * Returns a boolean indicating whether the operation is successful.
   */
  virtual bool run(IRClosure&, const AnalysisResult* = nullptr);

  /**
   * Performs transform operation on the specified IR basic block.
   *
   * Returns a boolean indicating whether the operation is successful.
   */
  virtual bool run(IRBasicBlock&, const AnalysisResult* = nullptr);

  /**
   * Performs transform operation on the specified IR instruction.
   *
   * Returns a boolean indicating whether the operation is successful.
   */
  virtual bool run(IRInstruction&, const AnalysisResult* = nullptr);
};

} /* end namespace jit */
} /* end namespace corevm */

#endif /* COREVM_TRANSFORM_PASS_H_ */
