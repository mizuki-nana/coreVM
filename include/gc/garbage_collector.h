#ifndef COREVM_GARBAGE_COLLECTOR_H_
#define COREVM_GARBAGE_COLLECTOR_H_

#include <algorithm>
#include "../dyobj/dynamic_object_heap.h"


namespace corevm {


namespace gc {


template<class garbage_collection_scheme>
class garbage_collector {
public:
  using dynamic_object_heap = typename corevm::dyobj::dynamic_object_heap<
    typename garbage_collection_scheme::dynamic_object_manager>;

  explicit garbage_collector(dynamic_object_heap&);

  void gc() const noexcept;

protected:
  void free() noexcept;

  garbage_collection_scheme _gc_scheme;
  dynamic_object_heap& _heap;
};


template<class garbage_collection_scheme>
corevm::gc::garbage_collector<garbage_collection_scheme>::garbage_collector(
  corevm::gc::garbage_collector<garbage_collection_scheme>::dynamic_object_heap& heap
):
  _heap(heap),
  _gc_scheme(garbage_collection_scheme())
{
  // Do nothing here.
}

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::gc() const noexcept
{
  _gc_scheme.gc(_heap);
  this->free();
}

template<class garbage_collection_scheme>
void
corevm::gc::garbage_collector<garbage_collection_scheme>::free() noexcept
{
  using _dynamic_object_heap_type = typename
    corevm::gc::garbage_collector<garbage_collection_scheme>::dynamic_object_heap;

  using _dynamic_object_type = typename _dynamic_object_heap_type::dynamic_object_type;

  std::remove_if(
    _heap.begin(),
    _heap.end(),
    [](typename _dynamic_object_heap_type::pair pair) -> bool
    {
      _dynamic_object_type& object = static_cast<_dynamic_object_type&>(pair.second);
      return object.manager().garbage_collectible();
    }
  );
}


} /* end namespace gc */


} /* end namespace corevm */


#endif /* COREVM_GARBAGE_COLLECTOR_H_ */