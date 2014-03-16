#ifndef COREVM_ERRORS_H_
#define COREVM_ERRORS_H_


#include <stdexcept>
#include <string>


namespace corevm {


namespace errors {


class corevm_runtime_error : public std::runtime_error {
public:
  explicit corevm_runtime_error(const std::string& what_arg): std::runtime_error(what_arg) {}
  explicit corevm_runtime_error(const char* what_arg): std::runtime_error(what_arg) {}
};


class corevm_logic_error : public std::logic_error {
public:
  explicit corevm_logic_error(const std::string& what_arg): std::logic_error(what_arg) {}
  explicit corevm_logic_error(const char* what_arg): std::logic_error(what_arg) {}
};


} /* end namespace errors */


} /* end namespace corevm */


#endif /* COREVM_ERRORS_H_ */
