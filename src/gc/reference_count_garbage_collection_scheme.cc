#include <algorithm>
#include "../../include/gc/garbage_collection_scheme.h"


void
corevm::gc::reference_count_garbage_collection_scheme::gc(
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type& heap) const
{
  using _dynamic_object_heap_type = typename
    corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type;

  heap.iterate(
    [this, &heap](
      _dynamic_object_heap_type::dynamic_object_id_type id,
      _dynamic_object_heap_type::dynamic_object_type object
    ) {
      this->check_and_dec_ref_count(heap, object);
    }
  );
}

void
corevm::gc::reference_count_garbage_collection_scheme::check_and_dec_ref_count(
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type& heap,
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_type& object) const
{
  using _dynamic_object_type = typename
    corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_type;

  if(!object.is_garbage_collectible()) {
    return;
  }

  object.iterate(
    [this, &heap](
      _dynamic_object_type::attr_key_type attr_key,
      _dynamic_object_type::dyobj_id_type dyobj_id
    ) {
      _dynamic_object_type& referenced_object = heap.at(dyobj_id);

      referenced_object.manager().dec_ref_count();
      this->check_and_dec_ref_count(heap, referenced_object);
    }
  );
}
