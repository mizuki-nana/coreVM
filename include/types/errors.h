#ifndef COREVM_TYPES_ERRORS_H_
#define COREVM_TYPES_ERRORS_H_

#include <string>
#include <boost/format.hpp>
#include "../errors.h"


namespace corevm {


namespace types {


class corevm_native_type_error : public corevm::errors::corevm_runtime_error {
public:
  explicit corevm_native_type_error(const std::string& what_arg): corevm::errors::corevm_runtime_error(what_arg) {}
  explicit corevm_native_type_error(const char* what_arg): corevm::errors::corevm_runtime_error(what_arg) {}
};


class corevm_native_type_conversion_error : public corevm_native_type_error {
public:
  explicit corevm_native_type_conversion_error(const std::string& src_type, const std::string& dst_type):
    corevm::types::corevm_native_type_error(
      str(boost::format("Cannot convert between native types %s to %s") % src_type % dst_type)
    ),
    src_type(src_type),
    dst_type(dst_type)
  {
  }

  explicit corevm_native_type_conversion_error(const char* src_type, const char* dst_type):
    corevm::types::corevm_native_type_error(
      str(boost::format("Cannot convert between native types %s to %s") % src_type % dst_type)
    ),
    src_type(src_type),
    dst_type(dst_type)
  {
  }

  const std::string src_type;
  const std::string dst_type;
};


class corevm_native_type_invalid_operator_error : public corevm_native_type_error {
public:
  explicit corevm_native_type_invalid_operator_error(const std::string& op, const std::string& type):
    corevm::types::corevm_native_type_error(
      str(boost::format("Cannot perform operator %s on native type %s") % op % type)
    ),
    op(op),
    type(type)
  {
  }

  explicit corevm_native_type_invalid_operator_error(const char* op, const char* type):
    corevm::types::corevm_native_type_error(
      str(boost::format("Cannot perform operator %s on native type %s") % op % type)
    ),
    op(op),
    type(type)
  {
  }

  const std::string op;
  const std::string type;
};


class corevm_native_type_out_of_range_error : public corevm_native_type_error {
public:
  explicit corevm_native_type_out_of_range_error(const std::string& what_arg): corevm::types::corevm_native_type_error(what_arg) {}
  explicit corevm_native_type_out_of_range_error(const char* what_arg): corevm::types::corevm_native_type_error(what_arg) {}
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_ERRORS_H_ */
