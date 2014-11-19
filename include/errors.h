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
#ifndef COREVM_ERRORS_H_
#define COREVM_ERRORS_H_


#include <stdexcept>
#include <string>


namespace corevm {


namespace errors {


class corevm_runtime_error : public std::runtime_error {
public:
  explicit corevm_runtime_error(const std::string& what_arg): std::runtime_error(what_arg) {}
  explicit corevm_runtime_error(const char* what_arg): std::runtime_error(what_arg) {}
};


class corevm_logic_error : public std::logic_error {
public:
  explicit corevm_logic_error(const std::string& what_arg): std::logic_error(what_arg) {}
  explicit corevm_logic_error(const char* what_arg): std::logic_error(what_arg) {}
};


} /* end namespace errors */


} /* end namespace corevm */


#endif /* COREVM_ERRORS_H_ */
