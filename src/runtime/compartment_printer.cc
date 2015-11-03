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
#include "compartment_printer.h"

#include <ostream>


// -----------------------------------------------------------------------------

corevm::runtime::compartment_printer::compartment_printer(
  const corevm::runtime::compartment& compartment,
  uint32_t opts)
  :
  m_compartment(compartment),
  m_opts(opts)
{
}

// -----------------------------------------------------------------------------

std::ostream&
corevm::runtime::compartment_printer::operator()(std::ostream& ost) const
{
  ost << "Compartment" << std::endl;
  ost << std::endl;
  ost << "Path: " << m_compartment.path() << std::endl;
  ost << std::endl;

  for (const auto& closure : m_compartment.m_closure_table)
  {
    corevm::runtime::closure_printer printer(closure, m_opts);
    printer(ost) << std::endl;
  }

  return ost;
}

// -----------------------------------------------------------------------------
