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
#ifndef COREVM_NATIVE_TYPE_VISITORS_H_
#define COREVM_NATIVE_TYPE_VISITORS_H_

#include "operators.h"
#include "types.h"
#include "visitors.impl.h"


namespace corevm {
namespace types {

// -----------------------------------------------------------------------------

/** Unary operator visitors */

typedef native_type_unary_visitor<positive> native_type_positive_visitor;
typedef native_type_unary_visitor<negation> native_type_negation_visitor;
typedef native_type_unary_visitor<increment> native_type_increment_visitor;
typedef native_type_unary_visitor<decrement> native_type_decrement_visitor;
typedef native_type_unary_visitor<bitwise_not> native_type_bitwise_not_visitor;
typedef native_type_unary_visitor<reverse> native_type_reverse_visitor;
typedef native_type_unary_visitor<absolute> native_type_abs_visitor;
typedef native_type_unary_visitor<sqrt>  native_type_sqrt_visitor;

// -----------------------------------------------------------------------------

/** Typed unary operator visitors */

typedef native_type_typed_unary_visitor<repr, string> native_type_repr_visitor;
typedef native_type_typed_unary_visitor<logical_not, boolean> native_type_logical_not_visitor;
typedef native_type_typed_unary_visitor<truthy, boolean> native_type_truthy_visitor;
typedef native_type_typed_unary_visitor<hash, int64> native_type_hash_visitor;

// -----------------------------------------------------------------------------

/** Parameterized unary operator visitors */

typedef native_type_parameterized_unary_visitor<slice> native_type_slice_visitor;
typedef native_type_parameterized_unary_visitor<stride> native_type_stride_visitor;

// -----------------------------------------------------------------------------

/** Binary operator visitors */

typedef native_type_binary_visitor<addition> native_type_addition_visitor;
typedef native_type_binary_visitor<subtraction> native_type_subtraction_visitor;
typedef native_type_binary_visitor<multiplication> native_type_multiplication_visitor;
typedef native_type_binary_visitor<division> native_type_division_visitor;
typedef native_type_binary_visitor<modulus> native_type_modulus_visitor;
typedef native_type_binary_visitor<pow_op> native_type_pow_visitor;

// -----------------------------------------------------------------------------

/** Typed cast binary operator visitors */

typedef native_type_typed_cast_binary_visitor<bitwise_and, int64> native_type_bitwise_and_visitor;
typedef native_type_typed_cast_binary_visitor<bitwise_or, int64> native_type_bitwise_or_visitor;
typedef native_type_typed_cast_binary_visitor<bitwise_xor, int64> native_type_bitwise_xor_visitor;
typedef native_type_typed_cast_binary_visitor<bitwise_left_shift, int64> native_type_bitwise_left_shift_visitor;
typedef native_type_typed_cast_binary_visitor<bitwise_right_shift, int64> native_type_bitwise_right_shift_visitor;

// -----------------------------------------------------------------------------

/** Typed binary operator visitors */

typedef native_type_typed_binary_visitor<logical_and, boolean> native_type_logical_and_visitor;
typedef native_type_typed_binary_visitor<logical_or, boolean> native_type_logical_or_visitor;
typedef native_type_typed_binary_visitor<eq, boolean> native_type_eq_visitor;
typedef native_type_typed_binary_visitor<neq, boolean> native_type_neq_visitor;
typedef native_type_typed_binary_visitor<gt, boolean> native_type_gt_visitor;
typedef native_type_typed_binary_visitor<lt, boolean> native_type_lt_visitor;
typedef native_type_typed_binary_visitor<gte, boolean> native_type_gte_visitor;
typedef native_type_typed_binary_visitor<lte, boolean> native_type_lte_visitor;
typedef native_type_typed_binary_visitor<cmp, int32> native_type_cmp_visitor;

// -----------------------------------------------------------------------------

} /* end namespace types */
} /* end namespace corevm */


#endif /* COREVM_NATIVE_TYPE_VISITORS_H_ */
