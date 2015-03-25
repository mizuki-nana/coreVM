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
#include "bytecode_loader_unittest_base.h"

#include "frontend/bytecode_loader_v0_1.h"
#include "runtime/process.h"

#include <sneaker/testing/_unittest.h>

#include <string>


class bytecode_loader_v0_1_unittest : public bytecode_loader_unittest_base
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
          "\"__id__\": 2,"
          "\"__vector__\": ["
            "[7, 702, 703],"
            "[8, 802, 803],"
            "[9, 902, 903]"
          "],"
          "\"__parent__\": 1"
        "},"
        "{"
          "\"__id__\": 1,"
          "\"__vector__\": ["
            "[7, 702, 703],"
            "[8, 802, 803],"
            "[9, 902, 903]"
          "],"
          "\"__parent__\": 0"
        "},"
        "{"
          "\"__id__\": 0,"
          "\"__vector__\": ["
            "[7, 702, 703],"
            "[8, 802, 803],"
            "[9, 902, 903]"
          "]"
        "}"
      "]"
    "}";
  }
};

// -----------------------------------------------------------------------------

TEST_F(bytecode_loader_v0_1_unittest, TestLoadSuccessful)
{
  corevm::runtime::process process;

  corevm::frontend::bytecode_loader::load(PATH, process);
}

// -----------------------------------------------------------------------------

class bytecode_loader_v0_1_invalid_instr_unittest : public bytecode_loader_unittest_base
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
          "\"__id__\": 0,"
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

// -----------------------------------------------------------------------------

TEST_F(bytecode_loader_v0_1_invalid_instr_unittest, TestLoadFails)
{
  corevm::runtime::process process;

  ASSERT_THROW(
    {
      corevm::frontend::bytecode_loader::load(PATH, process);
    },
    corevm::frontend::file_loading_error
  );
}

// -----------------------------------------------------------------------------
