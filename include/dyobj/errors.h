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
#ifndef COREVM_DYOBJ_ERRORS_H_
#define COREVM_DYOBJ_ERRORS_H_

#include "common.h"
#include "dyobj_id.h"
#include "dyobj_id_helper.h"
#include "../errors.h"

#include <boost/format.hpp>

#include <string>


namespace corevm {


namespace dyobj {


class runtime_error : public corevm::runtime_error {
public:
  explicit runtime_error(const std::string& what_arg):
    corevm::runtime_error(what_arg)
  {
  }

  explicit runtime_error(const char* what_arg):
    corevm::runtime_error(what_arg)
  {
  }
};


class object_not_found_error : public corevm::dyobj::runtime_error {
public:
  explicit object_not_found_error(const std::string& what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit object_not_found_error(const char* what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit object_not_found_error(corevm::dyobj::dyobj_id id):
    corevm::dyobj::runtime_error(
      str(boost::format("Object %s not found") % corevm::dyobj::dyobj_id_helper::id_to_string(id))
    ),
    id(id)
  {
  }

  corevm::dyobj::dyobj_id id;
};


class object_attribute_not_found_error : public corevm::dyobj::runtime_error {
public:
  explicit object_attribute_not_found_error(const std::string& what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit object_attribute_not_found_error(const char* what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit object_attribute_not_found_error(
    corevm::dyobj::attr_key attr_key, corevm::dyobj::dyobj_id id
  ):
    corevm::dyobj::runtime_error(
      str(boost::format("Attribute %u in object %s not found") % \
        attr_key % corevm::dyobj::dyobj_id_helper::id_to_string(id))
    ),
    attr_key(attr_key),
    id(id)
  {
  }

  corevm::dyobj::attr_key attr_key;
  corevm::dyobj::dyobj_id id;
};


class object_attribute_deletion_error : public corevm::dyobj::runtime_error {
public:
  explicit object_attribute_deletion_error(const std::string& what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit object_attribute_deletion_error(const char* what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit object_attribute_deletion_error(
    corevm::dyobj::attr_key attr_key, corevm::dyobj::dyobj_id id
  ):
    corevm::dyobj::runtime_error(
      str(
        boost::format("Attribute %u cannot be deleted from object %s") % \
          attr_key % corevm::dyobj::dyobj_id_helper::id_to_string(id)
      )
    ),
    attr_key(attr_key),
    id(id)
  {
  }

  corevm::dyobj::attr_key attr_key;
  corevm::dyobj::dyobj_id id;
};


class object_id_exceed_limit_error : public corevm::dyobj::runtime_error {
public:
  explicit object_id_exceed_limit_error():
    corevm::dyobj::runtime_error("The maximum limit of dynamic object ID has been exceeded")
  {
  }
};


class object_heap_insertion_failed_error : public corevm::dyobj::runtime_error {
public:
  explicit object_heap_insertion_failed_error():
    corevm::dyobj::runtime_error("Dynamic object has failed to be inserted into the heap")
  {
  }
};


class invalid_flag_bit_error : public corevm::dyobj::runtime_error {
public:
  explicit invalid_flag_bit_error():
    corevm::dyobj::runtime_error("Invalid flag bit provided")
  {
  }
};


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ERRORS_H_ */
