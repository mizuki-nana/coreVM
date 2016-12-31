%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define parser_class_name {ir_parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
# include <string>
# include "format.h"
class IRParserDriver;
}
// The parsing context.
%param { IRParserDriver& driver }
%locations
%define parse.error verbose
%initial-action
{
  // Initialize the initial location.
  if (!driver.input_file().empty())
  {
    @$.begin.filename = @$.end.filename = &driver.input_file();
  }
};
%code
{
# include "ir_parser_driver.h"
}
%token <std::string>      TYPE_NAME_I8
%token <std::string>      TYPE_NAME_UI8
%token <std::string>      TYPE_NAME_I16
%token <std::string>      TYPE_NAME_UI16
%token <std::string>      TYPE_NAME_I32
%token <std::string>      TYPE_NAME_UI32
%token <std::string>      TYPE_NAME_I64
%token <std::string>      TYPE_NAME_UI64
%token <std::string>      TYPE_NAME_SPF
%token <std::string>      TYPE_NAME_DPF
%token <std::string>      TYPE_NAME_BOOLEAN
%token <std::string>      TYPE_NAME_VOID
%token <std::string>      TYPE_NAME_STRING
%token <std::string>      TYPE_NAME_ARRAY
%token <std::string>      TYPE_NAME_STRUCTTYPE
%token <std::string>      TYPE_NAME_PTRTYPE

%token <corevm::IROpcode> OPCODE
%token <std::string>      DEF
%token <std::string>      STRUCTURE
%token <std::string>      IDENTIFIER
%token <std::string>      STRING_LITERAL
%token <int64_t>          DECIMALINTEGER
%token <double>           FLOATINGNUM
%token <std::string>      COMMA
%token <std::string>      COLON
%token <std::string>      SEMICOLON
%token <std::string>      PERCENT
%token <std::string>      STAR
%token <std::string>      POND
%token <std::string>      ASSIGN
%token <std::string>      LPAREN
%token <std::string>      RPAREN
%token <std::string>      LBRACKET
%token <std::string>      RBRACKET
%token <std::string>      LBRACE
%token <std::string>      RBRACE
%token <std::string>      LABEL
%token END                0  "end of file"

%type <corevm::IRValueType> ir_value_type_short_integer_numeric;
%type <corevm::IRValueType> ir_value_type_long_integer_numeric;
%type <corevm::IRValueType> ir_value_type_integer_numeric;
%type <corevm::IRValueType> ir_value_type_floating_numeric;
%type <corevm::IRValueType> ir_value_type_string;
%type <corevm::IRValueType> ir_value_type_array;
%type <corevm::IRValueType> ir_value_type;
%type <corevm::IRArrayValue> ir_array_value;
%type <corevm::IRValue> ir_value;
%type <std::string> identifier;
%type <corevm::IROperand> operand;
%type <std::vector<corevm::IROperand>> operand_list;
%type <corevm::IRLabel> label;
%type <std::vector<corevm::IRLabel>> label_list_core;
%type <std::vector<corevm::IRLabel>> label_list;
%type <corevm::IRInstruction> expr;
%type <corevm::IRInstruction> stmt;
%type <std::vector<corevm::IRInstruction >> stmt_group;
%type <corevm::IRBasicBlock> function_block;
%type <std::vector<corevm::IRBasicBlock>> function_block_list;
%type <corevm::IRParameter> function_arg;
%type <std::vector<corevm::IRParameter>> function_arg_list;
%type <std::vector<corevm::IRParameter>> function_arg_list_core;
%type <corevm::IRClosure> function_def;
%type <std::vector<corevm::IRClosure>> function_def_list;
%type <corevm::IRStructField> structure_field;
%type <std::vector<corevm::IRStructField>> structure_field_list;
%type <corevm::IRStructDecl> structure_def;
%type <std::vector<corevm::IRStructDecl>> structure_def_list;
%type <corevm::IRModule> input;

%debug

%{
    /**
     * TODO: Grammar currently cannot handle the following:
     *
     *     - Module metadata.
     *     - Use identifiers and string literlas in instructions.
     *
     * TODO: [COREVM-571] Refine format and grammar of Intermediate Representation
     */
%}

%%

%start input;

input
    : structure_def_list function_def_list
        {
            $$ = corevm::IRModule();
            $$.types = $1;
            $$.closures = $2;
        }
    ;

function_def_list
    :
        {
            $$ = std::vector<corevm::IRClosure>();
        }
    | function_def_list function_def
        {
            $$ = $1;
            $$.push_back($2);
        }
    ;

structure_def_list
    :
        {
            $$ = std::vector<corevm::IRStructDecl>();
        }
    | structure_def_list structure_def
        {
            $$ = $1;
            $$.push_back($2);
        }
    ;

