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
#include "frame_printer.h"

#include "closure.h"
#include "compartment.h"

#include <iostream>


// -----------------------------------------------------------------------------

corevm::runtime::frame_printer::frame_printer(
  const corevm::runtime::frame& frame)
  :
  m_frame(frame)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

std::ostream&
corevm::runtime::frame_printer::operator()(std::ostream& ost) const
{
  const corevm::runtime::closure& closure = *(m_frame.closure_ptr());
  ost << "Frame info for: " << closure.name << std::endl;

  ost << std::endl;
  ost << "--- BEGIN ---" << std::endl;
  ost << std::endl;

  // Eval stack info.
  ost << "Eval stack size: " << m_frame.eval_stack_size() << std::endl;

  corevm::runtime::compartment* compartment = m_frame.compartment_ptr();

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
    corevm::runtime::closure_printer closure_printer(closure);
    closure_printer(ost) << std::endl;
  }

  // Parent closure info.
  {
    if (closure.parent_id != corevm::runtime::NONESET_CLOSURE_ID)
    {
      corevm::runtime::closure* parent_closure = nullptr;
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
corevm::runtime::frame_printer::print_variables(std::ostream& ost,
  const corevm::runtime::compartment* compartment, const V& vars) const
{
  ost << std::endl;
  for (const auto var : vars)
  {
    std::string var_name;
    auto encoding_key = static_cast<corevm::runtime::encoding_key>(var);
    compartment->get_encoding_string(encoding_key, &var_name);
    ost << "  - " << var_name << std::endl;
  }
}

// -----------------------------------------------------------------------------
