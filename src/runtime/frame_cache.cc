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
#include "frame_cache.h"
#include "frame.h"

#include <boost/functional/hash.hpp>


namespace corevm {
namespace runtime {

// -----------------------------------------------------------------------------

size_t
ClosureCtxHash::operator()(const ClosureCtx& closure_ctx) const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, closure_ctx.compartment_id);
  boost::hash_combine(seed, closure_ctx.closure_id);
  return seed;
}

// -----------------------------------------------------------------------------

void
FrameCache::insert_parent_frame(const ClosureCtx& ctx, FramePtr parent_frame)
{
  m_cache.insert(std::make_pair(ctx, parent_frame));
  m_reverse_cache.insert(std::make_pair(parent_frame, ctx));
}

// -----------------------------------------------------------------------------

void
FrameCache::erase_parent_frame(FramePtr parent_frame)
{
  auto itr = m_reverse_cache.find(parent_frame);
  if (itr != m_reverse_cache.end())
  {
    m_reverse_cache.erase(itr);
    m_cache.erase(itr->second);
  }
}

// -----------------------------------------------------------------------------

FramePtr
FrameCache::parent_frame_of(const ClosureCtx& ctx)
{
  FramePtr parent = NULL;

  auto itr = m_cache.find(ctx);
  if (itr != m_cache.end())
  {
    parent = itr->second;
  }

  return parent;
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
