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
#ifndef COREVM_LOGGING_H_
#define COREVM_LOGGING_H_

#include <sneaker/logging/logging.h>

#include <string>


namespace corevm {


// -----------------------------------------------------------------------------

typedef sneaker::logging::logger<
  sneaker::logging::thread_safe_tag, sneaker::logging::exception_safe_tag> Logger;

// -----------------------------------------------------------------------------

sneaker::logging::log_scheme* log_mode_to_scheme(const std::string& mode);

// -----------------------------------------------------------------------------

class Loggable
{
public:
  void set_logger(Logger*);

protected:
  Loggable();

  Logger* m_logger;
};

// -----------------------------------------------------------------------------


} /* end namespace corevm */


#endif /* COREVM_LOGGING_H_ */
