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
#ifndef COREVM_RUNTIME_ERRORS_H_
#define COREVM_RUNTIME_ERRORS_H_

#include "common.h"
#include "corevm/errors.h"
#include "dyobj/common.h"

#include <boost/format.hpp>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

class InvalidOperationError : public RuntimeError
{
public:
  explicit InvalidOperationError(const char* what_arg)
    :
    RuntimeError(
      str(boost::format("Invalid operation: %s") % what_arg))
  {
  }
};

// -----------------------------------------------------------------------------

class FrameNotFoundError : public RuntimeError
{
public:
  FrameNotFoundError()
    :
    RuntimeError("Expected frame not found")
  {
  }
};

// -----------------------------------------------------------------------------

class InvocationCtxNotFoundError : public RuntimeError
{
public:
  InvocationCtxNotFoundError()
    :
    RuntimeError("Invocation context not found")
  {
  }
};

// -----------------------------------------------------------------------------

class EvaluationStackEmptyError : public RuntimeError
{
public:
  EvaluationStackEmptyError()
    :
    RuntimeError("Evaluation stack is empty")
  {
  }
};

// -----------------------------------------------------------------------------

class NameNotFoundError : public RuntimeError
{
public:
  NameNotFoundError(const char* name)
    :
    RuntimeError(str(boost::format("Local variable '%s' not found") % name))
  {
  }
};

// -----------------------------------------------------------------------------

class ObjectStackEmptyError : public RuntimeError
{
public:
  ObjectStackEmptyError()
    :
    RuntimeError("Process's object stack is empty")
  {
  }
};

// -----------------------------------------------------------------------------

class NativeTypeHandleNotFoundError : public RuntimeError
{
public:
  NativeTypeHandleNotFoundError()
    :
    RuntimeError("Native type handle not found")
  {
  }
};

// -----------------------------------------------------------------------------

class ObjectDeletionError : public RuntimeError
{
public:
  explicit ObjectDeletionError(dyobj::dyobj_id_t id)
    :
    RuntimeError(
      str(boost::format("Cannot delete object %#x") % id))
  {
  }
};

// -----------------------------------------------------------------------------

class InvocationError : public RuntimeError
{
public:
  explicit InvocationError(dyobj::dyobj_id_t id)
    :
    RuntimeError(str(boost::format("Cannot invoke call on object %#x") % id))
  {
  }
};

// -----------------------------------------------------------------------------

class MissingParameterError : public RuntimeError
{
public:
  MissingParameterError()
    :
    RuntimeError("Missing parameter")
  {
  }
};

// -----------------------------------------------------------------------------

class InvalidInstrAddrError : public RuntimeError
{
public:
  InvalidInstrAddrError()
    :
    RuntimeError("Invalid instruction address")
  {
  }
};

// -----------------------------------------------------------------------------

class NativeTypeHandleInsertionError : public RuntimeError
{
public:
  explicit NativeTypeHandleInsertionError(const char* what_arg)
    :
    RuntimeError(str(boost::format(
      "Cannot insert native type handle: %s") % what_arg))
  {
  }
};

// -----------------------------------------------------------------------------

class StringLiteralNotFoundError : public RuntimeError
{
public:
  explicit StringLiteralNotFoundError(encoding_key_t key)
    :
    RuntimeError(str(boost::format(
      "Cannot find string literal for key: %llu") % key))
  {
  }
};

// -----------------------------------------------------------------------------

class FptLiteralNotFoundError : public RuntimeError
{
public:
  explicit FptLiteralNotFoundError(encoding_key_t key)
    :
    RuntimeError(str(boost::format(
      "Cannot find floating-point literal for key: %llu") % key))
  {
  }
};

// -----------------------------------------------------------------------------

class CompartmentNotFoundError : public RuntimeError
{
public:
  explicit CompartmentNotFoundError(compartment_id_t id)
    :
    RuntimeError(str(boost::format("Cannot find compartment %llu") % id))
  {
  }
};

// -----------------------------------------------------------------------------

class ClosureNotFoundError : public RuntimeError
{
public:
  explicit ClosureNotFoundError(closure_id_t id)
    :
    RuntimeError(str(boost::format("Cannot find closure %lld") % id))
  {
  }
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_ERRORS_H_ */
