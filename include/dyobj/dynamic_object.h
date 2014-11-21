/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#ifndef COREVM_DYNAMIC_OBJECT_H_
#define COREVM_DYNAMIC_OBJECT_H_

#include <algorithm>
#include <unordered_map>
#include <boost/format.hpp>
#include <sneaker/libc/utils.h>
#include "common.h"
#include "dyobj_id.h"
#include "dyobj_id_helper.h"
#include "flags.h"
#include "errors.h"


#define COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE 0
#define COREVM_DYNAMIC_OBJECT_ATTR_MAP_DEFAULT_SIZE 16


namespace corevm {


namespace dyobj {


template<class dynamic_object_manager>
class dynamic_object {
public:
  typedef corevm::dyobj::attr_key attr_key_type;
  typedef corevm::dyobj::dyobj_id dyobj_id_type;

  typedef std::unordered_map<attr_key_type, dyobj_id_type> attr_map_type;
  typedef attr_map_type::value_type pair;

  typedef attr_map_type::iterator iterator;
  typedef attr_map_type::const_iterator const_iterator;

  static dynamic_object<dynamic_object_manager> create();

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

  corevm::dyobj::ntvhndl_key get_ntvhndl_key() const noexcept;
  void set_ntvhndl_key(corevm::dyobj::ntvhndl_key) noexcept;
  void clear_ntvhndl_key() noexcept;

  corevm::dyobj::instr_block_key get_instr_block_key() const noexcept;
  void set_instr_block_key(corevm::dyobj::instr_block_key) noexcept;
  void clear_instr_block_key() noexcept;

  bool get_flag(char) const;
  void set_flag(char);
  void clear_flag(char);

  bool is_garbage_collectible() const noexcept;

  bool hasattr(attr_key_type) const noexcept;

  void putattr(attr_key_type, dyobj_id_type) noexcept;

  void delattr(attr_key_type)
    throw(corevm::dyobj::object_attribute_deletion_error);

  dyobj_id_type getattr(attr_key_type) const
    throw(corevm::dyobj::object_attribute_not_found_error);

  bool has_ref(dyobj_id_type) const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

private:
  explicit dynamic_object(dyobj_id_type);

  void check_flag_bit(char) const throw(corevm::dyobj::invalid_flag_bit_error);

