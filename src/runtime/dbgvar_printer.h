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
#ifndef COREVM_DBGVAR_PRINTER_H_
#define COREVM_DBGVAR_PRINTER_H_

#include "fwd.h"
#include "runtime_types.h"

#include <iosfwd>


namespace corevm {


namespace runtime {


class DbgvarPrinter
{
public:
  DbgvarPrinter(const Process& process, const Frame& frame,
    const char* variable_name, RuntimeTypes::dyobj_ptr_type obj);

  std::ostream& operator()(std::ostream&) const;

private:
  const Process& m_process;
  const Frame& m_frame;
  const char* m_variable_name;
  RuntimeTypes::dyobj_ptr_type m_obj;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_DBGVAR_PRINTER_H_ */
