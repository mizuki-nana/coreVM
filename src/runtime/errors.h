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

class invalid_operation_error : public corevm::runtime::runtime_error
{
public:
  explicit invalid_operation_error(const std::string& what_arg):
    corevm::runtime::runtime_error(
      str(format("Invalid operation: %s") % what_arg)
    )
  {
  }

  explicit invalid_operation_error(const char* what_arg):
    corevm::runtime::runtime_error(
      str(format("Invalid operation: %s") % what_arg)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class frame_not_found_error : public corevm::runtime::runtime_error
{
public:
  explicit frame_not_found_error():
    corevm::runtime::runtime_error("Expected frame not found")
  {
  }
};

// -----------------------------------------------------------------------------

class invocation_ctx_not_found_error : public corevm::runtime::runtime_error
{
public:
  invocation_ctx_not_found_error():
    corevm::runtime::runtime_error("Invocation context not found")
  {
  }
};

// -----------------------------------------------------------------------------

class evaluation_stack_empty_error : public corevm::runtime::runtime_error
{
public:
  explicit evaluation_stack_empty_error():
    corevm::runtime::runtime_error("Evaluation stack is empty")
  {
  }
};

// -----------------------------------------------------------------------------

class local_variable_not_found_error : public corevm::runtime::runtime_error
{
public:
  explicit local_variable_not_found_error():
    corevm::runtime::runtime_error("Local variable not found")
  {
  }
};

// -----------------------------------------------------------------------------

class invalid_instr_error : public corevm::runtime::runtime_error
{
public:
  explicit invalid_instr_error():
    corevm::runtime::runtime_error("Invalid instruction code encountered")
  {
  }
};

// -----------------------------------------------------------------------------

class object_stack_empty_error : public corevm::runtime::runtime_error
{
public:
  explicit object_stack_empty_error():
    corevm::runtime::runtime_error("Process's object stack is empty")
  {
  }
};

// -----------------------------------------------------------------------------

class native_type_handle_not_found_error : public corevm::runtime::runtime_error
{
public:
  explicit native_type_handle_not_found_error():
    corevm::runtime::runtime_error("Native type handle not found")
  {
  }
};

// -----------------------------------------------------------------------------

class native_type_handle_deletion_error : public corevm::runtime::runtime_error
{
public:
  explicit native_type_handle_deletion_error():
    corevm::runtime::runtime_error("Native type handle cannot be deleted")
  {
  }
};

// -----------------------------------------------------------------------------

class object_deletion_error : public corevm::runtime::runtime_error
{
public:
  explicit object_deletion_error(const corevm::dyobj::dyobj_id id):
    corevm::runtime::runtime_error(
      str(
        format("Cannot delete object %s") % corevm::dyobj::id_to_string(id)
      )
    )
  {
  }
};

// -----------------------------------------------------------------------------

class invocation_error : public corevm::runtime::runtime_error
{
public:
  explicit invocation_error(const corevm::dyobj::dyobj_id id):
    corevm::runtime::runtime_error(
      str(
        format("Cannot invoke call on object %s") % corevm::dyobj::id_to_string(id)
      )
    )
  {
  }
};

// -----------------------------------------------------------------------------

class missing_parameter_error : public corevm::runtime::runtime_error
{
public:
  explicit missing_parameter_error():
    corevm::runtime::runtime_error("Missing parameter")
  {
  }
};

// -----------------------------------------------------------------------------

class invalid_instr_addr_error : public corevm::runtime::runtime_error
{
public:
  explicit invalid_instr_addr_error():
    corevm::runtime::runtime_error("Invalid instruction address")
  {
  }
};

// -----------------------------------------------------------------------------

class native_type_handle_insertion_error : public corevm::runtime::runtime_error
{
public:
  explicit native_type_handle_insertion_error(const char* what_arg):
    corevm::runtime::runtime_error(
      str(boost::format("Cannot insert native type handle: %s") % what_arg)
    )
  {
  }

  explicit native_type_handle_insertion_error(const std::string& what_arg):
    corevm::runtime::runtime_error(
      str(boost::format("Cannot insert native type handle: %s") % what_arg)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class encoding_string_not_found_error : public corevm::runtime::runtime_error
{
public:
  explicit encoding_string_not_found_error(uint64_t key):
    corevm::runtime::runtime_error(
      str(boost::format("Cannot find encoding string for key: %llu") % key)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class compartment_not_found_error : public corevm::runtime::runtime_error
{
public:
  explicit compartment_not_found_error(corevm::runtime::compartment_id id):
    corevm::runtime::runtime_error(
      str(boost::format("Cannot find compartment %llu") % id)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class closure_not_found_error : public corevm::runtime::runtime_error
{
public:
  explicit closure_not_found_error(corevm::runtime::closure_id id):
    corevm::runtime::runtime_error(
      str(boost::format("Cannot find closure %lld") % id)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class signal_error : public corevm::runtime::runtime_error
{
public:
  explicit signal_error(const char* what_arg):
    corevm::runtime::runtime_error(what_arg)
  {
  }

  explicit signal_error(const std::string& what_arg):
    corevm::runtime::runtime_error(what_arg)
  {
  }
};

// -----------------------------------------------------------------------------

class execution_signal_error : public corevm::runtime::signal_error
{
public:
  explicit execution_signal_error(const char* signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled arithmetic/execution signal %s") % signal)
    )
  {
  }

  explicit execution_signal_error(const std::string& signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled arithmetic/execution signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class termination_signal_error : public corevm::runtime::signal_error
{
public:
  explicit termination_signal_error(const char* signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled termination signal %s") % signal)
    )
  {
  }

  explicit termination_signal_error(const  std::string& signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled termination signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class operation_signal_error : public corevm::runtime::signal_error
{
public:
  explicit operation_signal_error(const char* signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled operation signal %s") % signal)
    )
  {
  }

  explicit operation_signal_error(const std::string& signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled operation signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

class io_signal_error : public corevm::runtime::signal_error
{
public:
  explicit io_signal_error(const char* signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled IO signal %s") % signal)
    )
  {
  }

  explicit io_signal_error(const std::string& signal):
    corevm::runtime::signal_error(
      str(format("Received unhandled IO signal %s") % signal)
    )
  {
  }
};

// -----------------------------------------------------------------------------

} /* end namespace runtime */


} /* end namespace corevm */


#endif /* RUNTIME_ERRORS_H_ */