  dyobj_id_type m_id;
  corevm::dyobj::flag m_flags;
  attr_map_type m_attrs;
  dynamic_object_manager m_manager;
  corevm::dyobj::ntvhndl_key m_ntvhndl_key;
  corevm::dyobj::instr_block_key m_instr_block_key;
};


template<class dynamic_object_manager>
corevm::dyobj::dynamic_object<dynamic_object_manager>
corevm::dyobj::dynamic_object<dynamic_object_manager>::create()
{
  return corevm::dyobj::dynamic_object<dynamic_object_manager>(
    corevm::dyobj::dyobj_id_helper::generate_dyobj_id()
  );
}

template<class dynamic_object_manager>
corevm::dyobj::dynamic_object<dynamic_object_manager>::dynamic_object(corevm::dyobj::dyobj_id id):
  m_id(id),
  m_flags(COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE),
  m_attrs(corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_map_type(COREVM_DYNAMIC_OBJECT_ATTR_MAP_DEFAULT_SIZE)),
  m_manager(dynamic_object_manager()),
  m_ntvhndl_key(corevm::dyobj::NONESET_NTVHNDL_KEY),
  m_instr_block_key(corevm::dyobj::NONESET_INSTR_BLOCK_KEY)
{
  // Do nothing here.
}

template<class dynamic_object_manager>
corevm::dyobj::dynamic_object<dynamic_object_manager>::~dynamic_object()
{
  // Do nothing here.
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
  return m_attrs.begin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::end() noexcept
{
  return m_attrs.end();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::cbegin() const noexcept
{
  return m_attrs.cbegin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object<dynamic_object_manager>::cend() const noexcept
{
  return m_attrs.cend();
}

template<class dynamic_object_manager>
corevm::dyobj::dyobj_id
corevm::dyobj::dynamic_object<dynamic_object_manager>::id() const noexcept
{
  return m_id;
}

template<class dynamic_object_manager>
corevm::dyobj::flag
corevm::dyobj::dynamic_object<dynamic_object_manager>::flags() const noexcept
{
  return m_flags;
}

template<class dynamic_object_manager>
dynamic_object_manager&
corevm::dyobj::dynamic_object<dynamic_object_manager>::manager() noexcept
{
  return m_manager;
}

template<class dynamic_object_manager>
corevm::dyobj::ntvhndl_key
corevm::dyobj::dynamic_object<dynamic_object_manager>::get_ntvhndl_key() const noexcept
{
  return m_ntvhndl_key;
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::set_ntvhndl_key(
  corevm::dyobj::ntvhndl_key ntvhndl_key) noexcept
{
  m_ntvhndl_key = ntvhndl_key;
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::clear_ntvhndl_key() noexcept
{
  m_ntvhndl_key = corevm::dyobj::NONESET_NTVHNDL_KEY;
}

template<class dynamic_object_manager>
corevm::dyobj::instr_block_key
corevm::dyobj::dynamic_object<dynamic_object_manager>::get_instr_block_key() const noexcept
{
  return m_instr_block_key;
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::set_instr_block_key(
  corevm::dyobj::instr_block_key key) noexcept
{
  m_instr_block_key = key;
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::clear_instr_block_key() noexcept
{
  m_instr_block_key = corevm::dyobj::NONESET_INSTR_BLOCK_KEY;
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::check_flag_bit(char bit) const
  throw(corevm::dyobj::invalid_flag_bit_error)
{
  if(bit > sizeof(corevm::dyobj::flag) * 8) {
    throw corevm::dyobj::invalid_flag_bit_error();
  }
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::get_flag(char bit) const
{
  this->check_flag_bit(bit);
  return static_cast<bool>(is_bit_set_uint32(m_flags, bit));
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::set_flag(char bit)
{
  this->check_flag_bit(bit);
  set_nth_bit_uint32(&m_flags, bit);
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::clear_flag(char bit)
{
  this->check_flag_bit(bit);
  clear_nth_bit_uint32(&m_flags, bit);
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::is_garbage_collectible() const noexcept
{
  return get_flag(corevm::dyobj::flags::IS_NOT_GARBAGE_COLLECTIBLE) == false && m_manager.garbage_collectible();
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::hasattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key) const noexcept
{
  return m_attrs.find(attr_key) != m_attrs.end();
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::delattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key)
  throw(corevm::dyobj::object_attribute_deletion_error)
{
  if(m_attrs.erase(attr_key) != 1) {
    throw corevm::dyobj::object_attribute_deletion_error(attr_key, id());
  }
}

template<class dynamic_object_manager>
corevm::dyobj::dyobj_id
corevm::dyobj::dynamic_object<dynamic_object_manager>::getattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key) const
  throw(corevm::dyobj::object_attribute_not_found_error)
{
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_map_type::const_iterator itr = m_attrs.find(attr_key);

  if(itr == m_attrs.cend()) {
    throw corevm::dyobj::object_attribute_not_found_error(attr_key, id());
  }

  return static_cast<corevm::dyobj::dyobj_id>(itr->second);
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object<dynamic_object_manager>::putattr(
  corevm::dyobj::dynamic_object<dynamic_object_manager>::attr_key_type attr_key,
  corevm::dyobj::dynamic_object<dynamic_object_manager>::dyobj_id_type obj_id) noexcept
{
  m_attrs.insert({attr_key, obj_id});
}

template<class dynamic_object_manager>
bool
corevm::dyobj::dynamic_object<dynamic_object_manager>::has_ref(dyobj_id_type id) const noexcept
{
  return std::find_if(
    cbegin(),
    cend(),
    [&id](const corevm::dyobj::dynamic_object<dynamic_object_manager>::pair& pair) -> bool {
      return id == static_cast<corevm::dyobj::dynamic_object<dynamic_object_manager>::dyobj_id_type>(pair.second);
    }
  ) != cend();
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
