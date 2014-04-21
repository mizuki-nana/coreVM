#ifndef COREVM_DYOBJ_ERRORS_H_
#define COREVM_DYOBJ_ERRORS_H_

#include <string>
#include <boost/format.hpp>
#include "common.h"
#include "dyobj_id.h"
#include "../errors.h"


namespace corevm {


namespace dyobj {


class dynamic_object_runtime_error : public corevm::errors::corevm_runtime_error {
public:
  explicit dynamic_object_runtime_error(const std::string& what_arg):
    corevm::errors::corevm_runtime_error(what_arg) {}
  explicit dynamic_object_runtime_error(const char* what_arg):
    corevm::errors::corevm_runtime_error(what_arg) {}
};


class dynamic_object_not_found_error : public corevm::dyobj::dynamic_object_runtime_error {
public:
  explicit dynamic_object_not_found_error(const std::string& what_arg):
    corevm::dyobj::dynamic_object_runtime_error(what_arg) {}
  explicit dynamic_object_not_found_error(const char* what_arg):
    corevm::dyobj::dynamic_object_runtime_error(what_arg) {}
  explicit dynamic_object_not_found_error(corevm::dyobj::dyobj_id id):
    corevm::dyobj::dynamic_object_runtime_error(
      str(boost::format("Object %s not found") % corevm::dyobj::id_to_string(id))
    ),
    id(id)
  {
  }

  corevm::dyobj::dyobj_id id;
};


class dynamic_object_attribute_not_found_error : public corevm::dyobj::dynamic_object_runtime_error {
public:
  explicit dynamic_object_attribute_not_found_error(const std::string& what_arg):
    corevm::dyobj::dynamic_object_runtime_error(what_arg) {}
  explicit dynamic_object_attribute_not_found_error(const char* what_arg):
    corevm::dyobj::dynamic_object_runtime_error(what_arg) {}
  explicit dynamic_object_attribute_not_found_error(corevm::dyobj::attr_key attr_key, corevm::dyobj::dyobj_id id):
    corevm::dyobj::dynamic_object_runtime_error(
      str(boost::format("Attribute %u in object %s not found") % attr_key % corevm::dyobj::id_to_string(id))
    ),
    attr_key(attr_key),
    id(id)
  {
  }

  corevm::dyobj::attr_key attr_key;
  corevm::dyobj::dyobj_id id;
};


class dynamic_object_attribute_cannot_be_deleted_error : public corevm::dyobj::dynamic_object_runtime_error {
public:
  explicit dynamic_object_attribute_cannot_be_deleted_error(const std::string& what_arg):
    corevm::dyobj::dynamic_object_runtime_error(what_arg) {}
  explicit dynamic_object_attribute_cannot_be_deleted_error(const char* what_arg):
    corevm::dyobj::dynamic_object_runtime_error(what_arg) {}
  explicit dynamic_object_attribute_cannot_be_deleted_error(corevm::dyobj::attr_key attr_key, corevm::dyobj::dyobj_id id):
    corevm::dyobj::dynamic_object_runtime_error(
      str(boost::format("Attribute %u cannot be deleted from object %s") % attr_key % corevm::dyobj::id_to_string(id))
    ),
    attr_key(attr_key),
    id(id)
  {
  }

  corevm::dyobj::attr_key attr_key;
  corevm::dyobj::dyobj_id id;
};


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ERRORS_H_ */