#ifndef COREVM_DYNAMIC_OBJECT_MANAGER_H_
#define COREVM_DYNAMIC_OBJECT_MANAGER_H_


namespace corevm {


namespace dyobj {


class dynamic_object_manager {
public:
  virtual bool garbage_collectible() const noexcept = 0;

  /*
   * Invoked when the associated object is first created.
   */
  virtual void on_create() noexcept = 0;

  /*
   * Invoked when the associated object is being set as an attribute of
   * another object.
   */
  virtual void on_setattr() noexcept = 0;

  /*
   * Invoked when the associated object is set to no longer be an attribute
   * of another object.
   */
  virtual void on_delattr() noexcept = 0;

  /*
   * Invoked when the associated object is being explicitly deleted from the
   * current scope.
   */
  virtual void on_delete() noexcept = 0;

  /*
   * Invoked when the associated object is exiting the containing scope.
   */
  virtual void on_exit() noexcept = 0;
};


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_MANAGER_H_ */
