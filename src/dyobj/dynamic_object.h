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
#ifndef COREVM_DYNAMIC_OBJECT_H_
#define COREVM_DYNAMIC_OBJECT_H_

#include "corevm/macros.h"
#include "dyobj/common.h"
#include "dyobj/flags.h"
#include "dyobj/errors.h"
#include "runtime/closure_ctx.h"
#include "types/fwd.h"

#include <boost/format.hpp>
#include <sneaker/libc/utils.h>

#if COREVM_USE_SMALL_ATTRIBUTE_TABLE
  #include "corevm/llvm_smallvector.h"
#else
  #include <vector>
#endif // COREVM_USE_SMALL_ATTRIBUTE_TABLE

#include <algorithm>


namespace corevm {
namespace dyobj {

template<class DynamicObjectManager>
class DynamicObject
{
public:
  typedef attr_key_t attr_key_type;
  typedef DynamicObject<DynamicObjectManager>* dyobj_ptr;

  typedef std::pair<attr_key_type, dyobj_ptr> attr_key_value_pair;

#if COREVM_USE_SMALL_ATTRIBUTE_TABLE
  typedef llvm::SmallVector<attr_key_value_pair, 20> attr_map_type;
#else
  typedef std::vector<attr_key_value_pair> attr_map_type;
#endif

  typedef typename attr_map_type::iterator iterator;
  typedef typename attr_map_type::const_iterator const_iterator;

  DynamicObject();

  /* Dynamic objects should not be copyable. */
  DynamicObject(const DynamicObject&) = delete;
  DynamicObject& operator=(const DynamicObject&) = delete;

  ~DynamicObject();

  bool operator==(const DynamicObject<DynamicObjectManager>&);
  bool operator!=(const DynamicObject<DynamicObjectManager>&);

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  dyobj_id_t id() const noexcept;

  flag_t flags() const noexcept;

  DynamicObjectManager& manager() noexcept;

  const types::NativeTypeHandle& ntvhndl() const noexcept;
  void set_ntvhndl(types::NativeTypeHandle*) noexcept;
  void clear_ntvhndl() noexcept;
  bool has_ntvhndl() const noexcept;

  bool get_flag(char) const;
  void set_flag(char);
  void clear_flag(char);

  bool is_garbage_collectible() const noexcept;

  size_t attr_count() const;

  bool hasattr(attr_key_type) const noexcept;

  void putattr(attr_key_type, dyobj_ptr) noexcept;

  void delattr(attr_key_type);

  dyobj_ptr getattr(attr_key_type) const;

  bool getattr(attr_key_type, dyobj_ptr*) const;

  const runtime::ClosureCtx& closure_ctx() const;

  void set_closure_ctx(const runtime::ClosureCtx&);

  bool has_ref(dyobj_ptr) const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

  void copy_from(const DynamicObject<DynamicObjectManager>&);

private:
  void check_flag_bit(char) const;

  struct AttributeKeyPred
  {
    explicit AttributeKeyPred(attr_key_t key)
      :
      m_key(key)
    {
    }

    bool operator()(const attr_key_value_pair& pair) const
    {
      return m_key == pair.first;
    }

  private:
    attr_key_t m_key;
  };

  struct AttributeValuePred
  {
    explicit AttributeValuePred(dyobj_ptr value)
      :
      m_value(value)
    {
    }

    bool operator()(const attr_key_value_pair& pair) const
    {
      return m_value == pair.second;
    }

  private:
    dyobj_ptr m_value;
  };