structure_def
    : STRUCTURE IDENTIFIER LBRACE structure_field_list RBRACE
        {
            $$ = corevm::IRStructDecl();
            $$.name = $2;
            $$.fields = $4;
        }
    ;

structure_field_list
    :
        {
            $$ = std::vector<corevm::IRStructField>();
        }
    | structure_field_list structure_field
        {
            $$ = $1;
            $$.push_back($2);
        }
    ;

structure_field
    : IDENTIFIER IDENTIFIER SEMICOLON
        {
            $$ = corevm::IRStructField();
            $$.type.set_string($1);
            $$.ref_type = corevm::value;
            $$.identifier = $2;
        }
    | IDENTIFIER STAR IDENTIFIER SEMICOLON
        {
            $$ = corevm::IRStructField();
            $$.type.set_string($1);
            $$.ref_type = corevm::pointer;
            $$.identifier = $3;
        }
    | ir_value_type IDENTIFIER SEMICOLON
        {
            $$ = corevm::IRStructField();
            $$.type.set_IRValueType($1);
            $$.ref_type = corevm::value;
            $$.identifier = $2;
        }
    | ir_value_type STAR IDENTIFIER SEMICOLON
        {
            $$ = corevm::IRStructField();
            $$.type.set_IRValueType($1);
            $$.ref_type = corevm::pointer;
            $$.identifier = $3;
        }
    ;

function_def
    : DEF ir_value_type IDENTIFIER function_arg_list LBRACE function_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = $3;
            $$.parameters = $4;
            $$.blocks = $6;
            $$.ret_reftype = corevm::value;
        }
    | DEF ir_value_type STAR IDENTIFIER function_arg_list LBRACE function_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = $4;
            $$.parameters = $5;
            $$.blocks = $7;
            $$.ret_reftype = corevm::pointer;
        }
    | DEF ir_value_type IDENTIFIER function_arg_list COLON IDENTIFIER LBRACE function_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = $3;
            $$.parameters = $4;
            $$.parent.set_string($6);
            $$.blocks = $8;
            $$.ret_reftype = corevm::value;
        }
    | DEF ir_value_type STAR IDENTIFIER function_arg_list COLON IDENTIFIER LBRACE function_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = $4;
            $$.parameters = $5;
            $$.parent.set_string($7);
            $$.blocks = $9;
            $$.ret_reftype = corevm::pointer;
        }
    ;

function_arg_list
    : LPAREN RPAREN
        {
            $$ = std::vector<corevm::IRParameter>();
        }
    | LPAREN function_arg_list_core RPAREN
        {
            $$ = $2;
        }
    ;

function_arg_list_core
    : function_arg
        {
            $$ = std::vector<corevm::IRParameter>();
            $$.push_back($1);
        }
    | function_arg_list_core COMMA function_arg
        {
            $$ = $1;
            $$.push_back($3);
        }
    ;

function_arg
    : ir_value_type IDENTIFIER
        {
            $$ = corevm::IRParameter();
            $$.type = $1;
            $$.identifier = $2;
            $$.ref_type = corevm::value;
        }
    | ir_value_type STAR IDENTIFIER
        {
            $$ = corevm::IRParameter();
            $$.type = $1;
            $$.identifier = $3;
            $$.ref_type = corevm::pointer;
        }
    ;

function_block_list
    :
        {
            $$ = std::vector<corevm::IRBasicBlock>();
        }
    | function_block_list function_block
        {
            $$ = $1;
            $$.push_back($2);
        }
    ;

function_block
    : IDENTIFIER COLON stmt_group
        {
            $$ = corevm::IRBasicBlock();
            $$.label = $1;
            $$.body = $3;
        }
    ;

stmt_group
    : stmt
        {
            $$ = std::vector<corevm::IRInstruction>();
        }
    | stmt_group stmt
        {
            $$ = $1;
            $$.push_back($2);
        }
    ;

stmt
    : identifier ASSIGN expr SEMICOLON
        {
            $$ = $3;
            $$.target.set_string($1);
        }
    | expr SEMICOLON
        {
            $$ = $1;
            $$.target.set_null();
        }
    ;

expr
    // TODO: consider remove 'type' field of IRInstruction. It doesn't seem to
    // be needed/used here.
    : OPCODE
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
        }
    | OPCODE ir_value
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.opcodeval = $2;
        }
    | OPCODE ir_value COMMA operand_list
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.opcodeval = $2;
            $$.oprds = $4;
        }
    | OPCODE ir_value COMMA operand_list label_list
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.opcodeval = $2;
            $$.oprds = $4;
            $$.labels.set_array($5);
        }
    ;

