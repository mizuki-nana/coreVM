/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#ifndef runtime_errorS_H_
#define runtime_errorS_H_

#include <string>
#include <boost/format.hpp>
#include "../errors.h"


namespace corevm {


namespace runtime {


class runtime_error : public corevm::runtime_error {
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


class frame_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit frame_not_found_error():
    corevm::runtime::runtime_error("Expected frame not found")
  {
  }
};


class evaluation_stack_empty_error : public corevm::runtime::runtime_error {
public:
  explicit evaluation_stack_empty_error():
    corevm::runtime::runtime_error("Evaluation stack is empty")
  {
  }
};


class local_variable_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit local_variable_not_found_error():
    corevm::runtime::runtime_error("Local variable not found")
  {
  }
};


class invalid_instr_error : public corevm::runtime::runtime_error {
public:
  explicit invalid_instr_error():
    corevm::runtime::runtime_error("Invalid instruction code encountered")
  {
  }
};


class object_stack_empty_error : public corevm::runtime::runtime_error {
public:
  explicit object_stack_empty_error():
    corevm::runtime::runtime_error("Process's object stack is empty")
  {
  }
};


class native_type_handle_not_found_error : public corevm::runtime::runtime_error {
public:
  explicit native_type_handle_not_found_error():
    corevm::runtime::runtime_error("Native type handle not found")
  {
  }
};


class native_type_handle_deletion_error : public corevm::runtime::runtime_error {
public:
  explicit native_type_handle_deletion_error():
    corevm::runtime::runtime_error("Native type handle cannot be deleted")
  {
  }
};


class missing_parameter_error : public corevm::runtime::runtime_error {
public:
  explicit missing_parameter_error():
    corevm::runtime::runtime_error("Missing parameter")
  {
  }
};


class invalid_instr_addr_error : public corevm::runtime::runtime_error {
public:
  explicit invalid_instr_addr_error():
    corevm::runtime::runtime_error("Invalid instruction address")
  {
  }
};


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


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* runtime_errorS_H_ */
