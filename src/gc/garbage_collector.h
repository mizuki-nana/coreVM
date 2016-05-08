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
#ifndef COREVM_GARBAGE_COLLECTOR_H_
#define COREVM_GARBAGE_COLLECTOR_H_

#include "corevm/llvm_smallvector.h"
#include "corevm/logging.h"
#include "dyobj/dynamic_object_heap.h"


namespace corevm {


namespace gc {


// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
class GarbageCollector : public Loggable
{
public:
  using dynamic_object_heap_type = typename dyobj::DynamicObjectHeap<
    typename garbage_collection_scheme::DynamicObjectManager>;

  using dynamic_object_type = typename dynamic_object_heap_type::dynamic_object_type;

  class Callback
  {
    public:
      using dynamic_object_type = typename GarbageCollector::dynamic_object_type;

      virtual void operator()(const dynamic_object_type& obj) = 0;

    virtual ~Callback() {}
  };

  explicit GarbageCollector(dynamic_object_heap_type&);

  void gc() noexcept;

  void gc(Callback*) noexcept;

protected:
  void free(Callback* f=nullptr) noexcept;

  garbage_collection_scheme m_gc_scheme;
  dynamic_object_heap_type& m_heap;
};

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
GarbageCollector<garbage_collection_scheme>::GarbageCollector(
  GarbageCollector<garbage_collection_scheme>::dynamic_object_heap_type& heap)
  :
  Loggable(),
  m_gc_scheme(),
  m_heap(heap)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
void
GarbageCollector<garbage_collection_scheme>::gc() noexcept
{
  m_gc_scheme.gc(m_heap);
  this->free();
}

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
void
GarbageCollector<garbage_collection_scheme>::gc(Callback* f) noexcept
{
  m_gc_scheme.set_logger(m_logger);
  m_gc_scheme.gc(m_heap);
  this->free(f);
}

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
void
GarbageCollector<garbage_collection_scheme>::free(Callback* f) noexcept
{
  // NOTE: Cannot use `std::remove_if` here because certain implementations
  // require copying the underlying container elements, in this case
  // `dynamic_object_type`, which does not support explicit copy semantics.

  llvm::SmallVector<dynamic_object_type*, 50> objs_to_delete;
  objs_to_delete.reserve(m_heap.size());

  for (auto itr = m_heap.begin(); itr != m_heap.end(); ++itr)
  {
    dynamic_object_type& obj = static_cast<dynamic_object_type&>(*itr);

    if (obj.is_garbage_collectible())
    {
      objs_to_delete.push_back(&obj);

      if (f)
      {
        (*f)(obj);
      }
    }
  }

  for (const auto obj : objs_to_delete)
  {
    m_heap.erase(obj);
  }
}

// -----------------------------------------------------------------------------


} /* end namespace gc */


} /* end namespace corevm */


#endif /* COREVM_GARBAGE_COLLECTOR_H_ */
