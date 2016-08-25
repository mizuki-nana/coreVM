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
#include "instr_printer.h"

#include "debug_opts.h"
#include "instr_info.h"

#include <iomanip>
#include <iostream>


namespace corevm {
namespace runtime {

// -----------------------------------------------------------------------------

InstrPrinter::InstrPrinter(
  const Instr& instr,
  uint32_t opts)
  :
  m_instr(instr),
  m_opts(opts)
{

}

// -----------------------------------------------------------------------------

std::ostream&
InstrPrinter::operator()(std::ostream& ost) const
{
  const bool show_canonical_form = m_opts & OPT_SHOW_CANONICAL_FORM;

  ost << std::hex << std::showbase;
  ost << std::setiosflags(std::ios::left);

  if (show_canonical_form)
  {
    ost << std::setw(10);
    ost << InstrSetInfo::instr_infos[m_instr.code].name;
  }
  else
  {
    ost << std::setw(6);
    ost << m_instr.code;
  }
  ost << " ";

  ost << std::setw(6) << m_instr.oprd1 << " ";
  ost << std::setw(6) << m_instr.oprd2;
  ost << std::resetiosflags(std::ios::adjustfield);
  ost << std::noshowbase << std::dec;

  return ost;
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
