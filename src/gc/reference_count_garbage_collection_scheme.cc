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


// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::gc(
  RefCountGarbageCollectionScheme::dynamic_object_heap_type& /* heap */) const
{
#if COREVM_457
  using _dynamic_object_heap_type = typename
    RefCountGarbageCollectionScheme::dynamic_object_heap_type;

  using heap_iterator_type = typename
    RefCountGarbageCollectionScheme::heap_iterator<_dynamic_object_heap_type>;

  heap_iterator_type heap_iterator(heap, *this);

  _dynamic_object_heap_type::size_type prev_active_size = heap.active_size();

  while (true)
  {
    heap.iterate(heap_iterator);

    this->remove_cycles(heap);

    if (heap.active_size() == prev_active_size)
    {
      break;
    }

    prev_active_size = heap.active_size();
  } /* end of `while (true)` */
#endif /* if COREVM_457 */
}

// -----------------------------------------------------------------------------

#if COREVM_457

template<typename dynamic_object_heap_type>
struct object_ref_count_decrementor
{
private:
  using dynamic_object_type = typename dynamic_object_heap_type::dynamic_object_type;

public:
  object_ref_count_decrementor(dynamic_object_heap_type& heap)
    :
    m_heap(heap)
  {
  }

  void operator()(
    const typename dynamic_object_type::attr_key_type& /* attr_key */,
    const typename dynamic_object_type::dyobj_id_type& dyobj_id)
  {
    dynamic_object_type& referenced_object = m_heap.at(dyobj_id);
    referenced_object.manager().dec_ref_count();
  }

private:
  dynamic_object_heap_type& m_heap;
};

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::check_and_dec_ref_count(
  RefCountGarbageCollectionScheme::dynamic_object_heap_type& heap,
  RefCountGarbageCollectionScheme::dynamic_object_type& object) const
{
  using _dynamic_object_heap_type = typename
    RefCountGarbageCollectionScheme::dynamic_object_heap_type;

  if (!object.is_garbage_collectible())
  {
    return;
  }

  object_ref_count_decrementor<_dynamic_object_heap_type> decrementor(heap);
  object.iterate(decrementor);
}

// -----------------------------------------------------------------------------

template<typename dynamic_object_heap_type>
struct cycled_object_reference_decrementor
{
private:
  using dynamic_object_type = typename dynamic_object_heap_type::dynamic_object_type;

public:
  cycled_object_reference_decrementor(
    dynamic_object_heap_type& heap,
    dynamic_object_type& object)
    :
    m_heap(heap),
    m_object(object)
  {
  }

  void operator()(
    const typename dynamic_object_type::attr_key_type& /* attr_key */,
    const typename dynamic_object_type::dyobj_id_type& dyobj_id)
  {
    dynamic_object_type& referenced_object = m_heap.at(dyobj_id);

    auto ref_count1 = m_object.manager().ref_count();
    auto ref_count2 = referenced_object.manager().ref_count();

    if (referenced_object.has_ref(m_object.id()) && ref_count1 == 1 && ref_count2 == 1)
    {
      m_object.manager().dec_ref_count();
      referenced_object.manager().dec_ref_count();
    }
  }

private:
  dynamic_object_heap_type& m_heap;
  dynamic_object_type& m_object;

}; /* end of `cycled_object_reference_decrementor` */

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::resolve_self_reference_cycles(
  RefCountGarbageCollectionScheme::dynamic_object_heap_type& heap,
  RefCountGarbageCollectionScheme::dynamic_object_type& object) const
{
  using _dynamic_object_heap_type = typename
    RefCountGarbageCollectionScheme::dynamic_object_heap_type;

  cycled_object_reference_decrementor<_dynamic_object_heap_type> decrementor(heap, object);
  object.iterate(decrementor);
}

// -----------------------------------------------------------------------------

