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
#ifndef COREVM_RUNTIME_UTILS_H_
#define COREVM_RUNTIME_UTILS_H_

#include "corevm/macros.h"
#include "dyobj/common.h"
#include "dyobj/errors.h"
#include "dyobj/util.h"
#include "common.h"

#include <cstdint>
#include <string>


namespace corevm {


namespace runtime {


/** Forward declaration of `Compartment`. */
class Compartment;


using dyobj::ntvhndl_key;

// -----------------------------------------------------------------------------

inline ntvhndl_key ptr_to_ntvhndl_key(void* ptr)
{
  return static_cast<ntvhndl_key>( (uint8_t*)(ptr) - (uint8_t*)(NULL) );
}

// -----------------------------------------------------------------------------

inline void* ntvhndl_key_to_ptr(const ntvhndl_key& key)
{
  return reinterpret_cast<void*>(key);
}

// -----------------------------------------------------------------------------

dyobj::attr_key
get_attr_key(
  Compartment* compartment,
  encoding_key str_key,
  std::string* attr_str);

// -----------------------------------------------------------------------------

dyobj::attr_key
get_attr_key(
  Compartment* compartment,
  encoding_key str_key);

// -----------------------------------------------------------------------------

template<typename ObjType>
dyobj::dyobj_id
getattr(const ObjType& obj, dyobj::attr_key attr_key,
  const std::string& attr_name)
{
  dyobj::dyobj_id attr_id = 0;

  if (!obj.getattr(attr_key, &attr_id))
  {
    THROW(dyobj::ObjectAttributeNotFoundError(
      attr_key, obj.id(), attr_name));
  }

  return attr_id;
}

// -----------------------------------------------------------------------------

template<typename ObjType>
dyobj::dyobj_id
getattr(const ObjType& obj, const std::string& attr_name)
{
  dyobj::attr_key attr_key = dyobj::hash_attr_str(attr_name);

  return getattr(obj, attr_key, attr_name);
}

// -----------------------------------------------------------------------------

template<typename ObjType>
dyobj::dyobj_id
getattr(const ObjType& obj,
  Compartment* compartment,
  encoding_key attr_encoding_key)
{
  std::string attr_name;
  dyobj::attr_key attr_key = get_attr_key(
    compartment, attr_encoding_key, &attr_name);

  return getattr(obj, attr_key, attr_name);
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_UTILS_H_ */
