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
#ifndef COREVM_DYOBJ_ERRORS_H_
#define COREVM_DYOBJ_ERRORS_H_

#include "common.h"
#include "dyobj_id.h"
#include "corevm/errors.h"

#include <boost/format.hpp>

#include <string>


namespace corevm {


namespace dyobj {


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

class ObjectNotFoundError : public corevm::dyobj::runtime_error
{
public:
  explicit ObjectNotFoundError(const std::string& what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit ObjectNotFoundError(const char* what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit ObjectNotFoundError(corevm::dyobj::dyobj_id id_):
    corevm::dyobj::runtime_error(
      str(boost::format("Object %#x not found") % id_)
    ),
    id(id_)
  {
  }

  corevm::dyobj::dyobj_id id;
};

// -----------------------------------------------------------------------------

class ObjectAttributeNotFoundError : public corevm::dyobj::runtime_error
{
public:
  explicit ObjectAttributeNotFoundError(const std::string& what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  explicit ObjectAttributeNotFoundError(const char* what_arg):
    corevm::dyobj::runtime_error(what_arg)
  {
  }

  ObjectAttributeNotFoundError(
    attr_key attr_key_, dyobj_id id_)
    :
    corevm::dyobj::runtime_error(
      str(boost::format(
        "Attribute %#x in object %#x not found") % attr_key_ % id_)),
    attr_key(attr_key_),
    id(id_)
  {
  }

  ObjectAttributeNotFoundError(attr_key attr_key_,
    dyobj_id id_, const std::string& attr_name)
    :
    corevm::dyobj::runtime_error(
      str(boost::format(
        "Attribute '%s' in object %#x not found") % attr_name % id_)),
    attr_key(attr_key_),
    id(id_)
  {
  }

  attr_key attr_key;
  dyobj_id id;
};

// -----------------------------------------------------------------------------

class ObjectCreationError : public corevm::dyobj::runtime_error
{
public:
  explicit ObjectCreationError():
    corevm::dyobj::runtime_error("Failed to create dynamic object")
  {
  }
};

// -----------------------------------------------------------------------------

class InvalidFlagBitError : public corevm::dyobj::runtime_error
{
public:
  explicit InvalidFlagBitError():
    corevm::dyobj::runtime_error("Invalid flag bit provided")
  {
  }
};

// -----------------------------------------------------------------------------


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ERRORS_H_ */
