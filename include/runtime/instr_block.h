#ifndef COREVM_INSTR_BLOCK_H_
#define COREVM_INSTR_BLOCK_H_

#include <vector>
#include "instr.h"


namespace corevm {


namespace runtime {


class instr_block {
public:
  explicit instr_block(std::vector<corevm::runtime::instr>& instrs):
    _block(instrs)
  {}

  instr_block& operator=(const instr_block& other) {
    _block = other._block;
    return *this;
  }

  std::vector<corevm::runtime::instr> block() const {
    return _block;
  }

private:
  std::vector<corevm::runtime::instr> _block;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTR_BLOCK_H_ */
