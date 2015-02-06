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
#include "../../include/frontend/signal_vector_loader.h"

#include "../../include/runtime/process.h"

#include <sneaker/testing/_unittest.h>

#include <cassert>
#include <fstream>
#include <string>


class signal_vector_loader_unittest : public ::testing::Test
{
protected:
  static const char* PATH;
  static const std::string SIGNAL_VECTOR;

  virtual void SetUp()
  {
    std::ofstream f(PATH, std::ios::binary);
    f << signal_vector_loader_unittest::SIGNAL_VECTOR;
    f.close();
  }

  virtual void TearDown()
  {
    remove(PATH);
  }

  corevm::runtime::process m_process;
};

// -----------------------------------------------------------------------------

const char* signal_vector_loader_unittest::PATH = "./example.sigvec";

// -----------------------------------------------------------------------------

const std::string signal_vector_loader_unittest::SIGNAL_VECTOR = \
  "{"
    "\"signals\": {"
      "\"SIGFPE\": ["
        "[101, 102, 103],"
        "[201, 202, 203],"
        "[301, 302, 303]"
      "],"
      "\"SIGIO\": ["
        "[108, 532, 413],"
        "[212, 209, 283],"
        "[863, 361, 913]"
      "]"
    "}"
  "}";

// -----------------------------------------------------------------------------

TEST_F(signal_vector_loader_unittest, TestLoadSuccessful)
{
  corevm::frontend::signal_vector_loader loader(PATH);

  loader.load(m_process);
}

// -----------------------------------------------------------------------------

TEST_F(signal_vector_loader_unittest, TestLoadFailsWithInvalidSignal)
{
  const std::string INVALID_SIGNAL_VECTOR = \
    "{"
      "\"signals\": {"
        "\"SIGFPE\": ["
          "[101, 102, 103],"
          "[201, 202, 203],"
          "[301, 302, 303]"
        "],"
        "\"SIGUNKNOWN\": ["
          "[108, 532, 413],"
          "[212, 209, 283],"
          "[863, 361, 913]"
        "]"
      "}"
    "}";

  const char* path = "./invalid_format.sigvec";

  std::ofstream f(path, std::ios::binary);
  f << INVALID_SIGNAL_VECTOR;
  f.close();

  corevm::frontend::signal_vector_loader loader(path);

  ASSERT_THROW(
    {
      loader.load(m_process);
    },
    corevm::frontend::file_loading_error
  );

  remove(path);
}

// -----------------------------------------------------------------------------

TEST_F(signal_vector_loader_unittest, TestLoadFailsWithInvalidPath)
{
  corevm::frontend::signal_vector_loader loader("some/random/path/example.sigvec");

  ASSERT_THROW(
    {
      loader.load(m_process);
    },
    corevm::frontend::file_loading_error
  );
}

// -----------------------------------------------------------------------------
