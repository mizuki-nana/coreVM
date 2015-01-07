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
#ifndef COREVM_INSTR_BLOCK_H_
#define COREVM_INSTR_BLOCK_H_

#include "instr.h"

#include <vector>


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
