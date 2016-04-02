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
#ifndef COREVM_DYNAMIC_OBJECT_H_
#define COREVM_DYNAMIC_OBJECT_H_

#include "corevm/macros.h"
#include "dyobj/common.h"
#include "dyobj/dyobj_id.h"
#include "dyobj/flags.h"
#include "dyobj/errors.h"
#include "runtime/closure_ctx.h"

#include <boost/format.hpp>
#include <sneaker/libc/utils.h>

#include <algorithm>
#include <vector>


namespace corevm {


namespace dyobj {


template<class DynamicObjectManager>
class DynamicObject
{
public:
  typedef attr_key attr_key_type;
  typedef dyobj_id dyobj_id_type;

  typedef std::pair<attr_key_type, dyobj_id_type> attr_key_value_pair;
  typedef std::vector<attr_key_value_pair> attr_map_type;

  typedef attr_map_type::iterator iterator;
  typedef attr_map_type::const_iterator const_iterator;

  DynamicObject();

  /* Dynamic objects should not be copyable. */
  DynamicObject(const DynamicObject&) = delete;
  DynamicObject& operator=(const DynamicObject&) = delete;

  ~DynamicObject();

  bool operator==(const DynamicObject<DynamicObjectManager>&);
  bool operator!=(const DynamicObject<DynamicObjectManager>&);

  void set_id(dyobj_id id) noexcept;

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  dyobj_id id() const noexcept;

  flag flags() const noexcept;

  DynamicObjectManager& manager() noexcept;

  ntvhndl_key ntvhndl_key() const noexcept;
  void set_ntvhndl_key(dyobj::ntvhndl_key) noexcept;
  void clear_ntvhndl_key() noexcept;

  bool get_flag(char) const;
  void set_flag(char);
  void clear_flag(char);

  bool is_garbage_collectible() const noexcept;

  size_t attr_count() const;

  bool hasattr(attr_key_type) const noexcept;

  void putattr(attr_key_type, dyobj_id_type) noexcept;

  void delattr(attr_key_type)
    throw(ObjectAttributeNotFoundError);

  dyobj_id_type getattr(attr_key_type) const
    throw(ObjectAttributeNotFoundError);

  bool getattr(attr_key_type, dyobj_id_type*) const;

  const runtime::ClosureCtx& closure_ctx() const;

  void set_closure_ctx(const runtime::ClosureCtx&);

  bool has_ref(dyobj_id_type) const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

  void copy_from(const DynamicObject<DynamicObjectManager>&);

private:
  void check_flag_bit(char) const throw(InvalidFlagBitError);

  struct attr_key_pred
  {
    explicit attr_key_pred(attr_key key)
      :
      m_key(key)
    {
    }

    bool operator()(const attr_key_value_pair& pair) const
    {
      return m_key == pair.first;
    }

  private:
    attr_key m_key;
  };

  struct attr_value_pred
  {
    explicit attr_value_pred(dyobj_id value)
      :
      m_value(value)
    {
    }

    bool operator()(const attr_key_value_pair& pair) const
    {
      return m_value == pair.second;
    }

  private:
    dyobj_id m_value;
  };