label_list
    : LBRACKET label_list_core RBRACKET
        {
            $$ = $2;
        }
    ;

label_list_core
    : label
        {
            $$ = std::vector<corevm::IRLabel>();
            $$.push_back($1);
        }
    | label_list_core COMMA label
        {
            $$ = $1;
            $$.push_back($3);
        }
    ;

label
    : LABEL POND IDENTIFIER
        {
            $$ = corevm::IRLabel();
            $$.name = $3;
        }
    ;

operand_list
    : operand
        {
            $$ = std::vector<corevm::IROperand>();
            $$.push_back($1);
        }
    | operand_list operand
        {
            $$ = $1;
            $$.push_back($2);
        }
    ;

operand
    : ir_value
        {
            $$ = corevm::IROperand();
            $$.type = corevm::constant;
            $$.value.set_IRValue($1);
        }
    | identifier
        {
            $$ = corevm::IROperand();
            $$.type = corevm::ref;
            $$.value.set_string($1);
        }
    ;

identifier
    : PERCENT IDENTIFIER
        {
            $$ = $2;
        }
    ;

ir_value
    : ir_value_type_short_integer_numeric DECIMALINTEGER
        {
            $$ = corevm::IRValue();
            $$.type = $1;
            $$.value.set_int($2);
        }
    | ir_value_type_long_integer_numeric DECIMALINTEGER
        {
            $$ = corevm::IRValue();
            $$.type = $1;
            $$.value.set_long($2);
        }
    | TYPE_NAME_SPF FLOATINGNUM
        {
            $$ = corevm::IRValue();
            $$.type = corevm::spf;
            $$.value.set_float($2);
        }
    | TYPE_NAME_DPF FLOATINGNUM
        {
            $$ = corevm::IRValue();
            $$.type = corevm::dpf;
            $$.value.set_double($2);
        }
    | ir_value_type_string STRING_LITERAL
        {
            $$ = corevm::IRValue();
            $$.type = $1;
            $$.value.set_string($2);
        }
    | ir_value_type_array ir_array_value
        {
            $$ = corevm::IRValue();
            $$.type = $1;
            $$.value.set_IRArrayValue($2);
        }
    | TYPE_NAME_BOOLEAN DECIMALINTEGER
        {
            $$ = corevm::IRValue();
            $$.type = corevm::boolean;
            // TODO: consider adding "true" and "false" as tokens.
            $$.value.set_bool(static_cast<bool>($2));
        }
    ;

ir_array_value
    : LBRACKET DECIMALINTEGER STAR ir_value_type RBRACKET
        {
            $$ = corevm::IRArrayValue();
            $$.len = $2;
            $$.type = $4;
        }
    ;

ir_value_type
    : ir_value_type_integer_numeric             { $$ = $1; }
    | ir_value_type_floating_numeric            { $$ = $1; }
    | ir_value_type_string                      { $$ = $1; }
    | ir_value_type_array                       { $$ = $1; }
    | TYPE_NAME_VOID                            { $$ = corevm::voidtype;   }
    | TYPE_NAME_STRUCTTYPE                      { $$ = corevm::structtype; }
    | TYPE_NAME_PTRTYPE                         { $$ = corevm::ptrtype;    }
    | TYPE_NAME_BOOLEAN                         { $$ = corevm::boolean;    }
    ;

ir_value_type_array
    : TYPE_NAME_ARRAY                           { $$ = corevm::array; }
    ;

ir_value_type_string
    : TYPE_NAME_STRING                          { $$ = corevm::string; }
    ;

ir_value_type_floating_numeric
    : TYPE_NAME_SPF                             { $$ = corevm::spf; }
    | TYPE_NAME_DPF                             { $$ = corevm::dpf; }
    ;

ir_value_type_integer_numeric
    : ir_value_type_short_integer_numeric       { $$ = $1; }
    | ir_value_type_long_integer_numeric        { $$ = $1; }
    ;

ir_value_type_long_integer_numeric
    : TYPE_NAME_I64                             { $$ = corevm::i64;  }
    | TYPE_NAME_UI64                            { $$ = corevm::ui64; }
    ;

ir_value_type_short_integer_numeric
    : TYPE_NAME_I8                              { $$ = corevm::i8;   }
    | TYPE_NAME_UI8                             { $$ = corevm::ui8;  }
    | TYPE_NAME_I16                             { $$ = corevm::i16;  }
    | TYPE_NAME_UI16                            { $$ = corevm::ui16; }
    | TYPE_NAME_I32                             { $$ = corevm::i32;  }
    | TYPE_NAME_UI32                            { $$ = corevm::ui32; }
    ;

%%

void
yy::ir_parser::error(const location_type& l, const std::string& m)
{
  driver.error(l, m);
}
