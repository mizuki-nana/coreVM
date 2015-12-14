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
#ifndef COREVM_COMPARTMENT_H_
#define COREVM_COMPARTMENT_H_

#include "closure.h"
#include "common.h"
#include "errors.h"

#include <string>


namespace corevm {


namespace runtime {


/** Forward declaration of `compartment_printer` */
class compartment_printer;


class compartment
{
public:
  explicit compartment(const std::string&);

  const std::string& path() const;

  void set_encoding_map(const encoding_map&);

  void set_encoding_map(const encoding_map&&);

  std::string get_encoding_string(encoding_key) const
    throw(encoding_string_not_found_error);

  void get_encoding_string(encoding_key, std::string*) const;

  size_t closure_count() const;

  const closure
    get_closure_by_id(closure_id) const
    throw(closure_not_found_error);

  void get_closure_by_id(
    closure_id, closure** closure);

  void set_closure_table(const closure_table&&);

  bool get_starting_closure(closure**);

  friend class compartment_printer;

private:
  const std::string m_path;
  encoding_map m_encoding_map;
  closure_table m_closure_table;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_COMPARTMENT_H_ */
