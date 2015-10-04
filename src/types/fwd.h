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
#ifndef COREVM_TYPES_FWD_H_
#define COREVM_TYPES_FWD_H_


namespace corevm {


namespace types {


class int8;
class uint8;
class int16;
class uint16;
class int32;
class uint32;
class int64;
class uint64;
class boolean;
class decimal;
class decimal2;
class string;
class array;
class map;


namespace variant {


template<typename... Types>
class variant;


} /* end namespace variant */


typedef corevm::types::variant::variant<
  corevm::types::int8,
  corevm::types::uint8,
  corevm::types::int16,
  corevm::types::uint16,
  corevm::types::int32,
  corevm::types::uint32,
  corevm::types::int64,
  corevm::types::uint64,
  corevm::types::boolean,
  corevm::types::decimal,
  corevm::types::decimal2,
  corevm::types::string,
  corevm::types::array,
  corevm::types::map
> native_type_handle;


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_FWD_H_ */
