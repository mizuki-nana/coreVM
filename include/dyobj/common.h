#ifndef COREVM_DYOBJ_COMMON_H_
#define COREVM_DYOBJ_COMMON_H_


#include <cstdint>


// 64 MB
#define COREVM_DEFAULT_HEAP_SIZE 1024 * 1024 * 64


namespace corevm {


namespace dyobj {


typedef uint32_t attr_key;


typedef int32_t ntvhndl_key;


typedef uint32_t flag;


typedef uint32_t instr_block_key;


const ntvhndl_key NONESET_NTVHNDL_KEY = -1;


const instr_block_key NONESET_INSTR_BLOCK_KEY = -1;


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_COMMON_H_ */
