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
#ifndef COREVM_BYTECODE_LOADER_H_
#define COREVM_BYTECODE_LOADER_H_

#include "errors.h"
#include "../runtime/process.h"

#include <sneaker/json/json.h>

#include <string>


namespace corevm {


namespace frontend {


using sneaker::json::JSON;

// -----------------------------------------------------------------------------

class bytecode_loader
{
public:
  virtual void load(
    const std::string&, const JSON&, corevm::runtime::process&) = 0;

  virtual const std::string format() const = 0;
  virtual const std::string version() const = 0;
  virtual const std::string schema() const = 0;

  static void load(const std::string&, corevm::runtime::process&)
    throw(corevm::frontend::file_loading_error);

private:
  static void validate_and_load(
    const std::string&, const JSON&, corevm::runtime::process&);
};

// -----------------------------------------------------------------------------


} /* end namespace frontend */


} /* end namespace corevm */


#endif /* COREVM_BYTECODE_LOADER_H_ */