  dyobj_id_type m_id;
  flag m_flags;
  attr_map_type m_attrs;
  DynamicObjectManager m_manager;
  dyobj::ntvhndl_key m_ntvhndl_key;
  runtime::ClosureCtx m_closure_ctx;
};

// -----------------------------------------------------------------------------

const int COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE = 0;

// -----------------------------------------------------------------------------

static const size_t COREVM_DYNAMIC_OBJECT_DEFAULT_ATTRIBUTE_COUNT = 10u;

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObject<DynamicObjectManager>::DynamicObject():
  m_id(0u),
  m_flags(COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE),
  m_attrs(),
  m_manager(),
  m_ntvhndl_key(NONESET_NTVHNDL_KEY),
  m_closure_ctx(runtime::ClosureCtx(
    runtime::NONESET_COMPARTMENT_ID, runtime::NONESET_CLOSURE_ID))
{
  m_attrs.reserve(COREVM_DYNAMIC_OBJECT_DEFAULT_ATTRIBUTE_COUNT);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObject<DynamicObjectManager>::~DynamicObject()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::operator==(
  const DynamicObject<DynamicObjectManager>& rhs)
{
  return this->id() == rhs.id();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::operator!=(
  const DynamicObject<DynamicObjectManager>& rhs)
{
  return !((*this) == rhs);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::set_id(
  dyobj_id id) noexcept
{
  m_id = id;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObject<DynamicObjectManager>::iterator
DynamicObject<DynamicObjectManager>::begin() noexcept
{
  return m_attrs.begin();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObject<DynamicObjectManager>::iterator
DynamicObject<DynamicObjectManager>::end() noexcept
{
  return m_attrs.end();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObject<DynamicObjectManager>::const_iterator
DynamicObject<DynamicObjectManager>::cbegin() const noexcept
{
  return m_attrs.cbegin();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObject<DynamicObjectManager>::const_iterator
DynamicObject<DynamicObjectManager>::cend() const noexcept
{
  return m_attrs.cend();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
dyobj_id
DynamicObject<DynamicObjectManager>::id() const noexcept
{
  return m_id;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
flag
DynamicObject<DynamicObjectManager>::flags() const noexcept
{
  return m_flags;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObjectManager&
DynamicObject<DynamicObjectManager>::manager() noexcept
{
  return m_manager;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
ntvhndl_key
DynamicObject<DynamicObjectManager>::ntvhndl_key() const noexcept
{
  return m_ntvhndl_key;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::set_ntvhndl_key(
  dyobj::ntvhndl_key ntvhndl_key) noexcept
{
  m_ntvhndl_key = ntvhndl_key;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::clear_ntvhndl_key() noexcept
{
  m_ntvhndl_key = NONESET_NTVHNDL_KEY;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::check_flag_bit(char bit) const
  throw(InvalidFlagBitError)
{
  if (!is_valid_flag_bit(bit))
  {
    THROW(InvalidFlagBitError());
  }
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::get_flag(char bit) const
{
  this->check_flag_bit(bit);
  return static_cast<bool>(is_bit_set_uint32(m_flags, bit));
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::set_flag(char bit)
{
  this->check_flag_bit(bit);
  set_nth_bit_uint32(&m_flags, bit);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::clear_flag(char bit)
{
  this->check_flag_bit(bit);
  clear_nth_bit_uint32(&m_flags, bit);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::is_garbage_collectible() const noexcept
{
  return (
    get_flag(flags::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE) == false &&
    m_manager.garbage_collectible()
  );
}

// -----------------------------------------------------------------------------

template<typename DynamicObjectManager>
size_t
DynamicObject<DynamicObjectManager>::attr_count() const
{
  return m_attrs.size();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::hasattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key) const noexcept
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(), attr_key_pred(attr_key));
  return itr != m_attrs.end();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::delattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key)
  throw(ObjectAttributeNotFoundError)
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(), attr_key_pred(attr_key));
  if (itr == m_attrs.end())
  {
    THROW(ObjectAttributeNotFoundError(attr_key, id()));
  }
  m_attrs.erase(itr);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
dyobj_id
DynamicObject<DynamicObjectManager>::getattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key) const
  throw(ObjectAttributeNotFoundError)
{
  dyobj_id attr_id = 0;

  bool res = getattr(attr_key, &attr_id);
  if (!res)
  {
    THROW(ObjectAttributeNotFoundError(attr_key, id()));
  }

  return attr_id;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::getattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key,
  dyobj_id_type* attr_id) const
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(), attr_key_pred(attr_key));
  bool res = itr != m_attrs.end();

  if (res)
  {
    *attr_id = itr->second;
  }

  return res;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::putattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key,
  DynamicObject<DynamicObjectManager>::dyobj_id_type obj_id) noexcept
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(), attr_key_pred(attr_key));
  if (itr == m_attrs.end())
  {
    m_attrs.emplace_back(attr_key, obj_id);
  }
  else
  {
    (*itr).second = obj_id;
  }
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
const corevm::runtime::ClosureCtx&
DynamicObject<DynamicObjectManager>::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::set_closure_ctx(
  const runtime::ClosureCtx& ctx)
{
  m_closure_ctx = ctx;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::has_ref(dyobj_id_type id) const noexcept
{
  return std::find_if(cbegin(), cend(), attr_value_pred(id)) != cend();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
template<typename Function>
void
DynamicObject<DynamicObjectManager>::iterate(Function func) noexcept
{
  std::for_each(
    begin(),
    end(),
    [&func](DynamicObject<DynamicObjectManager>::attr_key_value_pair& pair) {
      func(
        static_cast<DynamicObject<DynamicObjectManager>::attr_key_type>(pair.first),
        static_cast<DynamicObject<DynamicObjectManager>::dyobj_id_type>(pair.second)
      );
    }
  );
}

// -----------------------------------------------------------------------------

template <class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::copy_from(
  const DynamicObject<DynamicObjectManager>& src)
{
  // NOTE: Need to be careful about what fields are being copied here.
  m_flags = src.m_flags;
  m_attrs = src.m_attrs;
  m_closure_ctx = src.m_closure_ctx;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
inline
bool operator==(
  const DynamicObject<DynamicObjectManager>& lhs,
  const DynamicObject<DynamicObjectManager>& rhs)
{
  return lhs.id() == rhs.id();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
inline
bool operator!=(
  const DynamicObject<DynamicObjectManager>& lhs,
  const DynamicObject<DynamicObjectManager>& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_H_ */
