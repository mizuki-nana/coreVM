#ifndef COREVM_RUNTIME_ERRORS_H_
#define COREVM_RUNTIME_ERRORS_H_

#include "../errors.h"


namespace corevm {


namespace runtime {


class runtime_error : public corevm::errors::corevm_runtime_error {
public:
  explicit runtime_error(const std::string& what_arg):
    corevm::errors::corevm_runtime_error(what_arg) {}
  explicit runtime_error(const char* what_arg):
    corevm::errors::corevm_runtime_error(what_arg) {}
};


class evaluation_stack_empty_error : public corevm::runtime::runtime_error {
public:
  explicit evaluation_stack_empty_error():
    corevm::runtime::runtime_error("Evaluation stack is empty")
  {}
};


class evaluation_stack_not_empty_error : public corevm::runtime::runtime_error {
public:
  explicit evaluation_stack_not_empty_error():
    corevm::runtime::runtime_error("Evaluation stack is not empty")
  {}
};


class local_variable_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit local_variable_not_found_error():
    corevm::runtime::runtime_error("Local variable not found")
  {}
};


class invalid_instr_error : public corevm::runtime::runtime_error {
public:
  explicit invalid_instr_error():
    corevm::runtime::runtime_error("Invalid instruction code encountered")
  {}
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_ERRORS_H_ */
