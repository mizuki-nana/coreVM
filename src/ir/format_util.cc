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
#include "format_util.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>


// -----------------------------------------------------------------------------

corevm::IROpcode
string_to_IROpcode(const std::string& val)
{
  if (val == "alloca")
  {
    return corevm::alloca;
  }
  else if (val == "load")
  {
    return corevm::load;
  }
  else if (val == "store")
  {
    return corevm::store;
  }
  else if (val == "getattr")
  {
    return corevm::getattr;
  }
  else if (val == "setattr")
  {
    return corevm::setattr;
  }
  else if (val == "delattr")
  {
    return corevm::delattr;
  }
  else if (val == "getelement")
  {
    return corevm::getelement;
  }
  else if (val == "putelement")
  {
    return corevm::putelement;
  }
  else if (val == "len")
  {
    return corevm::len;
  }
  else if (val == "ret")
  {
    return corevm::ret;
  }
  else if (val == "br")
  {
    return corevm::br;
  }
  else if (val == "switch2")
  {
    return corevm::switch2;
  }
  else if (val == "pos")
  {
    return corevm::pos;
  }
  else if (val == "neg")
  {
    return corevm::neg;
  }
  else if (val == "inc")
  {
    return corevm::inc;
  }
  else if (val == "dec")
  {
    return corevm::dec;
  }
  else if (val == "add")
  {
    return corevm::add;
  }
  else if (val == "sub")
  {
    return corevm::sub;
  }
  else if (val == "mul")
  {
    return corevm::mul;
  }
  else if (val == "div")
  {
    return corevm::div;
  }
  else if (val == "mod")
  {
    return corevm::mod;
  }
  else if (val == "bnot")
  {
    return corevm::bnot;
  }
  else if (val == "band")
  {
    return corevm::band;
  }
  else if (val == "bor")
  {
    return corevm::bor;
  }
  else if (val == "bxor")
  {
    return corevm::bxor;
  }
  else if (val == "bls")
  {
    return corevm::bls;
  }
  else if (val == "brs")
  {
    return corevm::brs;
  }
  else if (val == "eq")
  {
    return corevm::eq;
  }
  else if (val == "neq")
  {
    return corevm::neq;
  }
  else if (val == "gt")
  {
    return corevm::gt;
  }
  else if (val == "lt")
  {
    return corevm::lt;
  }
  else if (val == "gte")
  {
    return corevm::gte;
  }
  else if (val == "lte")
  {
    return corevm::lte;
  }
  else if (val == "lnot")
  {
    return corevm::lnot;
  }
  else if (val == "land")
  {
    return corevm::land;
  }
  else if (val == "lor")
  {
    return corevm::lor;
  }
  else if (val == "cmp")
  {
    return corevm::cmp;
  }
  else if (val == "call")
  {
    return corevm::call;
  }

  assert(0);
  return static_cast<corevm::IROpcode>(0);
}

// -----------------------------------------------------------------------------

const char* IROpcode_to_string(corevm::IROpcode val)
{
  static const char* OPCODE_STRS[] {
    "alloca",
    "load",
    "store",
    "getattr",
    "setattr",
    "delattr",
    "getelement",
    "putelement",
    "len",
    "ret",
    "br",
    "switch2",
    "pos",
    "neg",
    "inc",
    "dec",
    "add",
    "sub",
    "mul",
    "div",
    "mod",
    "bnot",
    "band",
    "bor",
    "bxor",
    "bls",
    "brs",
    "eq",
    "neq",
    "gt",
    "lt",
    "gte",
    "lte",
    "lnot",
    "land",
    "lor",
    "cmp",
    "call"
  };

  return OPCODE_STRS[val];
}

// -----------------------------------------------------------------------------

void set_metadata(const MetadataPair& pair, corevm::IRModule& module)
{
  const auto& key = pair.first;
  const auto& value = pair.second;

  if (key == "module name")
  {
    module.meta.name = value;
  }
  else if (key == "format version")
  {
    module.meta.format_version = std::stoll(value);
  }
  else if (key == "target version")
  {
    module.meta.target_version = std::stoll(value);
  }
  else if (key == "path")
  {
    module.meta.path = value;
  }
  else if (key == "author")
  {
    module.meta.author = value;
  }
  else if (key == "timestamp")
  {
    module.meta.timestamp = std::stoll(value);
  }
}

// -----------------------------------------------------------------------------

void
set_metadata(const std::vector<MetadataPair>& metadata,
  corevm::IRModule& module)
{
  for (const auto& pair : metadata)
  {
    set_metadata(pair, module);
  }
}

// -----------------------------------------------------------------------------
