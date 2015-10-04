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
#ifndef COREVM_INVOCATION_CTX_H_
#define COREVM_INVOCATION_CTX_H_

#include "closure_ctx.h"
#include "common.h"
#include "errors.h"
#include "dyobj/dyobj_id.h"

#include <unordered_map>
#include <vector>


namespace corevm {


namespace runtime {


/** Forward declaration of `corevm::runtime::closure` */
struct closure;

/** Forward declaration of `corevm::runtime::compartment` */
class compartment;


typedef std::vector<corevm::dyobj::dyobj_id> param_list_type;
typedef std::unordered_map<corevm::runtime::variable_key, corevm::dyobj::dyobj_id> param_value_map_type;


class invocation_ctx
{
public:
  invocation_ctx(const corevm::runtime::closure_ctx&,
    corevm::runtime::compartment*, corevm::runtime::closure*);

  const closure_ctx& closure_ctx() const;

  corevm::runtime::compartment* compartment_ptr() const;

  corevm::runtime::closure* closure_ptr() const;

  const param_list_type& params_list() const;

  const param_value_map_type& param_value_map() const;

  bool has_params() const;

  void put_param(const corevm::dyobj::dyobj_id&);

  const corevm::dyobj::dyobj_id pop_param()
    throw(corevm::runtime::missing_parameter_error);

  bool has_param_value_pairs() const;

  bool has_param_value_pair_with_key(const corevm::runtime::variable_key&) const;

  void put_param_value_pair(
    const corevm::runtime::variable_key&, const corevm::dyobj::dyobj_id&);

  const corevm::dyobj::dyobj_id pop_param_value_pair(const corevm::runtime::variable_key&)
    throw(corevm::runtime::missing_parameter_error);

  std::vector<corevm::runtime::variable_key> param_value_pair_keys() const;

private:
  corevm::runtime::closure_ctx m_closure_ctx;
  corevm::runtime::compartment* m_compartment_ptr;
  corevm::runtime::closure* m_closure_ptr;
  param_list_type m_params_list;
  param_value_map_type m_param_value_map;
  size_t m_params_list_pop_index;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INVOCATION_CTX_H_ */
