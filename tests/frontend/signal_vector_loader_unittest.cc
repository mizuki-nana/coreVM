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
#include "frontend/signal_vector_loader.h"

#include "runtime/process.h"

#include <gtest/gtest.h>

#include <cassert>
#include <fstream>
#include <string>


class SignalVectorLoaderUnitTest : public ::testing::Test
{
protected:
  static const char* PATH;
  static const std::string SIGNAL_VECTOR;

  virtual void SetUp()
  {
    std::ofstream f(PATH, std::ios::binary);
    f << SignalVectorLoaderUnitTest::SIGNAL_VECTOR;
    f.close();
  }

  virtual void TearDown()
  {
    remove(PATH);
  }

  corevm::runtime::Process m_process;
};

// -----------------------------------------------------------------------------

const char* SignalVectorLoaderUnitTest::PATH = "./example.sigvec";

// -----------------------------------------------------------------------------

const std::string SignalVectorLoaderUnitTest::SIGNAL_VECTOR =
  "{"
    "\"signals\": {"
      "\"SIGFPE\": ["
        "[1, 102, 103],"
        "[2, 202, 203],"
        "[3, 302, 303]"
      "],"
      "\"SIGIO\": ["
        "[1, 532, 413],"
        "[2, 209, 283],"
        "[8, 361, 913]"
      "]"
    "}"
  "}";

// -----------------------------------------------------------------------------

TEST_F(SignalVectorLoaderUnitTest, TestLoadSuccessful)
{
  corevm::frontend::SignalVectorLoader loader(PATH);

  loader.load(m_process);
}

// -----------------------------------------------------------------------------

TEST_F(SignalVectorLoaderUnitTest, TestLoadFailsWithInvalidSignal)
{
  const std::string INVALID_SIGNAL_VECTOR =
    "{"
      "\"signals\": {"
        "\"SIGFPE\": ["
          "[1, 102, 103],"
          "[2, 202, 203],"
          "[3, 302, 303]"
        "],"
        "\"SIGUNKNOWN\": ["
          "[1, 532, 413],"
          "[2, 209, 283],"
          "[8, 361, 913]"
        "]"
      "}"
    "}";

  const char* path = "./invalid_format.sigvec";

  std::ofstream f(path, std::ios::binary);
  f << INVALID_SIGNAL_VECTOR;
  f.close();

  corevm::frontend::SignalVectorLoader loader(path);

  ASSERT_THROW(
    {
      loader.load(m_process);
    },
    corevm::frontend::FileLoadingError
  );

  remove(path);
}

// -----------------------------------------------------------------------------

TEST_F(SignalVectorLoaderUnitTest, TestLoadFailsWithInvalidPath)
{
  corevm::frontend::SignalVectorLoader loader("some/random/path/example.sigvec");

  ASSERT_THROW(
    {
      loader.load(m_process);
    },
    corevm::frontend::FileLoadingError
  );
}

// -----------------------------------------------------------------------------