template<typename dynamic_object_heap_type>
struct object_graph_builder
{
public:
  using dynamic_object_type = typename dynamic_object_heap_type::dynamic_object_type;
  using dyobj_id_type = typename dynamic_object_type::dyobj_id_type;
  using vertex_type = typename sneaker::algorithm::tarjan<dyobj_id_type>::vertex;
  using vertices_map_type = typename std::unordered_map<dyobj_id_type, vertex_type>;
  using neighbor_set_type = typename std::set<dyobj_id_type>;

public:
  object_graph_builder(
    vertices_map_type& vertices_map,
    neighbor_set_type& non_garbage_collectible_neighbors)
    :
    m_non_garbage_collectible_neighbors(non_garbage_collectible_neighbors),
    m_vertices_map(vertices_map)
  {
  }

  void operator()(dyobj_id_type id, dynamic_object_type& object)
  {
    if (object.get_flag(dyobj::flags::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE))
    {
      object.iterate(
        [&](
          const typename dynamic_object_type::attr_key_type& /* attr_key */,
          const typename dynamic_object_type::dyobj_id_type& neighbor_id)
        {
          m_non_garbage_collectible_neighbors.insert(neighbor_id);
        }
      );

      return;
    }

    vertex_type& vertex = get_vertex(id);

    object.iterate(
      [&](
        const typename dynamic_object_type::attr_key_type& /* attr_key */,
        const typename dynamic_object_type::dyobj_id_type& neighbor_id)
      {
        vertex_type& neighbor_vertex = get_vertex(neighbor_id);

        vertex.dependencies().push_back(&neighbor_vertex);
      }
    );
  }

private:
  vertex_type& get_vertex(const dyobj_id_type& id)
  {
    auto itr = m_vertices_map.find(id);

    if (itr == m_vertices_map.end())
    {
      m_vertices_map[id] = vertex_type(id);
    }

    return m_vertices_map.at(id);
  }

  neighbor_set_type& m_non_garbage_collectible_neighbors;
  vertices_map_type& m_vertices_map;

}; /* end of `object_graph_builder` */

// -----------------------------------------------------------------------------

void
RefCountGarbageCollectionScheme::remove_cycles(
  RefCountGarbageCollectionScheme::dynamic_object_heap_type& heap) const
{
  using _dynamic_object_heap_type = typename
    RefCountGarbageCollectionScheme::dynamic_object_heap_type;

  typedef object_graph_builder<_dynamic_object_heap_type> object_graph_builder_type;

  using vertex_type = typename object_graph_builder_type::vertex_type;
  using dyobj_id_type = typename object_graph_builder_type::dyobj_id_type;
  using vertices_map_type = typename object_graph_builder_type::vertices_map_type;
  using neighbor_set_type = typename object_graph_builder_type::neighbor_set_type;

  vertices_map_type vertices_map;
  neighbor_set_type non_garbage_collectible_neighbors;

  object_graph_builder_type builder(
    vertices_map, non_garbage_collectible_neighbors);

  heap.iterate(builder);

  std::list<vertex_type*> vertices;

  for (auto itr = vertices_map.begin(); itr != vertices_map.end(); ++itr)
  {
    vertex_type* vertex_ptr = static_cast<vertex_type*>(&itr->second);
    vertices.push_back(vertex_ptr);
  }

  sneaker::algorithm::tarjan<dyobj_id_type> algo;
  auto components = algo.get_components(vertices);

  std::list<std::list<dyobj_id_type>> cycles = components.cycles();

  for (auto itr = cycles.begin(); itr != cycles.end(); ++itr)
  {
    std::list<dyobj_id_type> cycle = static_cast<std::list<dyobj_id_type>>(*itr);

    std::set<dyobj_id_type> cycle_set;

    for (auto itr_ = cycle.begin(); itr_ != cycle.end(); ++itr_)
    {
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

    if (!intersect.empty())
    {
      continue;
    }

    for (auto itr_ = cycle.begin(); itr_ != cycle.end(); ++itr_)
    {
      dyobj_id_type id = static_cast<dyobj_id_type>(*itr_);
      auto& obj = heap.at(id);
      obj.manager().dec_ref_count();
    }
  }
}
#endif /* if COREVM_457 */

// -----------------------------------------------------------------------------


} /* end namespace gc */


} /* end namespace corevm */
