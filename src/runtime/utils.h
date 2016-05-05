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


// -----------------------------------------------------------------------------

dyobj::attr_key_t
get_attr_key(Compartment* compartment, encoding_key_t str_key,
  std::string* attr_str);

// -----------------------------------------------------------------------------

dyobj::attr_key_t
get_attr_key(Compartment* compartment, encoding_key_t str_key);

// -----------------------------------------------------------------------------

template<typename ObjPtrType>
ObjPtrType
getattr(const ObjPtrType& obj, dyobj::attr_key_t attr_key,
  const std::string& attr_name)
{
  ObjPtrType attr_ptr = NULL;

  if (!obj->getattr(attr_key, &attr_ptr))
  {
    THROW(dyobj::ObjectAttributeNotFoundError(
      attr_key, obj->id(), attr_name.c_str()));
  }

  return attr_ptr;
}

// -----------------------------------------------------------------------------

template<typename ObjPtrType>
ObjPtrType
getattr(const ObjPtrType& obj, const std::string& attr_name)
{
  dyobj::attr_key_t attr_key = dyobj::hash_attr_str(attr_name);

  return getattr(obj, attr_key, attr_name);
}

// -----------------------------------------------------------------------------

template<typename ObjPtrType>
ObjPtrType
getattr(const ObjPtrType& obj, Compartment* compartment,
  encoding_key_t attr_encoding_key)
{
  std::string attr_name;
  dyobj::attr_key_t attr_key = get_attr_key(
    compartment, attr_encoding_key, &attr_name);

  return getattr(obj, attr_key, attr_name);
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_UTILS_H_ */
