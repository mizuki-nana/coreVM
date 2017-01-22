# coreVM Intermediate Representation

This directory contains code relating to the coreVM intermediate
representation.

## Steps and environment used to generate lexer and parser files.

### Environment
Operating System:
`OS X 10.11.6 (15G1217)`

Flex:
`flex 2.6.2` ([Link](https://github.com/westes/flex/releases/tag/v2.6.2))

Bison:
`bison (GNU Bison) 3.0.4` ([Link](http://ftp.gnu.org/gnu/bison/bison-3.0.4.tar.gz))

Compiler (Clang):
`Apple LLVM version 7.3.0 (clang-703.0.31)`
`Target: x86_64-apple-darwin15.6.0`
`Thread model: posix`

### Steps

  * `cd src/ir/`
  * `/usr/local/bin/flex -o lex.yy.cc --header-file=lex.yy.hh ir_lexer.ll`
  * `bison --language=c++ --verbose --warnings=error ir_parser.yy`

**NOTE**: It's important to know that DO NOT compile lexer file in C++ mode
(i.e. by supplying cmdl argument --c++), but rather save the output of
flex as a C++ source file.
