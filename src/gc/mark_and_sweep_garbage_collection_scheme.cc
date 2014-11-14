#include <algorithm>
#include "../../include/gc/garbage_collection_scheme.h"


void
corevm::gc::mark_and_sweep_garbage_collection_scheme::gc(
  corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_heap_type& heap) const
{
  using _dynamic_object_heap_type = typename
    corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_heap_type;

  heap.iterate(
    [this, &heap](
      _dynamic_object_heap_type::dynamic_object_id_type id,
      _dynamic_object_heap_type::dynamic_object_type object
    ) {
      if(this->is_root_object(object)) {
        this->mark(heap, object);
      }
    }
  );
}

bool
corevm::gc::mark_and_sweep_garbage_collection_scheme::is_root_object(
  const corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_type& object) const noexcept
{
  // TODO: [COREVM-62] Complete mark and sweep GC scheme implementation
  return true;
}

void
corevm::gc::mark_and_sweep_garbage_collection_scheme::mark(
  corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_heap_type& heap,
  corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_type& object) const
{
  using _dynamic_object_type = typename
    corevm::gc::mark_and_sweep_garbage_collection_scheme::dynamic_object_type;

  if(!object.is_garbage_collectible()) {
    return;
  }

  object.manager().mark();

  object.iterate(
    [this, &heap](
      _dynamic_object_type::attr_key_type attr_key,
      _dynamic_object_type::dyobj_id_type dyobj_id
    ) {
      _dynamic_object_type& referenced_object = heap.at(dyobj_id);
      this->mark(heap, referenced_object);
    }
  );
}
