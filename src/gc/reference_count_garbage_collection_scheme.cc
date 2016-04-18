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
#include "reference_count_garbage_collection_scheme.h"

#include <sneaker/algorithm/tarjan.h>

#include <algorithm>
#include <set>
#include <unordered_map>


namespace corevm {


namespace gc {


// -----------------------------------------------------------------------------

RefCountGarbageCollectionScheme::DynamicObjectManager::DynamicObjectManager()
  :
  dyobj::DynamicObjectManager(),
  m_count(0u),
  m_attached(false)
{
}

// -----------------------------------------------------------------------------

/* virtual */
void
RefCountGarbageCollectionScheme::DynamicObjectManager::on_delete() noexcept
{
  dec_ref_count();
}

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::gc(
  RefCountGarbageCollectionScheme::dynamic_object_heap_type& heap) const
{
  RefCountGarbageCollectionScheme::HeapIterator heap_iterator(*this);

  auto prev_active_size = heap.active_size();

  while (true)
  {
    heap.iterate(heap_iterator);

    remove_cycles(heap);

    if (heap.active_size() == prev_active_size)
    {
      break;
    }

    prev_active_size = heap.active_size();
  } /* end of `while (true)` */
}

// -----------------------------------------------------------------------------

struct ObjectRefCountDecrementor
{
  using dynamic_object_type = typename RefCountGarbageCollectionScheme::dynamic_object_type;

  void operator()(const typename dynamic_object_type::attr_key_type&,
    dynamic_object_type* referenced_object)
  {
    if (!referenced_object->manager().locked())
    {
      referenced_object->manager().dec_ref_count();
    }
  }
};

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::check_and_dec_ref_count(
  RefCountGarbageCollectionScheme::dynamic_object_type* object) const
{
  if (!object->is_garbage_collectible())
  {
    return;
  }

  ObjectRefCountDecrementor decrementor;
  object->iterate(decrementor);
}

// -----------------------------------------------------------------------------

struct CycledObjectReferenceDecrementor
{
private:
  using dynamic_object_type = typename RefCountGarbageCollectionScheme::dynamic_object_type;

public:
  CycledObjectReferenceDecrementor(dynamic_object_type* object)
    :
    m_object(object)
  {
  }

  void operator()(const typename dynamic_object_type::attr_key_type&,
    dynamic_object_type* referenced_object)
  {
    if (referenced_object->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE) ||
        referenced_object->manager().locked() ||
        !referenced_object->has_ref(m_object))
    {
      return;
    }

    const auto ref_count1 = m_object->manager().ref_count();
    const auto ref_count2 = referenced_object->manager().ref_count();

    if (ref_count1 == 1 && ref_count2 == 1)
    {
      m_object->manager().dec_ref_count();
      referenced_object->manager().dec_ref_count();
    }
  }

private:
  dynamic_object_type* m_object;

}; /* end of `CycledObjectReferenceDecrementor` */

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::resolve_self_reference_cycles(
  RefCountGarbageCollectionScheme::dynamic_object_type* object) const
{
  if (!object->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE))
  {
    CycledObjectReferenceDecrementor decrementor(object);
    object->iterate(decrementor);
  }
}

// -----------------------------------------------------------------------------

struct ObjectGraphBuilder
{
public:
  using dynamic_object_type = typename RefCountGarbageCollectionScheme::dynamic_object_heap_type::dynamic_object_type;
  using vertex_type = typename sneaker::algorithm::tarjan<dynamic_object_type*>::vertex;
  using vertices_map_type = typename std::unordered_map<dynamic_object_type*, vertex_type>;
  using neighbor_set_type = typename std::set<dynamic_object_type*>;

public:
  ObjectGraphBuilder(vertices_map_type& vertices_map,
    neighbor_set_type& non_garbage_collectible_neighbors)
    :
    m_non_garbage_collectible_neighbors(non_garbage_collectible_neighbors),
    m_vertices_map(vertices_map)
  {
  }

