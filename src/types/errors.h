/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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

#include "corevm/errors.h"

#include <boost/format.hpp>

#include <string>


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

class RuntimeError : public corevm::RuntimeError
{
public:
  explicit RuntimeError(const std::string& what_arg)
    :
    corevm::RuntimeError(what_arg)
  {
  }

  explicit RuntimeError(const char* what_arg)
    :
    corevm::RuntimeError(what_arg)
  {
  }
};

// -----------------------------------------------------------------------------

class ConversionError : public corevm::types::RuntimeError
{
public:
  explicit ConversionError(const std::string& src_type_,
    const std::string& dst_type_)
    :
    corevm::types::RuntimeError(
      str(boost::format("Cannot convert between native types %s to %s") % src_type_ % dst_type_)),
    src_type(src_type_),
    dst_type(dst_type_)
  {
  }

  explicit ConversionError(const char* src_type_, const char* dst_type_)
    :
    corevm::types::RuntimeError(
      str(boost::format("Cannot convert between native types %s to %s") % src_type_ % dst_type_)),
    src_type(src_type_),
    dst_type(dst_type_)
  {
  }

  const std::string src_type;
  const std::string dst_type;
};

// -----------------------------------------------------------------------------

class InvalidOperatorError : public corevm::types::RuntimeError
{
public:
  explicit InvalidOperatorError(const std::string& op_, const std::string& type_)
    :
    corevm::types::RuntimeError(
      str(boost::format("Cannot perform operator %s on native type %s") % op_ % type_)),
    op(op_),
    type(type_)
  {
  }

  explicit InvalidOperatorError(const char* op_, const char* type_)
    :
    corevm::types::RuntimeError(
      str(boost::format("Cannot perform operator %s on native type %s") % op_ % type_)),
    op(op_),
    type(type_)
  {
  }

  const std::string op;
  const std::string type;
};

// -----------------------------------------------------------------------------

class OutOfRangeError : public corevm::types::RuntimeError
{
public:
  explicit OutOfRangeError(const std::string& what_arg)
    :
    types::RuntimeError(what_arg)
  {
  }

  explicit OutOfRangeError(const char* what_arg)
    :
    types::RuntimeError(what_arg)
  {
  }
};

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_ERRORS_H_ */
