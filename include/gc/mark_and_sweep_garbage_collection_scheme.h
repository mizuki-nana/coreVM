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
#ifndef COREVM_MARK_AND_SWEEP_GARBAGE_COLLECTION_SCHEME_H_
#define COREVM_MARK_AND_SWEEP_GARBAGE_COLLECTION_SCHEME_H_

#include "garbage_collection_scheme.h"

#include "../dyobj/dynamic_object.h"
#include "../dyobj/dynamic_object_heap.h"
#include "../dyobj/dynamic_object_manager.h"

#include <cstdint>


namespace corevm {


namespace gc {


class mark_and_sweep_garbage_collection_scheme : public garbage_collection_scheme
{
public:
  typedef class dynamic_object_manager : public corevm::dyobj::dynamic_object_manager
  {
    public:
      dynamic_object_manager()
        :
        m_marked(false)
      {
      }

      virtual inline bool garbage_collectible() const noexcept
      {
        return marked();
      }

      virtual inline void on_create() noexcept
      {
        // Do nothing here.
      }

      virtual inline void on_setattr() noexcept
      {
        // Do nothing here.
      }

      virtual inline void on_delattr() noexcept
      {
        // Do nothing here.
      }

      virtual inline void on_delete() noexcept
      {
        // Do nothing here.
      }

      virtual inline void on_exit() noexcept
      {
        // Do nothing here.
      }

      virtual inline bool marked() const noexcept
      {
        return m_marked;
      }

      virtual inline void mark() noexcept
      {
        m_marked = true;
      }

      virtual inline void unmark() noexcept
      {
        m_marked = false;
      }

    protected:
      bool m_marked;
  } mark_and_sweep_dynamic_object_manager;

  using dynamic_object_type = typename corevm::dyobj::dynamic_object<mark_and_sweep_dynamic_object_manager>;
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<mark_and_sweep_dynamic_object_manager>;

  virtual void gc(dynamic_object_heap_type&) const;

protected:
  virtual bool is_root_object(const dynamic_object_type&) const noexcept;
  virtual void mark(dynamic_object_heap_type&, dynamic_object_type&) const;
};


} /* end namespace gc */


} /* end namespace corevm */


#endif /* COREVM_MARK_AND_SWEEP_GARBAGE_COLLECTION_SCHEME_H_ */
