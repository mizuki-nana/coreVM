%{ /* -*- C++ -*- */
# include <cstdio>
# include <cerrno>
# include <cstdlib>
# include "ir_parser_driver.h"
# include "ir_parser.tab.hh"
# include "format.h"
# include "format_util.h"


// The location of the current token.
static yy::location loc;
%}
%option noyywrap nounput batch debug noinput
integer ([0]|[0-9]*)
floating_number [0-9]*\.[0-9]+([eE][-+]?[0-9]+)? 
string_literal \"(\\.|[^\\"])*\"
blank [ \t]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

#type_names
"void"                                    return yy::ir_parser::make_TYPE_NAME_VOID(yytext, loc);
"bool"                                    return yy::ir_parser::make_TYPE_NAME_BOOLEAN(yytext, loc);
"i8"                                      return yy::ir_parser::make_TYPE_NAME_I8(yytext, loc);
"ui8"                                     return yy::ir_parser::make_TYPE_NAME_UI8(yytext, loc);
"i16"                                     return yy::ir_parser::make_TYPE_NAME_I16(yytext, loc);
"ui16"                                    return yy::ir_parser::make_TYPE_NAME_UI16(yytext, loc);
"i32"                                     return yy::ir_parser::make_TYPE_NAME_I32(yytext, loc);
"ui32"                                    return yy::ir_parser::make_TYPE_NAME_UI32(yytext, loc);
"i64"                                     return yy::ir_parser::make_TYPE_NAME_I64(yytext, loc);
"ui64"                                    return yy::ir_parser::make_TYPE_NAME_UI64(yytext, loc);
"spf"                                     return yy::ir_parser::make_TYPE_NAME_SPF(yytext, loc);
"dpf"                                     return yy::ir_parser::make_TYPE_NAME_DPF(yytext, loc);
"string"                                  return yy::ir_parser::make_TYPE_NAME_STRING(yytext, loc);
"object"                                  return yy::ir_parser::make_TYPE_NAME_OBJECT(yytext, loc);

#opcodes
"alloca"                                  return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"load"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"store"                                   return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"getattr"                                 return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"setattr"                                 return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"delattr"                                 return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"getelement"                              return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"putelement"                              return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"len"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"ret"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"br"                                      return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"switch2"                                 return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"pos"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"neg"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"inc"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"dec"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"add"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"sub"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"mul"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"div"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"mod"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"bnot"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"band"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"bor"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"bxor"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"bls"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"brs"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"eq"                                      return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"neq"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"gt"                                      return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"lt"                                      return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"gte"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"lte"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"lnot"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"land"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"lor"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"cmp"                                     return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);
"call"                                    return yy::ir_parser::make_OPCODE(corevm::ir::string_to_IROpcode(yytext), loc);

#keywords
"array"                                   return yy::ir_parser::make_ARRAY(yytext, loc);
"def"                                     return yy::ir_parser::make_DEF(yytext, loc);
"label"                                   return yy::ir_parser::make_LABEL(yytext, loc);
"type"                                    return yy::ir_parser::make_TYPE(yytext, loc);

#boolean
"true"                                    return yy::ir_parser::make_BOOLEAN_CONSTANT(true, loc);
"false"                                   return yy::ir_parser::make_BOOLEAN_CONSTANT(false, loc);

#identifiers
[_a-zA-Z][_a-zA-Z0-9]*                    return yy::ir_parser::make_IDENTIFIER(yytext, loc);

#string_literal
{string_literal} {
  std::string s(yytext);
  s.erase(s.begin());
  s.pop_back();
  return yy::ir_parser::make_STRING_LITERAL(s, loc);
}

#integer
{integer} {
  long int n = strtol(yytext, NULL, 10);
  if (errno == ERANGE)
  {
    n = 0;
  }
  return yy::ir_parser::make_DECIMALINTEGER(n, loc);
}

#floating_number
{floating_number} {
  double f = strtod(yytext, NULL);
  if (errno == ERANGE)
  {
    f = 0.0;
  }
  return yy::ir_parser::make_FLOATINGNUM(f, loc);
}

#colon
":"                                       return yy::ir_parser::make_COLON(yytext, loc);

#semicolon
";"                                       return yy::ir_parser::make_SEMICOLON(yytext, loc);

#percent
"%"                                       return yy::ir_parser::make_PERCENT(yytext, loc);

#star
"*"                                       return yy::ir_parser::make_STAR(yytext, loc);

#pond
"#"                                       return yy::ir_parser::make_POND(yytext, loc);

#assign
"="                                       return yy::ir_parser::make_ASSIGN(yytext, loc);

#comma
","                                       return yy::ir_parser::make_COMMA(yytext, loc);

#brackets
"("                                       return yy::ir_parser::make_LPAREN(yytext, loc);
")"                                       return yy::ir_parser::make_RPAREN(yytext, loc);

#sqaure_brackets
"["                                       return yy::ir_parser::make_LBRACKET(yytext, loc);
"]"                                       return yy::ir_parser::make_RBRACKET(yytext, loc);

#curly_braces
"{"                                       return yy::ir_parser::make_LBRACE(yytext, loc);
"}"                                       return yy::ir_parser::make_RBRACE(yytext, loc);

{blank}+                                  loc.step();
[\n]+                                     loc.lines(yyleng); loc.step();

.                                         {
                                            /* ignore bad characters */
                                          }

<<EOF>>                                   return yy::ir_parser::make_END(loc);
%%
