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
#ifndef COREVM_FRAME_H_
#define COREVM_FRAME_H_

#include "closure_ctx.h"
#include "common.h"
#include "errors.h"
#include "dyobj/dyobj_id.h"
#include "types/fwd.h"

#include <cstdint>
#include <list>
#include <unordered_map>
#include <vector>


namespace corevm {


namespace runtime {


/** Forward declaration of `Closure` */
struct Closure;


/** Forward declaration of `Compartment` */
class Compartment;


/**
 * Each frame is consisted of:
 *
 * - Return address.
 * - Visible local variables.
 * - Invisible local variables.
 * - Evaluation stack.
 * - Closure context.
 * - A pointer to the associated compartment.
 * - A pointer to the associated closure object.
 */
class Frame
{
public:
  Frame(const ClosureCtx&,
    Compartment*, Closure*);

  Frame(const ClosureCtx&,
    Compartment*,
    Closure*, instr_addr);

  ~Frame();

  size_t eval_stack_size() const;

  instr_addr return_addr() const;

  void set_return_addr(const instr_addr);

  void push_eval_stack(types::native_type_handle&);

  void push_eval_stack(types::native_type_handle&&);

  types::native_type_handle pop_eval_stack()
    throw(EvaluationStackEmptyError);

  types::native_type_handle& top_eval_stack()
    throw(EvaluationStackEmptyError);

  void swap_eval_stack();

  const std::vector<types::native_type_handle>& eval_stack() const;

  types::native_type_handle& eval_stack_element(size_t i);

  size_t visible_var_count() const;

  bool has_visible_var(const variable_key) const;

  dyobj::dyobj_id get_visible_var(const variable_key)
    const throw(NameNotFoundError);

  bool get_visible_var_fast(
    const variable_key, dyobj::dyobj_id*) const;

  dyobj::dyobj_id pop_visible_var(const variable_key)
    throw(NameNotFoundError);

  void set_visible_var(variable_key, dyobj::dyobj_id);

  size_t invisible_var_count() const;

  bool has_invisible_var(const variable_key) const;

  dyobj::dyobj_id get_invisible_var(const variable_key)
    const throw(NameNotFoundError);

  bool get_invisible_var_fast(
    const variable_key, dyobj::dyobj_id*) const;

  dyobj::dyobj_id pop_invisible_var(const variable_key)
    throw(NameNotFoundError);

  void set_invisible_var(variable_key, dyobj::dyobj_id);

  std::vector<variable_key> visible_var_keys() const;

  std::vector<variable_key> invisible_var_keys() const;

  std::list<dyobj::dyobj_id> get_visible_objs() const;

  std::list<dyobj::dyobj_id> get_invisible_objs() const;

  ClosureCtx closure_ctx() const;

  Compartment* compartment_ptr() const;

  Closure* closure_ptr() const;

  Frame* parent() const;

  /**
   * Sets the optional parent of the frame in the process.
   */
  void set_parent(Frame*);

  dyobj::dyobj_id exc_obj() const;

  void set_exc_obj(dyobj::dyobj_id exc_obj);

  void clear_exc_obj();

protected:
  const runtime::ClosureCtx m_closure_ctx;
  Compartment* m_compartment_ptr;
  Closure* m_closure_ptr;
  Frame* m_parent;
  instr_addr m_return_addr;
  std::unordered_map<variable_key, dyobj::dyobj_id> m_visible_vars;
  std::unordered_map<variable_key, dyobj::dyobj_id> m_invisible_vars;
  std::vector<types::native_type_handle> m_eval_stack;
  dyobj::dyobj_id m_exc_obj;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_FRAME_H_ */
