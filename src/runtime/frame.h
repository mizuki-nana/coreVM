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
#include "runtime_types.h"
#include "dyobj/common.h"
#include "types/fwd.h"

#include <cstdint>
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
  typedef RuntimeTypes::dyobj_ptr_type dyobj_ptr;

  Frame(const ClosureCtx&, Compartment*, Closure*);

  Frame(const ClosureCtx&, Compartment*, Closure*, instr_addr_t);

  ~Frame();

  size_t eval_stack_size() const;

  instr_addr_t return_addr() const;

  void set_return_addr(const instr_addr_t);

  void push_eval_stack(const types::NativeTypeHandle&);

  void push_eval_stack(types::NativeTypeHandle&&);

  types::NativeTypeHandle pop_eval_stack();

  types::NativeTypeHandle& top_eval_stack();

  void swap_eval_stack();

  const std::vector<types::NativeTypeHandle>& eval_stack() const;

  types::NativeTypeHandle& eval_stack_element(size_t i);

  size_t visible_var_count() const;

  bool has_visible_var(const variable_key_t) const;

  dyobj_ptr get_visible_var(const variable_key_t) const;

  bool get_visible_var_fast(const variable_key_t, dyobj_ptr*) const;

  dyobj_ptr pop_visible_var(const variable_key_t);

  void set_visible_var(variable_key_t, dyobj_ptr);

  size_t invisible_var_count() const;

  bool has_invisible_var(const variable_key_t) const;

  dyobj_ptr get_invisible_var(const variable_key_t) const;

  bool get_invisible_var_fast(const variable_key_t, dyobj_ptr*) const;

  dyobj_ptr pop_invisible_var(const variable_key_t);

  void set_invisible_var(variable_key_t, dyobj_ptr);

  std::vector<variable_key_t> visible_var_keys() const;

  std::vector<variable_key_t> invisible_var_keys() const;

  std::vector<dyobj_ptr> get_visible_objs() const;

  std::vector<dyobj_ptr> get_invisible_objs() const;

  ClosureCtx closure_ctx() const;

  Compartment* compartment_ptr() const;

  Closure* closure_ptr() const;

  Frame* parent() const;

  /**
   * Sets the optional parent of the frame in the process.
   */
  void set_parent(Frame*);

  dyobj_ptr exc_obj() const;

  void set_exc_obj(dyobj_ptr);

  void clear_exc_obj();

protected:
  const runtime::ClosureCtx m_closure_ctx;
  Compartment* m_compartment_ptr;
  Closure* m_closure_ptr;
  Frame* m_parent;
  instr_addr_t m_return_addr;
  std::unordered_map<variable_key_t, dyobj_ptr> m_visible_vars;
  std::unordered_map<variable_key_t, dyobj_ptr> m_invisible_vars;
  std::vector<types::NativeTypeHandle> m_eval_stack;
  dyobj_ptr m_exc_obj;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_FRAME_H_ */
