/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
