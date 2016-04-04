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
#ifndef COREVM_NATIVE_TYPE_HANDLE_H_
#define COREVM_NATIVE_TYPE_HANDLE_H_

#include "visitors.h"
#include "variant/variant.h"


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

template<typename T>
T
get_value_from_handle(NativeTypeHandle& handle)
{
  return variant::apply_visitor(
    native_type_value_visitor<T>(), handle
  );
}

// -----------------------------------------------------------------------------

template<typename T>
T&
get_value_ref_from_handle(NativeTypeHandle& handle)
{
  return handle.get<T>();
}

// -----------------------------------------------------------------------------

template<class operator_visitor>
NativeTypeHandle
apply_unary_visitor(NativeTypeHandle& handle)
{
  return variant::apply_visitor(operator_visitor(), handle);
}

// -----------------------------------------------------------------------------

template<class operator_visitor, typename... Arguments>
NativeTypeHandle
apply_unary_visitor_parameterized(NativeTypeHandle& handle, Arguments... args)
{
  return variant::apply_visitor(operator_visitor(args...), handle);
}

// -----------------------------------------------------------------------------

template<class operator_visitor>
NativeTypeHandle
apply_binary_visitor(
  NativeTypeHandle& lhs, NativeTypeHandle& rhs)
{
  return variant::apply_visitor(operator_visitor(), lhs, rhs);
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_TYPE_HANDLE_H_ */
