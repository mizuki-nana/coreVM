#ifndef COREVM_DYNAMIC_OBJECT_HEAP_H_
#define COREVM_DYNAMIC_OBJECT_HEAP_H_

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <sneaker/allocator/allocator.h>
#include "common.h"
#include "dynamic_object.h"
#include "dyobj_heap_allocator.h"
#include "errors.h"


namespace corevm {


namespace dyobj {


template<class dynamic_object_manager>
class dynamic_object_heap {
public:

  typedef corevm::dyobj::dyobj_id dynamic_object_id_type;
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<dynamic_object_manager>;

  using dyobj_heap_map_type = typename std::unordered_map<
    dynamic_object_id_type,
    dynamic_object_type,
    std::hash<dynamic_object_id_type>,
    std::equal_to<dynamic_object_id_type>,
    corevm::dyobj::dyobj_heap_allocator<
      std::pair<const dynamic_object_id_type, dynamic_object_type>,
      COREVM_DEFAULT_HEAP_SIZE
    >
  >;

  using pair                = typename dyobj_heap_map_type::value_type;
  using hasher              = typename dyobj_heap_map_type::hasher;
  using key_equal           = typename dyobj_heap_map_type::key_equal;
  using allocator_type      = typename dyobj_heap_map_type::allocator_type;
  using reference           = typename dyobj_heap_map_type::reference;
  using const_reference     = typename dyobj_heap_map_type::const_reference;
  using pointer             = typename dyobj_heap_map_type::pointer;
  using const_pointer       = typename dyobj_heap_map_type::const_pointer;
  using iterator            = typename dyobj_heap_map_type::iterator;
  using const_iterator      = typename dyobj_heap_map_type::const_iterator;
  using size_type           = typename dyobj_heap_map_type::size_type;
  using difference_type     = typename dyobj_heap_map_type::difference_type;

  explicit dynamic_object_heap();
  ~dynamic_object_heap();

  size_type size() const noexcept {
    return m_map.size();
  }

  size_type max_size() const noexcept {
    return m_map.max_size();
  }

  size_type active_size() const noexcept;

  iterator erase(const_iterator pos) {
    return m_map.erase(pos);
  }

  void erase(dynamic_object_id_type id) {
    auto itr = m_map.find(id);

    if(itr != m_map.end()) {
      erase(itr);
    }
  }

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

  dynamic_object_type& at(const dynamic_object_id_type)
    throw(corevm::dyobj::object_not_found_error);

  dynamic_object_id_type create_dyobj()
    throw(corevm::dyobj::object_heap_insertion_failed_error);

private:
  dyobj_heap_map_type m_map;
};


template<class dynamic_object_manager>
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_heap():
  m_map()
{
  // Do nothing here.
}

template<class dynamic_object_manager>
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::~dynamic_object_heap()
{
  // Do nothing here.
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::size_type
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::active_size() const noexcept
{
  return std::count_if(
    cbegin(),
    cend(),
    [](const corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::pair& pair) -> bool {
      const auto& obj = static_cast<corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_type>(pair.second);
      return !obj.is_garbage_collectible();
    }
  );
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::begin() noexcept
{
  return m_map.begin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::cbegin() const noexcept
{
  return m_map.cbegin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::end() noexcept
{
  return m_map.end();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::cend() const noexcept
{
  return m_map.cend();
}

template<class dynamic_object_manager>
template<typename Function>
void
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterate(Function func) noexcept
{
  std::for_each(
    begin(),
    end(),
    [&func](corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::pair& pair) {
      func(
        static_cast<corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_id_type>(pair.first),
        static_cast<corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_type>(pair.second)
      );
    }
  );
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_type&
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::at(
  const corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_id_type id)
  throw(corevm::dyobj::object_not_found_error)
{
  typename dyobj_heap_map_type::iterator itr = m_map.find(id);

  if(itr == m_map.end()) {
    throw corevm::dyobj::object_not_found_error(id);
  }

  return static_cast<corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_type&>(itr->second);
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_id_type
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::create_dyobj()
  throw(corevm::dyobj::object_heap_insertion_failed_error)
{
  dynamic_object_id_type id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  dynamic_object_type dyobj(id);
  auto res = m_map.insert(std::pair<dynamic_object_id_type, dynamic_object_type>(id, dyobj));

  if(res.second == false) {
    throw corevm::dyobj::object_heap_insertion_failed_error();
  }

  return id;
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_HEAP_H_ */
