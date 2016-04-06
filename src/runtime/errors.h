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
#ifndef RUNTIME_ERRORS_H_
#define RUNTIME_ERRORS_H_

#include "common.h"
#include "corevm/errors.h"
#include "dyobj/dyobj_id.h"

#include <boost/format.hpp>

#include <string>


using boost::format;


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

class RuntimeError : public corevm::RuntimeError
{
public:
  explicit RuntimeError(const std::string& what_arg):
    corevm::RuntimeError(what_arg)
  {
  }

  explicit RuntimeError(const char* what_arg):
    corevm::RuntimeError(what_arg)
  {
  }
};

// -----------------------------------------------------------------------------

class InvalidOperationError : public corevm::runtime::RuntimeError
{
public:
  explicit InvalidOperationError(const std::string& what_arg):
    corevm::runtime::RuntimeError(
      str(format("Invalid operation: %s") % what_arg)
    )
  {
  }

  explicit InvalidOperationError(const char* what_arg):
    corevm::runtime::RuntimeError(
      str(format("Invalid operation: %s") % what_arg)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class FrameNotFoundError : public corevm::runtime::RuntimeError
{
public:
  explicit FrameNotFoundError():
    corevm::runtime::RuntimeError("Expected frame not found")
  {
  }
};

// -----------------------------------------------------------------------------

class InvocationCtxNotFoundError : public corevm::runtime::RuntimeError
{
public:
  InvocationCtxNotFoundError():
    corevm::runtime::RuntimeError("Invocation context not found")
  {
  }
};

// -----------------------------------------------------------------------------

class EvaluationStackEmptyError : public corevm::runtime::RuntimeError
{
public:
  explicit EvaluationStackEmptyError():
    corevm::runtime::RuntimeError("Evaluation stack is empty")
  {
  }
};

// -----------------------------------------------------------------------------

class NameNotFoundError : public corevm::runtime::RuntimeError
{
public:
  explicit NameNotFoundError():
    corevm::runtime::RuntimeError("Local variable not found")
  {
  }
};

// -----------------------------------------------------------------------------

class ObjectStackEmptyError : public corevm::runtime::RuntimeError
{
public:
  explicit ObjectStackEmptyError():
    corevm::runtime::RuntimeError("Process's object stack is empty")
  {
  }
};

// -----------------------------------------------------------------------------

class NativeTypeHandleNotFoundError : public corevm::runtime::RuntimeError
{
public:
  explicit NativeTypeHandleNotFoundError():
    corevm::runtime::RuntimeError("Native type handle not found")
  {
  }
};

// -----------------------------------------------------------------------------

class NativeTypeHandleDeletionError : public corevm::runtime::RuntimeError
{
public:
  explicit NativeTypeHandleDeletionError():
    corevm::runtime::RuntimeError("Native type handle cannot be deleted")
  {
  }
};

// -----------------------------------------------------------------------------

class ObjectDeletionError : public corevm::runtime::RuntimeError
{
public:
  explicit ObjectDeletionError(const dyobj::dyobj_id id):
    corevm::runtime::RuntimeError(
      str(format("Cannot delete object %#x") % id))
  {
  }
};

// -----------------------------------------------------------------------------

class InvocationError : public corevm::runtime::RuntimeError
{
public:
  explicit InvocationError(const dyobj::dyobj_id id):
    corevm::runtime::RuntimeError(
      str(format("Cannot invoke call on object %#x") % id))
  {
  }
};

// -----------------------------------------------------------------------------

class MissingParameterError : public corevm::runtime::RuntimeError
{
public:
  explicit MissingParameterError():
    corevm::runtime::RuntimeError("Missing parameter")
  {
  }
};

// -----------------------------------------------------------------------------

class InvalidInstrAddrError : public corevm::runtime::RuntimeError
{
public:
  explicit InvalidInstrAddrError():
    corevm::runtime::RuntimeError("Invalid instruction address")
  {
  }
};

// -----------------------------------------------------------------------------

class NativeTypeHandleInsertionError : public corevm::runtime::RuntimeError
{
public:
  explicit NativeTypeHandleInsertionError(const char* what_arg):
    corevm::runtime::RuntimeError(
      str(boost::format("Cannot insert native type handle: %s") % what_arg)
    )
  {
  }

  explicit NativeTypeHandleInsertionError(const std::string& what_arg):
    corevm::runtime::RuntimeError(
      str(boost::format("Cannot insert native type handle: %s") % what_arg)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class EncodingStringNotFoundError : public corevm::runtime::RuntimeError
{
public:
  explicit EncodingStringNotFoundError(encoding_key key):
    corevm::runtime::RuntimeError(
      str(boost::format("Cannot find encoding string for key: %llu") % key)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class CompartmentNotFoundError : public corevm::runtime::RuntimeError
{
public:
  explicit CompartmentNotFoundError(compartment_id id):
    corevm::runtime::RuntimeError(
      str(boost::format("Cannot find compartment %llu") % id)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class ClosureNotFoundError : public corevm::runtime::RuntimeError
{
public:
  explicit ClosureNotFoundError(closure_id id):
    corevm::runtime::RuntimeError(
      str(boost::format("Cannot find closure %lld") % id)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class SignalError : public corevm::runtime::RuntimeError
{
public:
  explicit SignalError(const char* what_arg):
    corevm::runtime::RuntimeError(what_arg)
  {
  }

  explicit SignalError(const std::string& what_arg):
    corevm::runtime::RuntimeError(what_arg)
  {
  }
};

// -----------------------------------------------------------------------------

class ExecutionSignalError : public corevm::runtime::SignalError
{
public:
  explicit ExecutionSignalError(const char* signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled arithmetic/execution signal %s") % signal)
    )
  {
  }

  explicit ExecutionSignalError(const std::string& signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled arithmetic/execution signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class TerminationSignalError : public corevm::runtime::SignalError
{
public:
  explicit TerminationSignalError(const char* signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled termination signal %s") % signal)
    )
  {
  }

  explicit TerminationSignalError(const  std::string& signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled termination signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class OperationSignalError : public corevm::runtime::SignalError
{
public:
  explicit OperationSignalError(const char* signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled operation signal %s") % signal)
    )
  {
  }

  explicit OperationSignalError(const std::string& signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled operation signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class IoSignalError : public corevm::runtime::SignalError
{
public:
  explicit IoSignalError(const char* signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled IO signal %s") % signal)
    )
  {
  }

  explicit IoSignalError(const std::string& signal):
    corevm::runtime::SignalError(
      str(format("Received unhandled IO signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

} /* end namespace runtime */


} /* end namespace corevm */


#endif /* RUNTIME_ERRORS_H_ */
