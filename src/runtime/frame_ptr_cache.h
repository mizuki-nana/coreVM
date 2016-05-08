/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#ifndef COREVM_FRAME_PTR_CACHE_H_
#define COREVM_FRAME_PTR_CACHE_H_

#include "closure_ctx.h"
#include "frame.h"

#include <unordered_map>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

struct ClosureCtxHash
{
  size_t operator()(const ClosureCtx& closure_ctx) const;
};

// -----------------------------------------------------------------------------

typedef Frame* FramePtr;

// -----------------------------------------------------------------------------

class FramePtrCache
{
public:
  void insert_parent_frame(const ClosureCtx& ctx, FramePtr parent_frame);

  void erase_parent_frame(FramePtr parent_frame);

  FramePtr parent_frame_of(const ClosureCtx& ctx);

private:
  std::unordered_map<ClosureCtx, FramePtr, ClosureCtxHash> m_cache;
  std::unordered_map<FramePtr, ClosureCtx> m_reverse_cache;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_FRAME_PTR_CACHE_H_ */
