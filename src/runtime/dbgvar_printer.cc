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
#include "dbgvar_printer.h"
#include "compartment.h"
#include "frame.h"
#include "process.h"

#include <ostream>
#include <sstream>
#include <string>


namespace corevm {
namespace runtime {

// -----------------------------------------------------------------------------

DbgvarPrinter::DbgvarPrinter(const Process& process, const Frame& frame,
  const char* variable_name, RuntimeTypes::dyobj_ptr_type obj)
  :
  m_process(process),
  m_frame(frame),
  m_variable_name(variable_name),
  m_obj(obj)
{
}

// -----------------------------------------------------------------------------

std::ostream&
DbgvarPrinter::operator()(std::ostream& ost) const
{
  std::stringstream ss;

  ss << "------------------- DEBUG -------------------" << std::endl;
  ss << std::setw(10) << "Module: " << m_frame.compartment()->path() << std::endl;
  ss << std::setw(10) << "Frame: " << m_frame.closure()->name << std::endl;
  ss << std::setw(10) << "Variable: " << m_variable_name << std::endl;
  ss << std::endl;

  ss << "\tAttribute count: " << m_obj->attr_count() << std::endl;

  size_t i = 1;
  for (auto itr = m_obj->begin(); itr != m_obj->end(); ++itr, ++i)
  {
    auto attr_key = itr->first;
    auto attr_obj = itr->second;
    const char* attr_name = NULL;
    m_process.get_attr_name(attr_key, &attr_name);

    ss << "\t" << std::setw(20) << attr_name << " : ";
    ss << "0x" << std::hex << reinterpret_cast<uint64_t>(attr_obj) << std::dec;
    ss << " [" << i << "]" << std::endl;
  }

  ss << "\tFlags: " << m_obj->flags() << std::endl;
  ss << "\tIs garbage collectible: " << m_obj->is_garbage_collectible() << std::endl;
  ss << std::endl;

  ost << ss.str();

  return ost;
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
