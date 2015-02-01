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
#include "../../include/gc/garbage_collection_scheme.h"

#include <sneaker/algorithm/tarjan.h>

#include <algorithm>
#include <set>
#include <unordered_map>


void
corevm::gc::reference_count_garbage_collection_scheme::gc(
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type& heap) const
{
  using _dynamic_object_heap_type = typename
    corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type;

  _dynamic_object_heap_type::size_type prev_active_size = heap.active_size();

  while (true) {
    heap.iterate(
      [this, &heap](
        _dynamic_object_heap_type::dynamic_object_id_type id,
        _dynamic_object_heap_type::dynamic_object_type& object
      ) {
        this->check_and_dec_ref_count(heap, object);
        this->resolve_self_reference_cycles(heap, object);
      }
    );

    this->remove_cycles(heap);

    if (heap.active_size() == prev_active_size) {
      break;
    }

    prev_active_size = heap.active_size();
  } /* end of `while (true)` */
}

void
corevm::gc::reference_count_garbage_collection_scheme::check_and_dec_ref_count(
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type& heap,
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_type& object) const
{
  using _dynamic_object_type = typename
    corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_type;

  if (!object.is_garbage_collectible()) {
    return;
  }

  object.iterate(
    [this, &heap](
      _dynamic_object_type::attr_key_type attr_key,
      _dynamic_object_type::dyobj_id_type dyobj_id
    ) {
      _dynamic_object_type& referenced_object = heap.at(dyobj_id);

      referenced_object.manager().dec_ref_count();
    }
  );
}

void
corevm::gc::reference_count_garbage_collection_scheme::resolve_self_reference_cycles(
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type& heap,
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_type& object) const
{
  using _dynamic_object_type = typename
    corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_type;

  object.iterate(
    [this, &heap, &object](
      _dynamic_object_type::attr_key_type attr_key,
      _dynamic_object_type::dyobj_id_type dyobj_id
    ) {
      _dynamic_object_type& referenced_object = heap.at(dyobj_id);

      auto ref_count1 = object.manager().ref_count();
      auto ref_count2 = referenced_object.manager().ref_count();

      if (referenced_object.has_ref(object.id()) && ref_count1 == 1 && ref_count2 == 1) {
        object.manager().dec_ref_count();
        referenced_object.manager().dec_ref_count();
      }
    }
  );
}

void
corevm::gc::reference_count_garbage_collection_scheme::remove_cycles(
  corevm::gc::reference_count_garbage_collection_scheme::dynamic_object_heap_type& heap) const
{
  using sneaker::algorithm::tarjan;

  using dyobj_id_type = typename dynamic_object_type::dyobj_id_type;
  using vertex_type = typename tarjan<dyobj_id_type>::vertex;

  std::unordered_map<dyobj_id_type, vertex_type> vertices_map;

  auto get_vertex = [&](dyobj_id_type id) -> vertex_type& {
    auto itr = vertices_map.find(id);

    if (itr == vertices_map.end()) {
      vertices_map[id] = vertex_type(id);
    }

    return vertices_map.at(id);
  };

  std::set<dyobj_id_type> non_garbage_collectible_neighbors;

  heap.iterate(
    [&](
      dynamic_object_heap_type::dynamic_object_id_type id,
      dynamic_object_heap_type::dynamic_object_type& object
    ) {
      if (object.get_flag(corevm::dyobj::flags::IS_NOT_GARBAGE_COLLECTIBLE) == true) {
        object.iterate(
          [&](
            dynamic_object_type::attr_key_type attr_key,
            dynamic_object_type::dyobj_id_type neighbor_id
          ) {
            non_garbage_collectible_neighbors.insert(neighbor_id);
          }
        );

        return;
      }

      vertex_type& vertex = get_vertex(id);

      object.iterate(
        [&](
          dynamic_object_type::attr_key_type attr_key,
          dynamic_object_type::dyobj_id_type neighbor_id
        ) {
          vertex_type& neighbor_vertex = get_vertex(neighbor_id);

          vertex.dependencies().push_back(&neighbor_vertex);
        }
      );
    }
  );

  std::list<vertex_type*> vertices;

  for (auto itr = vertices_map.begin(); itr != vertices_map.end(); ++itr) {
    vertex_type* vertex_ptr = static_cast<vertex_type*>(&itr->second);
    vertices.push_back(vertex_ptr);
  }

  tarjan<dyobj_id_type> algo;
  auto components = algo.get_components(vertices);

  std::list<std::list<dyobj_id_type>> cycles = components.cycles();

  for (auto itr = cycles.begin(); itr != cycles.end(); ++itr) {
    std::list<dyobj_id_type> cycle = static_cast<std::list<dyobj_id_type>>(*itr);

    std::set<dyobj_id_type> cycle_set;

    for (auto itr_ = cycle.begin(); itr_ != cycle.end(); ++itr_) {
      dyobj_id_type id = static_cast<dyobj_id_type>(*itr_);
      cycle_set.insert(id);
    }

    std::set<dyobj_id_type> intersect;
    std::set_intersection(
      non_garbage_collectible_neighbors.begin(),
      non_garbage_collectible_neighbors.end(),
      cycle_set.begin(),
      cycle_set.end(),
      std::inserter(intersect, intersect.begin())
    );

    if (!intersect.empty()) {
      continue;
    }

    for (auto itr_ = cycle.begin(); itr_ != cycle.end(); ++itr_) {
      dyobj_id_type id = static_cast<dyobj_id_type>(*itr_);
      auto& obj = heap.at(id);
      obj.manager().dec_ref_count();
    }
  }
}
