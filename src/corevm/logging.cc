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
#include "logging.h"


namespace corevm {

// -----------------------------------------------------------------------------

class void_log_scheme : public sneaker::logging::log_scheme
{
public:
  virtual void write(const char* msg);
};

// -----------------------------------------------------------------------------

void
void_log_scheme::write(const char* /* msg */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

sneaker::logging::log_scheme*
log_mode_to_scheme(const std::string& mode)
{
  sneaker::logging::log_scheme* scheme = NULL;

  if (mode == "")
  {
    scheme = new void_log_scheme();
  }
  else if (mode == "stdout")
  {
    scheme = new sneaker::logging::stdout_log_scheme();
  }
  else if (mode == "stderr")
  {
    scheme = new sneaker::logging::stderr_log_scheme();
  }
  else
  {
    scheme = new sneaker::logging::file_log_scheme(mode.c_str());
  }

  return scheme;
}

// -----------------------------------------------------------------------------

Loggable::Loggable()
  :
  m_logger(nullptr)
{
}

// -----------------------------------------------------------------------------

void
Loggable::set_logger(Logger* logger)
{
  m_logger = logger;
}

// -----------------------------------------------------------------------------

} /* end namespace corevm */
