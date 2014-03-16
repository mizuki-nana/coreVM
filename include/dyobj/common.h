#ifndef COREVM_DYOBJ_COMMON_H_
#define COREVM_DYOBJ_COMMON_H_


#include <cstdint>


// 64 MB
#define COREVM_DEFAULT_HEAP_SIZE 1024 * 1024 * 64


namespace corevm {


namespace dyobj {


typedef uint32_t attr_key;


typedef uint32_t addr;


typedef uint32_t flag;


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_COMMON_H_ */