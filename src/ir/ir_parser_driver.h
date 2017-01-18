/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#ifndef COREVM_IR_PARSER_DRIVER
#define COREVM_IR_PARSER_DRIVER

#include "ir_parser.tab.hh"
#include <string>

// Tell Flex the lexer's prototype ...
#define YY_DECL yy::ir_parser::symbol_type yylex (corevm::ir::IRParserDriver& driver, void* yyscanner)
// ... and declare it for the parser's sake.
YY_DECL;


namespace corevm {
namespace ir {

class IRParserDriver
{
public:
  IRParserDriver();
  virtual ~IRParserDriver();

  bool trace_parsing() const;
  void set_trace_parsing(bool);

  // Run the parser on input file.
  // Return 0 on success.
  int parse_from_file(const std::string& filepath);

  // Run the parser on input string.
  // Return 0 on success.
  int parse_from_string(const char*);

  // The name of the file being parsed.
  // Used later to pass the file name to the location tracker.
  const std::string& input_file() const;
  std::string& input_file();

  // Error handling.
  void error(const yy::location& l, const std::string& m);
  void error(const std::string& m);

  void set_module(corevm::IRModule&&);

  const corevm::IRModule& module() const;
private:
  // Whether parser traces should be generated.
  bool m_trace_parsing;

  std::string m_input_file;

  corevm::IRModule m_module;
};

} /* end namespace ir */
} /* end namespace corevm */

#endif /* COREVM_IR_PARSER_DRIVER */
