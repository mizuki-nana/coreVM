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
#include "closure.h"

#include <limits>
#include <ostream>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

static_assert(
  std::numeric_limits<closure_table::size_type>::max() >=
  std::numeric_limits<corevm::runtime::closure_id>::max(),
  "Closure ID incompatibility");

// -----------------------------------------------------------------------------

std::ostream& operator<<(
  std::ostream& ost, const corevm::runtime::closure& closure)
{
  ost << "Closure" << std::endl;
  ost << std::endl;
  ost << "-- BEGIN --" << std::endl;
  ost << std::endl;
  ost << "Name: " << closure.name << std::endl;
  ost << "ID: " << closure.id << std::endl;
  ost << "Parent ID: " << closure.parent_id << std::endl;
  ost << "Vector: " << std::endl << closure.vector;
  ost << std::endl;
  ost << "-- END --" << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------

corevm::runtime::closure_printer::closure_printer(
  const corevm::runtime::closure& closure)
  :
  m_closure(closure)
{
}

// -----------------------------------------------------------------------------

std::ostream&
corevm::runtime::closure_printer::operator()(std::ostream& ost)
{
  ost << "Closure:" << std::endl;
  ost << std::endl;
  ost << "Name: " << m_closure.name << std::endl;
  ost << "ID: " << m_closure.id << std::endl;
  ost << "Parent ID: " << m_closure.parent_id << std::endl;
  ost << "Vector:" << std::endl;
  ost << m_closure.vector << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
