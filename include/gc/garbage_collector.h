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
#ifndef COREVM_GARBAGE_COLLECTOR_H_
#define COREVM_GARBAGE_COLLECTOR_H_

#include "../dyobj/dynamic_object_heap.h"

#include <algorithm>


namespace corevm {


namespace gc {


template<class garbage_collection_scheme>
class garbage_collector
{
public:
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<
    typename garbage_collection_scheme::dynamic_object_manager>;

  using dynamic_object_type = typename dynamic_object_heap_type::dynamic_object_type;

  class callback
  {
    public:
      virtual void operator()(const dynamic_object_type& obj) = 0;
  };

  explicit garbage_collector(dynamic_object_heap_type&);

  void gc() noexcept;

  void gc(callback*) noexcept;

protected:
  void free(callback* f=nullptr) noexcept;

  garbage_collection_scheme m_gc_scheme;
  dynamic_object_heap_type& m_heap;
};

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
corevm::gc::garbage_collector<garbage_collection_scheme>::garbage_collector(
  corevm::gc::garbage_collector<garbage_collection_scheme>::dynamic_object_heap_type& heap)
  :
  m_gc_scheme(),
  m_heap(heap)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::gc() noexcept
{
  m_gc_scheme.gc(m_heap);
  this->free();
}

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::gc(callback* f) noexcept
{
  m_gc_scheme.gc(m_heap);
  this->free(f);
}

// -----------------------------------------------------------------------------

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::free(callback* f) noexcept
{
  auto remove_criterion = [](typename dynamic_object_heap_type::iterator itr) -> bool {
    dynamic_object_type& object = static_cast<dynamic_object_type&>(*itr);
    return object.is_garbage_collectible();
  };

  for (auto itr = m_heap.begin(); itr != m_heap.end();)
  {
    if (remove_criterion(itr))
    {
      dynamic_object_type& obj = static_cast<dynamic_object_type&>(*itr);

      if (f)
      {
        (*f)(obj);
      }

      auto itr_next = itr;
      ++itr_next;
      m_heap.erase(itr);
      itr = itr_next;
    }
    else
    {
      ++itr;
    }
  }
}

// -----------------------------------------------------------------------------


} /* end namespace gc */


} /* end namespace corevm */


#endif /* COREVM_GARBAGE_COLLECTOR_H_ */
