#ifndef COREVM_DYNAMIC_OBJECT_MANAGER_H_
#define COREVM_DYNAMIC_OBJECT_MANAGER_H_


namespace corevm {


namespace dyobj {


class dynamic_object_manager {
public:
  virtual bool garbage_collectible() const noexcept = 0;

  virtual void on_create() noexcept = 0;

  virtual void on_setattr() noexcept = 0;

  virtual void on_delattr() noexcept = 0;
};


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_MANAGER_H_ */
