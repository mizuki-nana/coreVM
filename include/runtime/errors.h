#ifndef COREVM_RUNTIME_ERRORS_H_
#define COREVM_RUNTIME_ERRORS_H_

#include "../errors.h"


namespace corevm {


namespace runtime {


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


class frame_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit frame_not_found_error():
    corevm::runtime::runtime_error("Expected frame not found")
  {
  }
};


class evaluation_stack_empty_error : public corevm::runtime::runtime_error {
public:
  explicit evaluation_stack_empty_error():
    corevm::runtime::runtime_error("Evaluation stack is empty")
  {
  }
};


class evaluation_stack_not_empty_error : public corevm::runtime::runtime_error {
public:
  explicit evaluation_stack_not_empty_error():
    corevm::runtime::runtime_error("Evaluation stack is not empty")
  {
  }
};


class local_variable_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit local_variable_not_found_error():
    corevm::runtime::runtime_error("Local variable not found")
  {
  }
};


class invalid_instr_error : public corevm::runtime::runtime_error {
public:
  explicit invalid_instr_error():
    corevm::runtime::runtime_error("Invalid instruction code encountered")
  {
  }
};


class object_stack_empty_error : public corevm::runtime::runtime_error {
public:
  explicit object_stack_empty_error():
    corevm::runtime::runtime_error("Process's object stack is empty")
  {
  }
};


class native_type_handle_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit native_type_handle_not_found_error():
    corevm::runtime::runtime_error("Native type handle not found")
  {
  }
};


class native_type_handle_deletion_error : public corevm::runtime::runtime_error {
public:
  explicit native_type_handle_deletion_error():
    corevm::runtime::runtime_error("Native type handle cannot be deleted")
  {
  }
};


class missing_parameter_error : public corevm::runtime::runtime_error {
public:
  explicit missing_parameter_error():
    corevm::runtime::runtime_error("Missing parameter")
  {
  }
};


class invalid_instr_addr_error : public corevm::runtime::runtime_error {
public:
  explicit invalid_instr_addr_error():
    corevm::runtime::runtime_error("Invalid instruction address")
  {
  }
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_ERRORS_H_ */
