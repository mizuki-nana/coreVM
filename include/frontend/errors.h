#ifndef COREVM_FRONTEND_ERRORS_H_
#define COREVM_FRONTEND_ERRORS_H_

#include <string>
#include <boost/format.hpp>
#include "../errors.h"


namespace corevm {


namespace frontend {


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


class file_loading_error : public corevm::frontend::runtime_error {
public:
  explicit file_loading_error(const std::string& what_arg):
    corevm::frontend::runtime_error(what_arg)
  {
  }

  explicit file_loading_error(const char* what_arg):
    corevm::frontend::runtime_error(what_arg)
  {
  }
};


}; /* end namespace frontend */


} /* end namespace corevm */


#endif /* COREVM_FRONTEND_ERRORS_H_ */
