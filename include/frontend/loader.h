#ifndef COREVM_FRONTEND_LOADER_H_
#define COREVM_FRONTEND_LOADER_H_

#include <string>
#include <sneaker/json/json.h>
#include "bytecode_runner.h"
#include "errors.h"


namespace corevm {


namespace frontend {


using sneaker::json::JSON;


class loader {
public:
  explicit loader(const std::string&);

  const std::string path() const;

  bytecode_runner* load() throw(corevm::frontend::file_loading_error);
protected:
  void validate(const JSON&);

  std::string _path;
};


} /* end namespace frontend */


} /* end namespace corevm */


#endif /* COREVM_FRONTEND_LOADER_H_ */
