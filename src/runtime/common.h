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
#ifndef COREVM_RUNTIME_COMMON_H_
#define COREVM_RUNTIME_COMMON_H_

#include <cstdint>
#include <string>
#include <unordered_map>


namespace corevm {


namespace runtime {


const uint32_t COREVM_DEFAULT_GC_INTERVAL = 10;


typedef int32_t instr_addr;


typedef uint16_t instr_code;


typedef uint64_t instr_oprd;


typedef int32_t variable_key;


typedef uint8_t gc_bitfield_t;


typedef int64_t closure_id;


typedef int32_t compartment_id;


const compartment_id NONESET_COMPARTMENT_ID = -1;


const closure_id NONESET_CLOSURE_ID = -1;


const instr_addr NONESET_INSTR_ADDR = -1;


typedef std::unordered_map<uint64_t, std::string> encoding_map;


// Default size of native types pool: 128 MB.
const uint64_t COREVM_DEFAULT_NATIVE_TYPES_POOL_SIZE = 1024 * 1024 * 128;


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_COMMON_H_ */
