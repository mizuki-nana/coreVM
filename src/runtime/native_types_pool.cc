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

#include "../../include/runtime/native_types_pool.h"

#include "../../include/runtime/utils.h"


typedef corevm::runtime::native_types_pool _MyType;


_MyType::size_type
corevm::runtime::native_types_pool::size() const
{
  return m_container.size();
}

_MyType::size_type
corevm::runtime::native_types_pool::max_size() const
{
  return m_container.max_size();
}

_MyType::reference
corevm::runtime::native_types_pool::at(const corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  void* raw_ptr = corevm::runtime::ntvhndl_key_to_ptr(key);
  _MyType::pointer ptr = static_cast<_MyType::pointer>(raw_ptr);

  try {
    ptr = m_container[ptr];
  } catch(const corevm::memory::invalid_address_error&) {
    throw corevm::runtime::native_type_handle_not_found_error();
  }

  return *ptr;
}

corevm::dyobj::ntvhndl_key
corevm::runtime::native_types_pool::create()
{
  auto ptr = m_container.create();

  if (ptr == nullptr) {
    throw corevm::runtime::native_type_handle_insertion_error(
      "insufficient memory to store native type handle"
    );
  }

  return corevm::runtime::ptr_to_ntvhndl_key(ptr);
}

void
corevm::runtime::native_types_pool::erase(const corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  void* raw_ptr = corevm::runtime::ntvhndl_key_to_ptr(key);
  _MyType::pointer ptr = static_cast<_MyType::pointer>(raw_ptr);

  try {
    m_container.destroy(ptr);
  } catch(const corevm::memory::invalid_address_error&) {
    throw corevm::runtime::native_type_handle_not_found_error();
  }
}
