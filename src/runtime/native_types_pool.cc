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
#include "native_types_pool.h"

#include "common.h"
#include "utils.h"
#include "corevm/macros.h"
#include "types/native_type_handle.h"

#include <ostream>


namespace {

// -----------------------------------------------------------------------------

typedef corevm::runtime::NativeTypesPool _MyType;

// -----------------------------------------------------------------------------

} /* anonymous namespace */


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

NativeTypesPool::NativeTypesPool()
  :
  m_container(COREVM_DEFAULT_NATIVE_TYPES_POOL_SIZE)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

NativeTypesPool::NativeTypesPool(uint64_t total_size)
  :
  m_container(total_size)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

_MyType::size_type
NativeTypesPool::size() const
{
  return m_container.size();
}

// -----------------------------------------------------------------------------

_MyType::size_type
NativeTypesPool::max_size() const
{
  return m_container.max_size();
}

// -----------------------------------------------------------------------------

_MyType::size_type
NativeTypesPool::total_size() const
{
  return m_container.total_size();
}

// -----------------------------------------------------------------------------

_MyType::reference
NativeTypesPool::at(_MyType::const_pointer ptr)
{
  ptr = m_container[ptr];

  if (ptr == nullptr)
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  return *const_cast<_MyType::pointer>(ptr);
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle*
NativeTypesPool::create()
{
  auto ptr = m_container.create();

  if (ptr == nullptr)
  {
    THROW(NativeTypeHandleInsertionError(
      "insufficient memory to store native type handle"));
  }

  return ptr;
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle*
NativeTypesPool::create(const types::NativeTypeHandle& hndl)
{
  auto ptr = m_container.create(hndl);

  if (ptr == nullptr)
  {
    THROW(NativeTypeHandleInsertionError(
      "insufficient memory to store native type handle"));
  }

  return ptr;
}

// -----------------------------------------------------------------------------

void
NativeTypesPool::erase(_MyType::pointer ptr)
{
  ptr = m_container[ptr];

  if (ptr == nullptr)
  {
    THROW(NativeTypeHandleNotFoundError());
  }

  m_container.destroy(ptr);
}

// -----------------------------------------------------------------------------

std::ostream&
operator<<(std::ostream& ost, const NativeTypesPool& pool)
{
  ost << "Native types pool: ";
  ost << pool.size() << "/" << pool.max_size();
  ost << " (" << pool.total_size() << " bytes)";
  ost << std::endl << std::endl;

  ost << "-- BEGIN --" << std::endl;
  ost << std::endl;
  ost << pool.m_container;
  ost << std::endl;
  ost << "-- END --" << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
