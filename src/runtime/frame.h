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
#ifndef COREVM_FRAME_H_
#define COREVM_FRAME_H_

#include "closure.h"
#include "closure_ctx.h"
#include "common.h"
#include "errors.h"
#include "fwd.h"
#include "instr_fwd.h"
#include "runtime_types.h"
#include "dyobj/common.h"
#include "types/fwd.h"
#include "corevm/macros.h"

#include <cstdint>
#include <vector>

#if COREVM_USE_LINEAR_VARIABLE_TABLE
  #include "linear_map.h"
  #include "corevm/llvm_smallvector.h"
#else
  #include <unordered_map>
#endif // COREVM_USE_LINEAR_VARIABLE_TABLE


namespace corevm {


namespace runtime {


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

  instr_addr_t pc() const;

  void set_pc(instr_addr_t);

  void set_pc_safe(instr_addr_t);

  const Instr& current_instr() const;

  void inc_pc();

  bool can_execute() const;

  instr_addr_t return_addr() const;

  void set_return_addr(instr_addr_t);

  void push_eval_stack(const types::NativeTypeHandle&);

  void push_eval_stack(types::NativeTypeHandle&&);

  types::NativeTypeHandle pop_eval_stack();

  types::NativeTypeHandle& top_eval_stack();

  void swap_eval_stack();

  const std::vector<types::NativeTypeHandle>& eval_stack() const;

  types::NativeTypeHandle& eval_stack_element(size_t i);

  size_t visible_var_count() const;

  dyobj_ptr get_visible_var(variable_key_t) const;

  bool get_visible_var_fast(variable_key_t, dyobj_ptr*) const;

  bool get_visible_var_through_ancestry(variable_key_t, dyobj_ptr*);

  dyobj_ptr pop_visible_var(variable_key_t);

  void set_visible_var(variable_key_t, dyobj_ptr);

  size_t invisible_var_count() const;

  dyobj_ptr get_invisible_var(variable_key_t) const;

  bool get_invisible_var_fast(variable_key_t, dyobj_ptr*) const;

  bool get_invisible_var_through_ancestry(variable_key_t, dyobj_ptr*);

  dyobj_ptr pop_invisible_var(variable_key_t);

  void set_invisible_var(variable_key_t, dyobj_ptr);

  std::vector<variable_key_t> visible_var_keys() const;

  std::vector<variable_key_t> invisible_var_keys() const;

  std::vector<dyobj_ptr> get_visible_objs() const;

  std::vector<dyobj_ptr> get_invisible_objs() const;

  ClosureCtx closure_ctx() const;

  Compartment* compartment() const;

  Closure* closure() const;

  Frame* parent() const;

  /**
   * Sets the optional parent of the frame in the process.
   */
  void set_parent(Frame*);

  dyobj_ptr exc_obj() const;

  void set_exc_obj(dyobj_ptr);

  void clear_exc_obj();

protected:

#if COREVM_USE_LINEAR_VARIABLE_TABLE
  typedef LinearMap<variable_key_t, dyobj_ptr,
    llvm::SmallVector<std::pair<variable_key_t, dyobj_ptr>, 20>> VariableTable;
#else
  typedef std::unordered_map<variable_key_t, dyobj_ptr> VariableTable;
#endif

  instr_addr_t m_pc;
  const runtime::ClosureCtx m_closure_ctx;
  Compartment* m_compartment;
  Closure* m_closure;
  Frame* m_parent;
  instr_addr_t m_return_addr;
  VariableTable m_visible_vars;
  VariableTable m_invisible_vars;
  std::vector<types::NativeTypeHandle> m_eval_stack;
  dyobj_ptr m_exc_obj;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_FRAME_H_ */
