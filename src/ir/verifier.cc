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
#include "verifier.h"
#include "format.h"

#include <string>

namespace corevm {
namespace ir {

// -----------------------------------------------------------------------------

class Verifier::Impl
{
public:
  Impl();

  bool verify(const IRModule&);

  const char* msg() const;
private:
  bool verify(const IRClosure&);

  std::string m_msg;
};

// -----------------------------------------------------------------------------

Verifier::Verifier()
  :
  m_impl(new Verifier::Impl())
{
}

// -----------------------------------------------------------------------------

bool
Verifier::verify(const IRModule& module)
{
  return m_impl->verify(module);
}

// -----------------------------------------------------------------------------

const char*
Verifier::msg() const
{
  return m_impl->msg();
}

// -----------------------------------------------------------------------------

Verifier::Impl::Impl()
  :
  m_msg()
{
}

// -----------------------------------------------------------------------------

bool
Verifier::Impl::verify(const IRModule& /* module */)
{
  // TODO: [COREVM-548] Implement IR verification pass
  return false;
}

// -----------------------------------------------------------------------------

bool
Verifier::Impl::verify(const IRClosure& /* closure */)
{
  // TODO: [COREVM-548] Implement IR verification pass
  return false;
}

// -----------------------------------------------------------------------------

const char*
Verifier::Impl::msg() const
{
    return m_msg.c_str();
}

// -----------------------------------------------------------------------------

} /* end namespace ir */
} /* end namespace corevm */
