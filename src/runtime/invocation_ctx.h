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
#ifndef COREVM_INVOCATION_CTX_H_
#define COREVM_INVOCATION_CTX_H_

#include "closure.h"
#include "closure_ctx.h"
#include "common.h"
#include "errors.h"
#include "linear_map.h"
#include "runtime_types.h"
#include "corevm/llvm_smallvector.h"


namespace corevm {
namespace runtime {

/** Forward declaration of `Compartment` */
class Compartment;


class InvocationCtx
{
public:
  typedef RuntimeTypes::dynamic_object_type* param_type;

private:
  typedef llvm::SmallVector<param_type, 20> param_list_type;

  typedef LinearMap<variable_key_t, param_type,
    llvm::SmallVector<std::pair<variable_key_t, param_type>, 20>> param_value_map_type;

public:
  InvocationCtx(const ClosureCtx&, Compartment*, Closure*);

  const ClosureCtx& closure_ctx() const;

  Compartment* compartment() const;

  Closure* closure() const;

  const param_list_type& params_list() const;

  const param_value_map_type& param_value_map() const;

  bool has_params() const;

  void put_param(param_type);

  param_type pop_param();

  bool has_param_value_pairs() const;

  bool has_param_value_pair_with_key(variable_key_t) const;

  void put_param_value_pair(variable_key_t, param_type);

  param_type pop_param_value_pair(variable_key_t);

  std::vector<variable_key_t> param_value_pair_keys() const;

private:
  runtime::ClosureCtx m_closure_ctx;
  Compartment* m_compartment;
  Closure* m_closure;
  param_list_type m_params_list;
  param_value_map_type m_param_value_map;
  size_t m_params_list_pop_index;
};

} /* end namespace runtime */
} /* end namespace corevm */


#endif /* COREVM_INVOCATION_CTX_H_ */
