#ifndef COREVM_GARBAGE_COLLECTION_SCHEME_H_
#define COREVM_GARBAGE_COLLECTION_SCHEME_H_


#include "../dyobj/dynamic_object.h"
#include "../dyobj/dynamic_object_heap.h"
#include "../dyobj/dynamic_object_manager.h"


namespace corevm {


namespace gc {


class garbage_collection_scheme {};


class reference_count_garbage_collection_scheme : public garbage_collection_scheme {
public:
  typedef class dynamic_object_manager : public corevm::dyobj::dynamic_object_manager {
    public:
      explicit dynamic_object_manager(): _count(0) {}

      virtual inline bool garbage_collectible() const noexcept {
        return _count <= 1;
      }

      virtual inline void on_create() noexcept {
        this->inc_ref_count();
      }

      virtual inline void on_setattr() noexcept {
        this->inc_ref_count();
      }

      virtual inline void on_delattr() noexcept {
        this->dec_ref_count();
      }

      virtual inline uint64_t ref_count() const noexcept {
        return _count;
      }

      virtual inline void inc_ref_count() noexcept {
        _count++;
      }

      virtual inline void dec_ref_count() noexcept {
        _count--;
      }

    protected:
      uint64_t _count;
  } reference_count_dynamic_object_manager;

  using dynamic_object_type = typename corevm::dyobj::dynamic_object<reference_count_dynamic_object_manager>;
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<reference_count_dynamic_object_manager>;

  virtual void gc(dynamic_object_heap_type&) const;

protected:
  void check_and_dec_ref_count(dynamic_object_heap_type&, dynamic_object_type&) const;
};


class mark_and_sweep_garbage_collection_scheme : public garbage_collection_scheme {
public:
  typedef class dynamic_object_manager : public corevm::dyobj::dynamic_object_manager {
    public:
      explicit dynamic_object_manager(): _marked(false) {}

      virtual inline bool garbage_collectible() const noexcept {
        return marked();
      }

      virtual inline void on_create() noexcept {
        // Do nothing here.
      }

      virtual inline void on_setattr() noexcept {
        // Do nothing here.
      }

      virtual inline void on_delattr() noexcept {
        // Do nothing here.
      }

      virtual inline bool marked() const noexcept {
        return _marked;
      }

      virtual inline void mark() noexcept {
        _marked = true;
      }

      virtual inline void unmark() noexcept {
        _marked = false;
      }

    protected:
      bool _marked;
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


#endif /* COREVM_GARBAGE_COLLECTION_SCHEME_H_ */
