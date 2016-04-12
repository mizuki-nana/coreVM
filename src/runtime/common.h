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
#ifndef COREVM_RUNTIME_COMMON_H_
#define COREVM_RUNTIME_COMMON_H_

#include <cstdint>
#include <string>
#include <vector>


namespace corevm {


namespace runtime {


const uint32_t COREVM_DEFAULT_GC_INTERVAL = 10;


typedef int32_t instr_addr_t;


typedef uint16_t instr_code_t;


typedef uint64_t instr_oprd_t;


typedef int32_t variable_key_t;


typedef uint8_t gc_bitfield_t;


typedef int64_t closure_id_t;


typedef uint64_t encoding_key_t;


typedef std::vector<std::string> EncodingMap;


typedef int32_t compartment_id_t;


const compartment_id_t NONESET_COMPARTMENT_ID = -1;


const closure_id_t NONESET_CLOSURE_ID = -1;


const instr_addr_t NONESET_INSTR_ADDR = -1;


// Default size of native types pool: 256 MB.
const uint64_t COREVM_DEFAULT_NATIVE_TYPES_POOL_SIZE = 1024 * 1024 * 256;


// Default number of stacks to unwind on failures.
const size_t COREVM_DEFAULT_STACK_UNWIND_COUNT = 5;


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_COMMON_H_ */
