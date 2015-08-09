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

#include "closure.h"
#include "closure_ctx.h"
#include "compartment.h"
#include "common.h"
#include "errors.h"
#include "dyobj/dyobj_id.h"
#include "types/native_type_handle.h"

#include <cstdint>
#include <list>
#include <vector>


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
class frame
{
public:
  frame(const corevm::runtime::closure_ctx&,
    corevm::runtime::compartment*, corevm::runtime::closure*);

  frame(const corevm::runtime::closure_ctx&,
    corevm::runtime::compartment*,
    corevm::runtime::closure*, corevm::runtime::instr_addr);

  ~frame();

  uint32_t eval_stack_size() const;

  corevm::runtime::instr_addr return_addr() const;

  void set_return_addr(const corevm::runtime::instr_addr);

  void push_eval_stack(corevm::types::native_type_handle&);

  void push_eval_stack(corevm::types::native_type_handle&&);

  corevm::types::native_type_handle pop_eval_stack()
    throw(corevm::runtime::evaluation_stack_empty_error);

  corevm::types::native_type_handle& top_eval_stack()
    throw(corevm::runtime::evaluation_stack_empty_error);

  const std::vector<corevm::types::native_type_handle>& eval_stack() const;

  corevm::types::native_type_handle& eval_stack_element(size_t i);

  bool has_visible_var(const corevm::runtime::variable_key) const;

  corevm::dyobj::dyobj_id get_visible_var(const corevm::runtime::variable_key)
    const throw(corevm::runtime::name_not_found_error);

  bool get_visible_var_fast(
    const corevm::runtime::variable_key, corevm::dyobj::dyobj_id*) const;

  corevm::dyobj::dyobj_id pop_visible_var(const corevm::runtime::variable_key)
    throw(corevm::runtime::name_not_found_error);

  void set_visible_var(corevm::runtime::variable_key, corevm::dyobj::dyobj_id);

  bool has_invisible_var(const corevm::runtime::variable_key) const;

  corevm::dyobj::dyobj_id get_invisible_var(const corevm::runtime::variable_key)
    const throw(corevm::runtime::name_not_found_error);

  bool get_invisible_var_fast(
    const corevm::runtime::variable_key, corevm::dyobj::dyobj_id*) const;

  corevm::dyobj::dyobj_id pop_invisible_var(const corevm::runtime::variable_key)
    throw(corevm::runtime::name_not_found_error);

  void set_invisible_var(corevm::runtime::variable_key, corevm::dyobj::dyobj_id);

  std::list<corevm::dyobj::dyobj_id> get_visible_objs() const;

  std::list<corevm::dyobj::dyobj_id> get_invisible_objs() const;

  corevm::runtime::closure_ctx closure_ctx() const;

  corevm::runtime::compartment* compartment_ptr() const;

  corevm::runtime::closure* closure_ptr() const;

  corevm::runtime::frame* parent() const;

  /**
   * Sets the optional parent of the frame in the process.
   */
  void set_parent(corevm::runtime::frame*);

  corevm::dyobj::dyobj_id exc_obj() const;

  void set_exc_obj(corevm::dyobj::dyobj_id exc_obj);

  void clear_exc_obj();

protected:
  const corevm::runtime::closure_ctx m_closure_ctx;
  corevm::runtime::compartment* m_compartment_ptr;
  corevm::runtime::closure* m_closure_ptr;
  corevm::runtime::frame* m_parent;
  corevm::runtime::instr_addr m_return_addr;
  std::unordered_map<corevm::runtime::variable_key, corevm::dyobj::dyobj_id> m_visible_vars;
  std::unordered_map<corevm::runtime::variable_key, corevm::dyobj::dyobj_id> m_invisible_vars;
  std::vector<corevm::types::native_type_handle> m_eval_stack;
  corevm::dyobj::dyobj_id m_exc_obj;
};


}; /* end namespace runtime */


}; /* end namespace corevm */


#endif /* COREVM_FRAME_H_ */
