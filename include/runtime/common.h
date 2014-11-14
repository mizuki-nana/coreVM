#ifndef COREVM_RUNTIME_COMMON_H_
#define COREVM_RUNTIME_COMMON_H_

#include <stdint.h>


namespace corevm {


namespace runtime {


typedef int32_t instr_addr;


typedef uint16_t instr_code;


typedef uint64_t instr_oprd;


typedef int32_t variable_key;


typedef uint8_t gc_bitfield_t;


const instr_addr NONESET_INSTR_ADDR = -1;


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_COMMON_H_ */
