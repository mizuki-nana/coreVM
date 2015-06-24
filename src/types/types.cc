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
#include "types.h"

#include <boost/type_traits.hpp>

// -----------------------------------------------------------------------------

/**
 * Static assertions to check if all the native types are
 * nothrow default-constructible. This is to enable the underlying optimizations
 * in `boost::variant`'s implementation to avoid the performance penalty of
 * achieving the "never-empty guarantee".
 *
 * "If any bounded type is nothrow default-constructible (as indicated by
 * boost::has_nothrow_constructor), the library guarantees variant will use
 * only single storage and in-place construction for every bounded type in the
 * variant. Note, however, that in the event of assignment failure, an
 * unspecified nothrow default-constructible bounded type will be
 * default-constructed in the left-hand side operand so as to preserve the
 * never-empty guarantee".
 *
 * Please refer to the following link for more details:
 * http://www.boost.org/doc/libs/1_58_0/doc/html/variant/design.html#variant.design.never-empty.optimizations
 */
static_assert(boost::has_nothrow_constructor<corevm::types::int8>::value, "types::int8 does not statisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint8>::value, "types::uint8 does not statisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::int16>::value, "types::int16 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint16>::value, "types::uint16 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::int32>::value, "types::int32 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint32>::value, "types::uint32 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::int64>::value, "types::int64 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint64>::value, "types::uint64 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::boolean>::value, "types::boolean does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::decimal>::value, "types::decimal does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::decimal2>::value, "types::decimal2 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::string>::value, "types::string does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::array>::value, "types::array does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::map>::value, "types::map does not satisfy nothrow default-constructible requirement");

// -----------------------------------------------------------------------------

/**
 * Static assertions to check if all the native types are
 * nothrow copy-constructible. This is to enable the underlying optimizations
 * in `boost::variant`'s implementation to avoid the performance penalty of
 * achieving the "never-empty guarantee".
 *
 * "For each bounded type T that is nothrow copy-constructible
 * (as indicated by boost::has_nothrow_copy), the library guarantees variant
 * will use only single storage and in-place construction for T".
 *
 * Please refer to the following link for more details:
 * http://www.boost.org/doc/libs/1_58_0/doc/html/variant/design.html#variant.design.never-empty.optimizations
 *
 * NOTE: Currently the native string, array and map types are not nothrow
 * copy-constructible.
 */
static_assert(boost::has_nothrow_copy<corevm::types::int8>::value, "types::int8 does not statisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint8>::value, "types::uint8 does not statisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::int16>::value, "types::int16 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint16>::value, "types::uint16 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::int32>::value, "types::int32 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint32>::value, "types::uint32 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::int64>::value, "types::int64 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint64>::value, "types::uint64 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::boolean>::value, "types::boolean does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::decimal>::value, "types::decimal does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::decimal2>::value, "types::decimal2 does not satisfy nothrow copy-constructible requirement");
//static_assert(boost::has_nothrow_copy<corevm::types::string>::value, "types::string does not satisfy nothrow copy-constructible requirement");
//static_assert(boost::has_nothrow_copy<corevm::types::array>::value, "types::array does not satisfy nothrow copy-constructible requirement");
//static_assert(boost::has_nothrow_copy<corevm::types::map>::value, "types::map does not satisfy nothrow copy-constructible requirement");

// -----------------------------------------------------------------------------
