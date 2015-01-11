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
#include "../../include/frontend/bytecode_runner.h"

#include "../../include/runtime/instr_block.h"
#include "../../include/runtime/instr.h"

#include <sneaker/json/json.h>

#include <cassert>


corevm::runtime::instr_block
corevm::frontend::bytecode_runner::get_vector_from_json(const JSON& json)
{
    assert(json.type() == JSON::ARRAY);

    const JSON::array& vector = json.array_items();
    corevm::runtime::instr_block::block_type block;

    for (auto itr = vector.begin(); itr != vector.end(); ++itr) {
        const JSON& instr_json = *itr;
        const JSON::array& instr_tuple = instr_json.array_items();

        corevm::runtime::instr_code code = 0;
        corevm::runtime::instr_oprd oprd1 = 0;
        corevm::runtime::instr_oprd oprd2 = 0;

        const JSON& code_raw = instr_tuple[0];
        code = static_cast<corevm::runtime::instr_code>(code_raw.int_value());

        if (instr_tuple.size() >= 2) {
          const JSON& oprd1_raw = instr_tuple[1];
          oprd1 = static_cast<corevm::runtime::instr_oprd>(oprd1_raw.int_value());
        }

        if (instr_tuple.size() == 3) {
          const JSON& oprd2_raw = instr_tuple[2];
          oprd2 = static_cast<corevm::runtime::instr_oprd>(oprd2_raw.int_value());
        }

        block.push_back(corevm::runtime::instr
          {
            .code = code,
            .oprd1 = oprd1,
            .oprd2 = oprd2,
          }
        );
    }

    return corevm::runtime::instr_block(block);
}
