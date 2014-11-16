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
    m_block(instrs)
  {
  }

  explicit instr_block(std::initializer_list<corevm::runtime::instr> instrs):
    m_block(instrs)
  {
  }

  instr_block& operator=(const instr_block& other) {
    m_block = other.block();
    return *this;
  }

  block_type block() const {
    return m_block;
  }

  block_type::iterator begin() {
    return m_block.begin();
  }

  block_type::iterator end() {
    return m_block.end();
  }

private:
  block_type m_block;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_INSTR_BLOCK_H_ */
