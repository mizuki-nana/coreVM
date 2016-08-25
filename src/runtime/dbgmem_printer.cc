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
#include "dbgmem_printer.h"

#include <sneaker/utility/os.h>

#include <iostream>


namespace corevm {
namespace runtime {

// -----------------------------------------------------------------------------

DbgMemPrinter::DbgMemPrinter(uint32_t opts)
  :
  m_opts(opts)
{
}

// -----------------------------------------------------------------------------

std::ostream&
DbgMemPrinter::operator()(std::ostream& ost) const
{
  ost << "Memory usages:" << std::endl;

  uint64_t vm_peak = 0;
  uint64_t vm_size = 0;
  uint64_t vm_hwm = 0;
  uint64_t vm_rss = 0;

  sneaker::utility::get_process_mem_usage(&vm_peak, &vm_size, &vm_hwm, &vm_rss);

  const bool in_bytes = m_opts & DbgMemPrinter::OPTS_UNIT_BYTES;

  const std::string unit(in_bytes ? "bytes" : "kB");

  if (in_bytes)
  {
    vm_peak *= 1024;
    vm_size *= 1024;
    vm_hwm *= 1024;
    vm_rss *= 1024;
  }

  ost << "VmSize: " << vm_size << " " << unit << std::endl;
  ost << "VmRSS: " << vm_rss << " " << unit << std::endl;

  if (m_opts & DbgMemPrinter::OPTS_SHOW_PEAK)
  {
    ost << "VmPeak: " << vm_peak << " " << unit << std::endl;
    ost << "VmHWM: " << vm_hwm << " " << unit << std::endl;
  }

  return ost;
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
