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
#include "ir/reader.h"
#include "ir/format.h"

#include <sneaker/utility/cmdline_program.h>

#include <cstdio>
#include <string>


class IRStats : public sneaker::utility::cmdline_program
{
public:
  IRStats();

  virtual ~IRStats() {}

protected:
  virtual int do_run();

  virtual bool check_parameters() const;

private:
  void print_stats(const corevm::IRModule&);

  void print_stats(const corevm::IRModuleMeta&);

  void print_stats(const corevm::IRStructDecl&);

  void print_stats(const corevm::IRClosure&);

  std::string m_input;

  struct Stats
  {
    Stats()
      :
      total_instruction_count(0)
    {
    }

    void visit(const corevm::IRBasicBlock& block)
    {
      total_instruction_count += block.body.size();
    }

    size_t total_instruction_count;
  };

  Stats m_stats;
};

// -----------------------------------------------------------------------------

IRStats::IRStats()
  :
  sneaker::utility::cmdline_program("Print stats of coreVM IR"),
  m_input(),
  m_stats()
{
  add_positional_parameter("input", 1);
  add_string_parameter("input", "input file", &m_input);
}

// -----------------------------------------------------------------------------

bool
IRStats::check_parameters() const
{
  return !m_input.empty();
}

// -----------------------------------------------------------------------------

int
IRStats::do_run()
{
  corevm::IRModule module;
  std::string err;

  const bool res = corevm::ir::read_module_from_file(
    m_input.c_str(), module, err);

  if (!res)
  {
    printf("Error while reading from %s :\n", m_input.c_str());
    printf("%s\n", err.c_str());
    return -1;
  }

  print_stats(module);

  return 0;
}

// -----------------------------------------------------------------------------

void
IRStats::print_stats(const corevm::IRModule& module)
{
  print_stats(module.meta);
  printf("\n");

  printf("Struct decls: %ld\n", module.types.size());
  for (const auto& type : module.types)
  {
    print_stats(type);
  }
  printf("\n");

  printf("Closures: %ld\n", module.closures.size());
  for (const auto& closure : module.closures)
  {
    print_stats(closure);
  }
  printf("\n");

  printf("Total instruction count: %ld\n", m_stats.total_instruction_count);
}

// -----------------------------------------------------------------------------

void
IRStats::print_stats(const corevm::IRModuleMeta& meta)
{
  printf("Module name: %s\n", meta.name.c_str());
  printf("Format version: %s\n", meta.format_version.c_str());
  printf("Target version: %s\n", meta.target_version.c_str());
  printf("Path: %s\n", meta.path.c_str());
  printf("Author: %s\n", meta.author.c_str());
  printf("Timestamp: %lld\n", meta.timestamp);
}

// -----------------------------------------------------------------------------

void
IRStats::print_stats(const corevm::IRStructDecl& decl)
{
  printf("Struct decl: %s\n", decl.name.c_str());
  printf("\tFields: %ld\n", decl.fields.size());
}

// -----------------------------------------------------------------------------

void
IRStats::print_stats(const corevm::IRClosure& closure)
{
  printf("Closure: %s\n", closure.name.c_str());
  printf("\tBlocks: %ld\n", closure.blocks.size());

  for (const auto& block : closure.blocks)
  {
    m_stats.visit(block);
  }
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  IRStats program;
  return program.run(argc, argv);
}

// -----------------------------------------------------------------------------

