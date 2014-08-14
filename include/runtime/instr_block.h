#ifndef COREVM_INSTR_BLOCK_H_
#define COREVM_INSTR_BLOCK_H_

#include <vector>
#include "instr.h"


namespace corevm {


namespace runtime {


class instr_block {
public:
  using block_type = typename std::vector<corevm::runtime::instr>;

  explicit instr_block(block_type& instrs):
    _block(instrs)
  {}

  explicit instr_block(std::initializer_list<corevm::runtime::instr> instrs):
    _block(instrs)
  {}

  instr_block& operator=(const instr_block& other) {
    _block = other._block;
    return *this;
  }

  block_type block() const {
    return _block;
  }

  block_type::iterator begin() {
    return _block.begin();
  }

  block_type::iterator end() {
    return _block.end();
  }

private:
  block_type _block;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTR_BLOCK_H_ */
