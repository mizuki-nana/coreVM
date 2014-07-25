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

  // Seems like we cannot use `std::remove_if` on associative map.
  // http://stackoverflow.com/questions/9210014/remove-elements-from-an-unordered-map-fulfilling-a-predicate

  /*
  std::remove_if(
    _heap.begin(),
    _heap.end(),
    [](typename _dynamic_object_heap_type::pair& pair) -> bool
    {
      _dynamic_object_type& object = static_cast<_dynamic_object_type&>(pair.second);
      return object.manager().garbage_collectible();
    }
  );
  */

  auto remove_criterion = [](typename _dynamic_object_heap_type::iterator itr) -> bool {
    _dynamic_object_type& object = static_cast<_dynamic_object_type&>(itr->second);
    return object.manager().garbage_collectible();
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
