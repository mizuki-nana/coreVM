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
#include "bytecode_loader_unittest.h"

#include "../../include/frontend/bytecode_loader_v0_1.h"
#include "../../include/runtime/process.h"

#include <sneaker/testing/_unittest.h>

#include <string>


class bytecode_loader_v0_1_unittest : public bytecode_loader_unittest
{
protected:
  virtual const char* bytecode()
  {
    return \
    "{"
      "\"format\": \"application/json\","
      "\"format-version\": \"0.1\","
      "\"target-version\": \"0.1\","
      "\"path\": \"./example.corevm\","
      "\"timestamp\": \"2014-10-12T15:33:30\","
      "\"encoding\": \"utf8\","
      "\"author\": \"Yanzheng Li\","
      "\"encoding_map\": ["
        "{"
          "\"key\": \"name\","
          "\"value\": 111"
        "}"
      "],"
      "\"__MAIN__\": ["
        "{"
          "\"__name__\": \"hello_world\","
          "\"__vector__\": ["
            "[701, 702, 703],"
            "[801, 802, 803],"
            "[901, 902, 903]"
          "],"
          "\"__parent__\": \"hello_world_decorator\""
        "},"
        "{"
          "\"__name__\": \"hello_world_decorator\","
          "\"__vector__\": ["
            "[701, 702, 703],"
            "[801, 802, 803],"
            "[901, 902, 903]"
          "],"
          "\"__parent__\": \"__main__\""
        "},"
        "{"
          "\"__name__\": \"__main__\","
          "\"__vector__\": ["
            "[701, 702, 703],"
            "[801, 802, 803],"
            "[901, 902, 903]"
          "]"
        "}"
      "]"
    "}";
  }
};


TEST_F(bytecode_loader_v0_1_unittest, TestLoadSuccessful)
{
  corevm::runtime::process process;

  corevm::frontend::bytecode_loader::load(PATH, process);
}
