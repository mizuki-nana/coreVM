#ifndef COREVM_DYNAMIC_OBJECT_H_
#define COREVM_DYNAMIC_OBJECT_H_

#include <algorithm>
#include <unordered_map>
#include <boost/format.hpp>
#include <sneaker/libc/utils.h>
#include "../types/native_type_handle.h"
#include "common.h"
#include "dyobj_id.h"
#include "errors.h"


#define COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE 0
#define COREVM_DYNAMIC_OBJECT_ATTR_MAP_DEFAULT_SIZE 16


namespace corevm {


namespace dyobj {


template<class dynamic_object_manager>
class dynamic_object {
public:
  using attr_key_type = typename corevm::dyobj::attr_key;
  using dyobj_id_type = typename corevm::dyobj::dyobj_id;

  using attr_map_type = typename std::unordered_map<attr_key_type, dyobj_id_type>;
  using pair = typename attr_map_type::value_type;

  using iterator = typename attr_map_type::iterator;
  using const_iterator = typename attr_map_type::const_iterator;

  explicit dynamic_object(dyobj_id_type&);

  ~dynamic_object();

  bool operator==(const corevm::dyobj::dynamic_object<dynamic_object_manager>&);
  bool operator!=(const corevm::dyobj::dynamic_object<dynamic_object_manager>&);

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  corevm::dyobj::dyobj_id id() const noexcept;
  corevm::dyobj::flag flags() const noexcept;

  dynamic_object_manager& manager() noexcept;

  corevm::types::native_type_handle* get_ntvhdle() noexcept;
  void set_ntvhdle(corevm::types::native_type_handle*) noexcept;

  bool get_flag(char) noexcept;
  void set_flag(char) noexcept;
  void clear_flag(char) noexcept;

  bool hasattr(attr_key_type) const noexcept;

  void putattr(attr_key_type, dyobj_id_type) noexcept;

  void delattr(attr_key_type)
    throw(corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);

  dyobj_id_type getattr(attr_key_type) const
    throw(corevm::dyobj::dynamic_object_attribute_not_found_error);

  template<typename Function>
  void iterate(Function) noexcept;

private:
  dyobj_id_type _id;
  corevm::dyobj::flag _flags;
  attr_map_type _attrs;
  dynamic_object_manager _manager;
  corevm::types::native_type_handle* _ntvhdle = nullptr;
};


template<class dynamic_object_manager>
corevm::dyobj::dynamic_object<dynamic_object_manager>::dynamic_object(corevm::dyobj::dyobj_id& id):
  _id(id),
  _flags(COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE),
  _attrs(corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_map_type(COREVM_DYNAMIC_OBJECT_ATTR_MAP_DEFAULT_SIZE)),
  _manager(dynamic_object_manager())
{
}

template<class dynamic_object_manager>
corevm::dyobj::dynamic_object<dynamic_object_manager>::~dynamic_object()
{
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::operator==(
  const corevm::dyobj::dynamic_object<dynamic_object_manager>& rhs)
{
  return this->id() == rhs.id();
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::operator!=(
  const corevm::dyobj::dynamic_object<dynamic_object_manager>& rhs)
{
  return !(static_cast<corevm::dyobj::dynamic_object<dynamic_object_manager>>(*this) == rhs);
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::begin() noexcept
{
  return _attrs.begin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::end() noexcept
{
  return _attrs.end();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::cbegin() const noexcept
{
  return _attrs.cbegin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::cend() const noexcept
{
  return _attrs.cend();
}

template<class dynamic_object_manager>
corevm::dyobj::dyobj_id
corevm::dyobj::dynamic_object<dynamic_object_manager>::id() const noexcept
{
  return this->_id;
}

template<class dynamic_object_manager>
corevm::dyobj::flag
corevm::dyobj::dynamic_object<dynamic_object_manager>::flags() const noexcept
{
  return this->_flags;
}

template<class dynamic_object_manager>
dynamic_object_manager&
corevm::dyobj::dynamic_object<dynamic_object_manager>::manager() noexcept
{
  return this->_manager;
}

template<class dynamic_object_manager>
corevm::types::native_type_handle*
corevm::dyobj::dynamic_object<dynamic_object_manager>::get_ntvhdle() noexcept
{
  return this->_ntvhdle;
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::set_ntvhdle(
  corevm::types::native_type_handle* ntvhdle) noexcept
{
  this->_ntvhdle = ntvhdle;
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::get_flag(char bit) noexcept
{
  return static_cast<bool>(is_bit_set_uint32(this->_flags, bit));
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::set_flag(char bit) noexcept
{
  set_nth_bit_uint32(&this->_flags, bit);
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::clear_flag(char bit) noexcept
{
  clear_nth_bit_uint32(&this->_flags, bit);
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::hasattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key) const noexcept
{
  return this->_attrs.find(attr_key) != this->_attrs.end();
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::delattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key)
  throw(corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error)
{
  if(this->_attrs.erase(attr_key) != 1) {
    throw corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error(attr_key, id());
  }
}

template<class dynamic_object_manager>
corevm::dyobj::dyobj_id
corevm::dyobj::dynamic_object<dynamic_object_manager>::getattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key) const
  throw(corevm::dyobj::dynamic_object_attribute_not_found_error)
{
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_map_type::const_iterator itr = this->_attrs.find(attr_key);

  if(itr == this->_attrs.cend()) {
    throw corevm::dyobj::dynamic_object_attribute_not_found_error(attr_key, id());
  }

  return static_cast<corevm::dyobj::dyobj_id>(itr->second);
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::putattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key,
  corevm::dyobj::dynamic_object<dynamic_object_manager>::dyobj_id_type obj_id) noexcept
{
  this->_attrs.insert({attr_key, obj_id});
}

template<class dynamic_object_manager>
template<typename Function>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::iterate(Function func) noexcept
{
  std::for_each(
    begin(),
    end(),
    [&func](corevm::dyobj::dynamic_object<dynamic_object_manager>::pair& pair) {
      func(
        static_cast<corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type>(pair.first),
        static_cast<corevm::dyobj::dynamic_object<dynamic_object_manager>::dyobj_id_type>(pair.second)
      );
    }
  );
}

template<class dynamic_object_manager>
inline
bool operator==(
  const corevm::dyobj::dynamic_object<dynamic_object_manager>& lhs,
  const corevm::dyobj::dynamic_object<dynamic_object_manager>& rhs)
{
  return lhs.id() == rhs.id();
}

template<class dynamic_object_manager>
inline
bool operator!=(
  const corevm::dyobj::dynamic_object<dynamic_object_manager>& lhs,
  const corevm::dyobj::dynamic_object<dynamic_object_manager>& rhs)
{
  return !operator==(lhs, rhs);
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_H_ */
