#ifndef COREVM_DYOBJ_ERRORS_H_
#define COREVM_DYOBJ_ERRORS_H_

#include <string>
#include <boost/format.hpp>
#include "common.h"
#include "dyobj_id.h"
#include "dyobj_id_helper.h"
#include "../errors.h"


namespace corevm {


namespace dyobj {


class runtime_error : public corevm::errors::corevm_runtime_error {
public:
  explicit runtime_error(const std::string& what_arg):
    corevm::errors::corevm_runtime_error(what_arg)
  {
  }

  explicit runtime_error(const char* what_arg):
    corevm::errors::corevm_runtime_error(what_arg)
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
