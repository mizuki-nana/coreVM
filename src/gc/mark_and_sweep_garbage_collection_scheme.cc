/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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
#include "mark_and_sweep_garbage_collection_scheme.h"

#include <algorithm>


void
corevm::gc::mark_and_sweep_garbage_collection_scheme::gc(
  corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_heap_type& heap) const
{
  using _dynamic_object_heap_type = typename
    corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_heap_type;

  heap.iterate(
    [this, &heap](
      _dynamic_object_heap_type::dynamic_object_id_type id,
      _dynamic_object_heap_type::dynamic_object_type& object)
    {
      if (this->is_root_object(object))
      {
        this->mark(heap, object);
      }
    }
  );
}

// -----------------------------------------------------------------------------

bool
corevm::gc::mark_and_sweep_garbage_collection_scheme::is_root_object(
  const corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_type& object) const noexcept
{
  // TODO: [COREVM-62] Complete mark and sweep GC scheme implementation
  return true;
}

// -----------------------------------------------------------------------------

void
corevm::gc::mark_and_sweep_garbage_collection_scheme::mark(
  corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_heap_type& heap,
  corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_type& object) const
{
  using _dynamic_object_type = typename
    corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_type;

  if (!object.is_garbage_collectible())
  {
    return;
  }

  object.manager().mark();

  object.iterate(
    [this, &heap](
      _dynamic_object_type::attr_key_type attr_key,
      _dynamic_object_type::dyobj_id_type dyobj_id)
    {
      _dynamic_object_type& referenced_object = heap.at(dyobj_id);
      this->mark(heap, referenced_object);
    }
  );
}

// -----------------------------------------------------------------------------
