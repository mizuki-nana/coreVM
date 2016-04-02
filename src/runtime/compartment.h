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


/** Forward declaration of `CompartmentPrinter` */
class CompartmentPrinter;


class Compartment
{
public:
  explicit Compartment(const std::string&);

  const std::string& path() const;

  void set_encoding_map(const EncodingMap&);

  void set_encoding_map(const EncodingMap&&);

  std::string get_encoding_string(encoding_key) const
    throw(EncodingStringNotFoundError);

  void get_encoding_string(encoding_key, std::string*) const;

  size_t closure_count() const;

  const Closure
    get_closure_by_id(closure_id) const
    throw(ClosureNotFoundError);

  void get_closure_by_id(
    closure_id, Closure** closure);

  void set_closure_table(const ClosureTable&&);

  bool get_starting_closure(Closure**);

  friend class CompartmentPrinter;

private:
  const std::string m_path;
  EncodingMap m_encoding_map;
  ClosureTable m_closure_table;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_COMPARTMENT_H_ */
