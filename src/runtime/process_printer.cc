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
#include "process_printer.h"

#include "compartment_printer.h"


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

ProcessPrinter::ProcessPrinter(
  const Process& process,
  uint32_t opts)
  :
  m_process(process),
  m_opts(opts)
{
}

// -----------------------------------------------------------------------------

std::ostream&
ProcessPrinter::operator()(std::ostream& ost) const
{
  ost << "Process" << std::endl;
  ost << std::endl;
  ost << "-- BEGIN --" << std::endl;

  ost << "Heap size: " << m_process.heap_size() << std::endl;
  ost << "Max heap size: " << m_process.max_heap_size() << std::endl;
  ost << "Native types pool size: " << m_process.ntvhndl_pool_size() << std::endl;
  ost << "Max native types pool size: " << m_process.max_ntvhndl_pool_size() << std::endl;
  ost << "Compartments: " << m_process.compartment_count() << std::endl;

  for (const auto& compartment : m_process.m_compartments)
  {
    CompartmentPrinter printer(compartment, m_opts);
    printer(ost) << std::endl;
  }

  ost << "Total Instructions: " << m_process.m_instrs.size() << std::endl;
  ost << "Program counter: " << m_process.pc() << std::endl;

  ost << m_process.m_dynamic_object_heap << std::endl;
  ost << m_process.m_ntvhndl_pool << std::endl;

  ost << "-- END --" << std::endl;
  ost << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