  void operator()(dynamic_object_type* object)
  {
    if (object->manager().locked())
    {
      return;
    }

    if (object->get_flag(dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE))
    {
      object->iterate(
        [&](
          const typename dynamic_object_type::attr_key_type&,
          dynamic_object_type* neighbor)
        {
          m_non_garbage_collectible_neighbors.insert(neighbor);
        }
      );
    }
    else
    {
      vertex_type& vertex = get_vertex(object);

      object->iterate(
        [&](
          const typename dynamic_object_type::attr_key_type&,
          dynamic_object_type* neighbor)
        {
          vertex_type& neighbor_vertex = get_vertex(neighbor);

          vertex.dependencies().push_back(&neighbor_vertex);
        }
      );
    }
  }

private:
  vertex_type& get_vertex(dynamic_object_type* obj)
  {
    auto itr = m_vertices_map.find(obj);

    if (itr == m_vertices_map.end())
    {
      m_vertices_map[obj] = vertex_type(obj);
    }

    return m_vertices_map.at(obj);
  }

  neighbor_set_type& m_non_garbage_collectible_neighbors;
  vertices_map_type& m_vertices_map;

}; /* end of `ObjectGraphBuilder` */

// -----------------------------------------------------------------------------

template<typename CycleList>
static bool
is_pure_circular_chain(const CycleList& cycle)
{
  bool res = true;
  for (auto itr = cycle.begin(); itr != cycle.end(); ++itr)
  {
    auto obj = static_cast<typename CycleList::value_type>(*itr);
    if (obj->manager().ref_count() > 1)
    {
      res = false;
      break;
    }
  }

  return res;
}

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::remove_cycles(
  RefCountGarbageCollectionScheme::dynamic_object_heap_type& heap) const
{
  using dynamic_object_heap_type = typename
    RefCountGarbageCollectionScheme::dynamic_object_heap_type;

  using dynamic_object_type = typename dynamic_object_heap_type::dynamic_object_type;

  using vertex_type = typename ObjectGraphBuilder::vertex_type;
  using vertices_map_type = typename ObjectGraphBuilder::vertices_map_type;
  using neighbor_set_type = typename ObjectGraphBuilder::neighbor_set_type;

  vertices_map_type vertices_map;
  neighbor_set_type non_garbage_collectible_neighbors;

  ObjectGraphBuilder builder(vertices_map, non_garbage_collectible_neighbors);

  heap.iterate(builder);

  std::list<vertex_type*> vertices;

  for (auto itr = vertices_map.begin(); itr != vertices_map.end(); ++itr)
  {
    auto vertex_ptr = static_cast<vertex_type*>(&itr->second);
    vertices.push_back(vertex_ptr);
  }

  sneaker::algorithm::tarjan<dynamic_object_type*> algo;
  auto components = algo.get_components(vertices);

  const auto cycles = components.cycles();

  for (auto itr = cycles.begin(); itr != cycles.end(); ++itr)
  {
    const auto& cycle = static_cast<std::list<dynamic_object_type*>>(*itr);

    std::set<dynamic_object_type*> cycle_set;

    for (auto itr_ = cycle.begin(); itr_ != cycle.end(); ++itr_)
    {
      auto obj_ = static_cast<dynamic_object_type*>(*itr_);
      cycle_set.insert(obj_);
    }

    std::set<dynamic_object_type*> intersect;
    std::set_intersection(
      non_garbage_collectible_neighbors.begin(),
      non_garbage_collectible_neighbors.end(),
      cycle_set.begin(),
      cycle_set.end(),
      std::inserter(intersect, intersect.begin())
    );

    if (!intersect.empty())
    {
      continue;
    }

    if (!is_pure_circular_chain(cycle))
    {
      continue;
    }

    for (auto itr_ = cycle.begin(); itr_ != cycle.end(); ++itr_)
    {
      auto obj_ = static_cast<dynamic_object_type*>(*itr_);
      obj_->manager().dec_ref_count();
    }
  }
}

// -----------------------------------------------------------------------------


} /* end namespace gc */


} /* end namespace corevm */
