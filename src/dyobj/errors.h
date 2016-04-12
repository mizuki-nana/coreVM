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
#ifndef COREVM_DYOBJ_ERRORS_H_
#define COREVM_DYOBJ_ERRORS_H_

#include "common.h"
#include "corevm/errors.h"

#include <boost/format.hpp>

#include <string>


namespace corevm {


namespace dyobj {


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

class ObjectNotFoundError : public dyobj::RuntimeError
{
public:
  explicit ObjectNotFoundError(const std::string& what_arg)
    :
    dyobj::RuntimeError(what_arg)
  {
  }

  explicit ObjectNotFoundError(const char* what_arg)
    :
    dyobj::RuntimeError(what_arg)
  {
  }

  explicit ObjectNotFoundError(corevm::dyobj::dyobj_id_t id_)
    :
    dyobj::RuntimeError(str(boost::format("Object %#x not found") % id_)),
    id(id_)
  {
  }

  corevm::dyobj::dyobj_id_t id;
};

// -----------------------------------------------------------------------------

class ObjectAttributeNotFoundError : public dyobj::RuntimeError
{
public:
  explicit ObjectAttributeNotFoundError(const std::string& what_arg)
    :
    dyobj::RuntimeError(what_arg)
  {
  }

  explicit ObjectAttributeNotFoundError(const char* what_arg)
    :
    dyobj::RuntimeError(what_arg)
  {
  }

  ObjectAttributeNotFoundError(attr_key_t attr_key_, dyobj_id_t id_)
    :
    dyobj::RuntimeError(
      str(boost::format(
        "Attribute %#x in object %#x not found") % attr_key_ % id_)),
    attr_key(attr_key_),
    id(id_)
  {
  }

  ObjectAttributeNotFoundError(attr_key_t attr_key_,
    dyobj_id_t id_, const std::string& attr_name)
    :
    dyobj::RuntimeError(
      str(boost::format(
        "Attribute '%s' in object %#x not found") % attr_name % id_)),
    attr_key(attr_key_),
    id(id_)
  {
  }

  attr_key_t attr_key;
  dyobj_id_t id;
};

// -----------------------------------------------------------------------------

class ObjectCreationError : public dyobj::RuntimeError
{
public:
  explicit ObjectCreationError()
    :
    dyobj::RuntimeError("Failed to create dynamic object")
  {
  }
};

// -----------------------------------------------------------------------------

class InvalidFlagBitError : public dyobj::RuntimeError
{
public:
  explicit InvalidFlagBitError()
    :
    dyobj::RuntimeError("Invalid flag bit provided")
  {
  }
};

// -----------------------------------------------------------------------------


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ERRORS_H_ */
