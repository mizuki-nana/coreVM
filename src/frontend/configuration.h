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
#ifndef COREVM_CONFIGURATION_H_
#define COREVM_CONFIGURATION_H_

#include "errors.h"

#include <sneaker/json/json.h>

#include <cstdint>
#include <string>


using sneaker::json::JSON;


namespace corevm {


namespace frontend {


class configuration
{
public:
  configuration();

  static configuration load_config(const std::string&)
    throw(corevm::frontend::configuration_loading_error);

public:
  /* Value accessors. */
  uint64_t heap_alloc_size() const;

  uint64_t pool_alloc_size() const;

  uint32_t gc_interval() const;

  /* Value setters. */
  void set_heap_alloc_size(uint64_t);

  void set_pool_alloc_size(uint64_t);

  void set_gc_interval(uint32_t);

private:
  static void set_values(configuration&, const JSON&);

  uint64_t m_heap_alloc_size;
  uint64_t m_pool_alloc_size;
  uint32_t m_gc_interval;

private:
  static const std::string schema;
};


} /* end namespace frontend */


} /* end namespace corevm */


#endif /* COREVM_CONFIGURATION_H_ */
