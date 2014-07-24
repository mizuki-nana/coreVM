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

  using dynamic_object_id_type = typename corevm::dyobj::dyobj_id;
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

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

  dynamic_object_type& at(const dynamic_object_id_type)
    throw(corevm::dyobj::dynamic_object_not_found_error);

private:
  dyobj_heap_map_type _map;
};


template<class dynamic_object_manager>
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_heap():
  _map()
{
  // Do nothing here.
}

template<class dynamic_object_manager>
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::~dynamic_object_heap()
{
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::begin() noexcept
{
  return this->_map.begin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::cbegin() const noexcept
{
  return this->_map.cbegin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::end() noexcept
{
  return this->_map.end();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::cend() const noexcept
{
  return this->_map.cend();
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
  throw(corevm::dyobj::dynamic_object_not_found_error)
{
  typename dyobj_heap_map_type::iterator itr = this->_map.find(id);

  if(itr != this->_map.end()) {
    return static_cast<corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_type&>(itr->second);
  }

  throw corevm::dyobj::dynamic_object_not_found_error(id);
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_HEAP_H_ */
