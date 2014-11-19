/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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

#include <algorithm>
#include "../dyobj/dynamic_object_heap.h"


namespace corevm {


namespace gc {


template<class garbage_collection_scheme>
class garbage_collector {
public:
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<
    typename garbage_collection_scheme::dynamic_object_manager>;

  explicit garbage_collector(dynamic_object_heap_type&);

  void gc() noexcept;

protected:
  void free() noexcept;

  garbage_collection_scheme _gc_scheme;
  dynamic_object_heap_type& _heap;
};


template<class garbage_collection_scheme>
corevm::gc::garbage_collector<garbage_collection_scheme>::garbage_collector(
  corevm::gc::garbage_collector<garbage_collection_scheme>::dynamic_object_heap_type& heap
):
  _gc_scheme(garbage_collection_scheme()),
  _heap(heap)
{
  // Do nothing here.
}

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::gc() noexcept
{
  _gc_scheme.gc(_heap);
  this->free();
}

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::free() noexcept
{
  using _dynamic_object_heap_type = typename
    corevm::gc::garbage_collector<garbage_collection_scheme>::dynamic_object_heap_type;

  using _dynamic_object_type = typename _dynamic_object_heap_type::dynamic_object_type;

  auto remove_criterion = [](typename _dynamic_object_heap_type::iterator itr) -> bool {
    _dynamic_object_type& object = static_cast<_dynamic_object_type&>(itr->second);
    return object.is_garbage_collectible();
  };

  for(auto itr = _heap.begin(); itr != _heap.end();) {
    if(remove_criterion(itr)) {
      itr = _heap.erase(itr);
    } else {
      ++itr;
    }
  }
}


} /* end namespace gc */


} /* end namespace corevm */


#endif /* COREVM_GARBAGE_COLLECTOR_H_ */
