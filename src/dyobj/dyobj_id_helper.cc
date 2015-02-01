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

#include "../../include/dyobj/dyobj_id_helper.h"

#include "../../include/dyobj/errors.h"

#include <stdexcept>

#include <boost/lexical_cast.hpp>


sneaker::atomic::atomic_incrementor<
  corevm::dyobj::dyobj_id,
  corevm::dyobj::DYOBJ_LIMIT
> corevm::dyobj::dyobj_id_helper::atomic_value = 0;


corevm::dyobj::dyobj_id
corevm::dyobj::dyobj_id_helper::generate_dyobj_id()
{
  try
  {
    return ++corevm::dyobj::dyobj_id_helper::atomic_value;
  }
  catch (const std::underflow_error&)
  {
    throw corevm::dyobj::object_id_exceed_limit_error();
  }
  catch (const std::overflow_error&)
  {
    throw corevm::dyobj::object_id_exceed_limit_error();
  }
}

std::string
corevm::dyobj::dyobj_id_helper::id_to_string(corevm::dyobj::dyobj_id id)
{
  return boost::lexical_cast<std::string>(id);
}