  flag_t m_flags;
  attr_map_type m_attrs;
  DynamicObjectManager m_manager;
  types::NativeTypeHandle* m_ntvhndl;
  runtime::ClosureCtx m_closure_ctx;
};

// -----------------------------------------------------------------------------

const int COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE = 0;

// -----------------------------------------------------------------------------

static const size_t COREVM_DYNAMIC_OBJECT_DEFAULT_ATTRIBUTE_COUNT = 10u;

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObject<DynamicObjectManager>::DynamicObject()
  :
  m_flags(COREVM_DYNAMIC_OBJECT_DEFAULT_FLAG_VALUE),
  m_attrs(),
  m_manager(),
  m_ntvhndl(NULL),
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
  return id() == rhs.id();
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
  return m_attrs.begin();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObject<DynamicObjectManager>::const_iterator
DynamicObject<DynamicObjectManager>::cend() const noexcept
{
  return m_attrs.end();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
dyobj_id_t
DynamicObject<DynamicObjectManager>::id() const noexcept
{
  return static_cast<dyobj_id_t>(
    reinterpret_cast<const uint8_t*>(this) - reinterpret_cast<uint8_t*>(0));
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
flag_t
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
const types::NativeTypeHandle&
DynamicObject<DynamicObjectManager>::ntvhndl() const noexcept
{
#if __DEBUG__
  ASSERT(m_ntvhndl);
#endif
  return *m_ntvhndl;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::set_ntvhndl(types::NativeTypeHandle* ntvhndl) noexcept
{
  m_ntvhndl = ntvhndl;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::clear_ntvhndl() noexcept
{
  m_ntvhndl = NULL;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::has_ntvhndl() const noexcept
{
  return m_ntvhndl != NULL;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::check_flag_bit(char bit) const
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
  check_flag_bit(bit);
  return static_cast<bool>(is_bit_set_uint32(m_flags, bit));
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::set_flag(char bit)
{
  check_flag_bit(bit);
  set_nth_bit_uint32(&m_flags, bit);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::clear_flag(char bit)
{
  check_flag_bit(bit);
  clear_nth_bit_uint32(&m_flags, bit);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::is_garbage_collectible() const noexcept
{
  return (
    get_flag(DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE) == false &&
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
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(), AttributeKeyPred(attr_key));
  return itr != m_attrs.end();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::delattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key)
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(), AttributeKeyPred(attr_key));
  if (itr == m_attrs.end())
  {
    THROW(ObjectAttributeNotFoundError(attr_key, id()));
  }
  m_attrs.erase(itr);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObject<DynamicObjectManager>::dyobj_ptr
DynamicObject<DynamicObjectManager>::getattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key) const
{
  DynamicObject<DynamicObjectManager>::dyobj_ptr attr_ptr = NULL;

  bool res = getattr(attr_key, &attr_ptr);
  if (!res)
  {
    THROW(ObjectAttributeNotFoundError(attr_key, id()));
  }

  return attr_ptr;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
bool
DynamicObject<DynamicObjectManager>::getattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key,
  dyobj_ptr* attr_ptr) const
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(),
    AttributeKeyPred(attr_key));

  bool res = itr != m_attrs.end();

  if (res)
  {
    *attr_ptr = itr->second;
  }

  return res;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObject<DynamicObjectManager>::putattr(
  DynamicObject<DynamicObjectManager>::attr_key_type attr_key,
  DynamicObject<DynamicObjectManager>::dyobj_ptr obj_ptr) noexcept
{
  auto itr = std::find_if(m_attrs.begin(), m_attrs.end(),
    AttributeKeyPred(attr_key));

  if (itr == m_attrs.end())
  {
    m_attrs.push_back(std::make_pair(attr_key, obj_ptr));
  }
  else
  {
    (*itr).second = obj_ptr;
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
DynamicObject<DynamicObjectManager>::has_ref(dyobj_ptr ref_ptr) const noexcept
{
  return std::find_if(cbegin(), cend(), AttributeValuePred(ref_ptr)) != cend();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
template<typename Function>
void
DynamicObject<DynamicObjectManager>::iterate(Function func) noexcept
{
  std::for_each(begin(), end(),
    [&func](DynamicObject<DynamicObjectManager>::attr_key_value_pair& pair) {
      func(
        static_cast<DynamicObject<DynamicObjectManager>::attr_key_type>(pair.first),
        static_cast<DynamicObject<DynamicObjectManager>::dyobj_ptr>(pair.second)
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
