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
#ifndef COREVM_TYPES_ERRORS_H_
#define COREVM_TYPES_ERRORS_H_

#include <string>
#include <boost/format.hpp>
#include "../errors.h"


namespace corevm {


namespace types {


class runtime_error : public corevm::runtime_error
{
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

// -----------------------------------------------------------------------------

class conversion_error : public corevm::types::runtime_error
{
public:
  explicit conversion_error(const std::string& src_type, const std::string& dst_type):
    corevm::types::runtime_error(
      str(boost::format("Cannot convert between native types %s to %s") % src_type % dst_type)
    ),
    src_type(src_type),
    dst_type(dst_type)
  {
  }

  explicit conversion_error(const char* src_type, const char* dst_type):
    corevm::types::runtime_error(
      str(boost::format("Cannot convert between native types %s to %s") % src_type % dst_type)
    ),
    src_type(src_type),
    dst_type(dst_type)
  {
  }

  const std::string src_type;
  const std::string dst_type;
};

// -----------------------------------------------------------------------------

class invalid_operator_error : public corevm::types::runtime_error
{
public:
  explicit invalid_operator_error(const std::string& op, const std::string& type):
    corevm::types::runtime_error(
      str(boost::format("Cannot perform operator %s on native type %s") % op % type)
    ),
    op(op),
    type(type)
  {
  }

  explicit invalid_operator_error(const char* op, const char* type):
    corevm::types::runtime_error(
      str(boost::format("Cannot perform operator %s on native type %s") % op % type)
    ),
    op(op),
    type(type)
  {
  }

  const std::string op;
  const std::string type;
};

// -----------------------------------------------------------------------------

class out_of_range_error : public corevm::types::runtime_error
{
public:
  explicit out_of_range_error(const std::string& what_arg): corevm::types::runtime_error(what_arg)
  {
  }

  explicit out_of_range_error(const char* what_arg): corevm::types::runtime_error(what_arg)
  {
  }
};

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_ERRORS_H_ */
