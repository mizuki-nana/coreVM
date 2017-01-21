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
#ifndef COREVM_LOWERING_PASS_TARGET_LLVM_IR_H_
#define COREVM_LOWERING_PASS_TARGET_LLVM_IR_H_

#include "analysis_pass.h"
#include <llvm/IR/Module.h>
#include <memory>

namespace corevm {
namespace jit {

struct LLVMModuleAnalysisResult : public AnalysisResult
{
  mutable std::unique_ptr<llvm::Module> target_module;
};

class LoweringPassTargetLLVMIR : public AnalysisPass
{
public:
  static const char* Name;

  virtual ~LoweringPassTargetLLVMIR();

  /**
   * Implements `AnalysisPass::get_analysis_result()`.
   */
  virtual std::shared_ptr<const AnalysisResult> get_analysis_result() const;

  /**
   * Implements `AnalysisPass::run(const IRModule&, const AnalysisResult* = nullptr)`.
   */
  virtual bool run(const IRModule&, const AnalysisResult* = nullptr);

  /**
   * Implements `AnalysisPass::run(const IRTypeDecl&, const AnalysisResult* = nullptr)`.
   */
  virtual bool run(const IRTypeDecl&, const AnalysisResult* = nullptr);

  /**
   * Implements `AnalysisPass::run(const IRClosure&, const AnalysisResult* = nullptr)`.
   */
  virtual bool run(const IRClosure&, const AnalysisResult* = nullptr);

  /**
   * Implements `AnalysisPass::run(const IRBasicBlock&, const AnalysisResult* = nullptr)`.
   */
  virtual bool run(const IRBasicBlock&, const AnalysisResult* = nullptr);

  /**
   * Implements `AnalysisPass::run(const IRInstruction&, const AnalysisResult* = nullptr)`.
   */
  virtual bool run(const IRInstruction&, const AnalysisResult* = nullptr);
};

} /* end namespace jit */
} /* end namespace corevm */

#endif /* COREVM_LOWERING_PASS_TARGET_LLVM_IR_H_ */
