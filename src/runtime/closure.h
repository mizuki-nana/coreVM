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
#ifndef COREVM_CLOSURE_H_
#define COREVM_CLOSURE_H_

#include "catch_site.h"
#include "common.h"
#include "loc_info.h"
#include "vector.h"

#include <iosfwd>
#include <string>
#include <vector>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

typedef struct closure
{
  closure()
    :
    name(),
    id(NONESET_CLOSURE_ID),
    parent_id(NONESET_CLOSURE_ID),
    vector(),
    locs(),
    catch_sites()
  {
  }

  closure(
    std::string name_,
    closure_id id_,
    closure_id parent_id_,
    vector vector_,
    loc_table locs_,
    catch_site_list catch_sites_)
    :
    name(name_),
    id(id_),
    parent_id(parent_id_),
    vector(vector_),
    locs(locs_),
    catch_sites(catch_sites_)
  {
  }

  std::string name;
  closure_id id;
  closure_id parent_id;
  vector vector;
  loc_table locs;
  catch_site_list catch_sites;
} closure;

// -----------------------------------------------------------------------------

typedef std::vector<closure> closure_table;

// -----------------------------------------------------------------------------

struct closure_printer
{
  closure_printer(const closure&, uint32_t opts);

  std::ostream& operator()(std::ostream&) const;

private:
  const closure& m_closure;
  const uint32_t m_opts;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_CLOSURE_H_ */
