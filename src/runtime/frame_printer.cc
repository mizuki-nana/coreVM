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
#include "frame_printer.h"

#include "closure.h"
#include "compartment.h"

#include <iostream>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

FramePrinter::FramePrinter(
  const Frame& frame,
  uint32_t opts)
  :
  m_frame(frame),
  m_opts(opts)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

std::ostream&
FramePrinter::operator()(std::ostream& ost) const
{
  const Closure& closure = *(m_frame.closure_ptr());
  ost << "Frame info for: " << closure.name << std::endl;

  ost << std::endl;
  ost << "--- BEGIN ---" << std::endl;
  ost << std::endl;

  // Eval stack info.
  ost << "Eval stack size: " << m_frame.eval_stack_size() << std::endl;

  Compartment* compartment = m_frame.compartment_ptr();

  // Visible variables info.
  {
    ost << "Visible vars: " << m_frame.visible_var_count() << std::endl;
    print_variables(ost, compartment, m_frame.visible_var_keys());
  }

  ost << std::endl;

  // Invisible variables info.
  {
    ost << "Invisible vars: " << m_frame.invisible_var_count() << std::endl;
    print_variables(ost, compartment, m_frame.invisible_var_keys());
  }

  // Closure info.
  {
    ClosurePrinter closure_printer(closure, m_opts);
    closure_printer(ost) << std::endl;
  }

  // Parent closure info.
  {
    if (closure.parent_id != NONESET_CLOSURE_ID)
    {
      runtime::Closure* parent_closure = nullptr;
      compartment->get_closure_by_id(closure.parent_id, &parent_closure);

      if (parent_closure)
      {
        ost << "Outer scope: " << parent_closure->name << std::endl;
        ost << std::endl;
      }
    }
  }

  ost << "--- END ---" << std::endl;
  ost << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------

template<typename V>
void
FramePrinter::print_variables(std::ostream& ost,
  const Compartment* compartment, const V& vars) const
{
  ost << std::endl;
  for (const auto var : vars)
  {
    std::string var_name;
    auto encoding_key = static_cast<runtime::encoding_key_t>(var);
    compartment->get_string_literal(encoding_key, &var_name);
    ost << "  - " << var_name << std::endl;
  }
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
