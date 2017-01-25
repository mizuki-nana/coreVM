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
# include <utility>
# include "format.h"
# include "format_util.h"
namespace corevm {
namespace ir {
class IRParserDriver;
} /* end namespace ir */
} /* end namespace corevm */
}
// The parsing context.
%param { corevm::ir::IRParserDriver& driver }
%param { void* yyscanner }
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
%token <std::string>      TYPE_NAME_OBJECT

%token <corevm::IROpcode> OPCODE
%token <std::string>      ARRAY
%token <std::string>      DEF
%token <std::string>      TYPE
%token <std::string>      IDENTIFIER
%token <std::string>      STRING_LITERAL
%token <int64_t>          DECIMALINTEGER
%token <double>           FLOATINGNUM
%token <bool>             BOOLEAN_CONSTANT
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
%type <corevm::IRValueType> ir_value_type;
%type <corevm::IRValue> ir_value;
%type <corevm::IRArrayType> ir_value_type_array;
%type <corevm::IRIdentifierType> ir_identifier_type;
%type <std::string> identifier;
%type <corevm::IROperand> operand;
%type <std::vector<corevm::IROperand>> operand_list;
%type <corevm::IRLabel> label;
%type <std::vector<corevm::IRLabel>> label_list_core;
%type <std::vector<corevm::IRLabel>> label_list;
%type <std::vector<std::string>> instruction_option_list;
%type <std::vector<std::string>> instruction_options;
%type <corevm::IRInstruction> expr;
%type <corevm::IRInstruction> instruction;
%type <std::vector<corevm::IRInstruction >> instruction_group;
%type <corevm::IRBasicBlock> basic_block;
%type <std::vector<corevm::IRBasicBlock>> basic_block_list;
%type <corevm::IRParameter> function_arg;
%type <std::vector<corevm::IRParameter>> function_arg_list;
%type <std::vector<corevm::IRParameter>> function_arg_list_core;
%type <corevm::IRClosure> function_def;
%type <std::vector<corevm::IRClosure>> function_def_list;
%type <corevm::IRTypeField> type_field;
%type <std::vector<corevm::IRTypeField>> type_field_list;
%type <corevm::IRTypeDecl> type_def;
%type <std::vector<corevm::IRTypeDecl>> type_def_list;
%type <corevm::ir::MetadataPair> metadata_def;
%type <std::vector<corevm::ir::MetadataPair>> metadata_def_list;
%type <corevm::IRModule> input;

%debug

%%

%start input;

input
    : metadata_def_list type_def_list function_def_list
        {
            $$ = corevm::IRModule();
            $$.types = std::move($2);
            $$.closures = std::move($3);
            corevm::ir::set_metadata($1, $$);
            driver.set_module(std::move($$));
        }
    ;

metadata_def_list
    :
        {
            $$ = std::vector<corevm::ir::MetadataPair>();
        }
    |   metadata_def_list metadata_def
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

metadata_def
    : STRING_LITERAL COLON STRING_LITERAL
        {
            $$ = std::make_pair($1, $3);      
        }
    ;

function_def_list
    :
        {
            $$ = std::vector<corevm::IRClosure>();
        }
    | function_def_list function_def
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

type_def_list
    :
        {
            $$ = std::vector<corevm::IRTypeDecl>();
        }
    | type_def_list type_def
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

type_def
    : TYPE IDENTIFIER LBRACE type_field_list RBRACE
        {
            $$ = corevm::IRTypeDecl();
            $$.name = std::move($2);
            $$.fields = std::move($4);
        }
    ;

type_field_list
    :
        {
            $$ = std::vector<corevm::IRTypeField>();
        }
    | type_field_list type_field
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

type_field
    : ir_identifier_type IDENTIFIER SEMICOLON
        {
            $$ = corevm::IRTypeField();
            $$.type = std::move($1);
            $$.ref_type = corevm::value;
            $$.identifier = std::move($2);
        }
    | ir_identifier_type STAR IDENTIFIER SEMICOLON
        {
            $$ = corevm::IRTypeField();
            $$.type = std::move($1);
            $$.ref_type = corevm::pointer;
            $$.identifier = std::move($3);
        }
    ;

function_def
    : DEF ir_identifier_type IDENTIFIER function_arg_list LBRACE basic_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = std::move($3);
            $$.parameters = std::move($4);
            $$.blocks = std::move($6);
            $$.ret_reftype = corevm::value;
        }
    | DEF ir_identifier_type STAR IDENTIFIER function_arg_list LBRACE basic_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = std::move($4);
            $$.parameters = std::move($5);
            $$.blocks = std::move($7);
            $$.ret_reftype = corevm::pointer;
        }
    | DEF ir_identifier_type IDENTIFIER function_arg_list COLON IDENTIFIER LBRACE basic_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = std::move($3);
            $$.parameters = std::move($4);
            $$.parent.set_string($6);
            $$.blocks = std::move($8);
            $$.ret_reftype = corevm::value;
        }
    | DEF ir_identifier_type STAR IDENTIFIER function_arg_list COLON IDENTIFIER LBRACE basic_block_list RBRACE
        {
            $$ = corevm::IRClosure();
            $$.rettype = $2;
            $$.name = std::move($4);
            $$.parameters = std::move($5);
            $$.parent.set_string($7);
            $$.blocks = std::move($9);
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
            $$ = std::move($2);
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
            $$ = std::move($1);
            $$.push_back($3);
        }
    ;

function_arg
    : ir_identifier_type IDENTIFIER
        {
            $$ = corevm::IRParameter();
            $$.type = std::move($1);
            $$.identifier = std::move($2);
            $$.ref_type = corevm::value;
        }
    | ir_identifier_type STAR IDENTIFIER
        {
            $$ = corevm::IRParameter();
            $$.type = std::move($1);
            $$.identifier = std::move($3);
            $$.ref_type = corevm::pointer;
        }
    ;

basic_block_list
    :
        {
            $$ = std::vector<corevm::IRBasicBlock>();
        }
    | basic_block_list basic_block
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

basic_block
    : IDENTIFIER COLON instruction_group
        {
            $$ = corevm::IRBasicBlock();
            $$.label = std::move($1);
            $$.body = std::move($3);
        }
    ;

instruction_group
    : instruction
        {
            $$ = std::vector<corevm::IRInstruction>();
            $$.push_back($1);
        }
    | instruction_group instruction
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

instruction
    : identifier ASSIGN expr SEMICOLON
        {
            $$ = std::move($3);
            $$.target.set_string($1);
        }
    | expr SEMICOLON
        {
            $$ = std::move($1);
            $$.target.set_null();
        }
    ;

expr
    : OPCODE instruction_options
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.options = std::move($2);
            $$.type.set_null();
        }
    | OPCODE instruction_options ir_identifier_type
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.options = std::move($2);
            $$.type.set_IRIdentifierType($3);
        }
    | OPCODE instruction_options operand_list
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.options = std::move($2);
            $$.oprds = std::move($3);
            $$.type.set_null();
        }
    | OPCODE instruction_options operand_list label_list
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.options = std::move($2);
            $$.oprds = std::move($3);
            $$.labels.set_array($4);
            $$.type.set_null();
        }
    | OPCODE instruction_options ir_identifier_type operand_list
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.options = std::move($2);
            $$.type.set_IRIdentifierType($3);
            $$.oprds = std::move($4);
        }
    | OPCODE instruction_options ir_identifier_type operand_list label_list
        {
            $$ = corevm::IRInstruction();
            $$.opcode = $1;
            $$.options = std::move($2);
            $$.type.set_IRIdentifierType($3);
            $$.oprds = std::move($4);
            $$.labels.set_array($5);
        }
    ;

instruction_options
    :
        {
            $$ = std::vector<std::string>();
        }
    | LBRACKET instruction_option_list RBRACKET
        {
            $$ = std::move($2);
        }
    ;

instruction_option_list
    : IDENTIFIER
        {
            $$ = std::vector<std::string>();
            $$.push_back($1);
        }
    | instruction_option_list IDENTIFIER
        {
            $$ = std::move($1);
            $$.push_back($2);
        }
    ;

label_list
    : LBRACKET label_list_core RBRACKET
        {
            $$ = std::move($2);
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
            $$ = std::move($1);
            $$.push_back($3);
        }
    ;

label
    : LABEL POND IDENTIFIER
        {
            $$ = corevm::IRLabel();
            $$.name = std::move($3);
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
            $$ = std::move($1);
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
            $$ = std::move($2);
        }
    ;

ir_identifier_type
    : IDENTIFIER
        {
            $$ = corevm::IRIdentifierType();
            $$.type = corevm::IdentifierType_Identifier;
            $$.value.set_string($1);
        }
    | ir_value_type
        {
            $$ = corevm::IRIdentifierType();
            $$.type = corevm::IdentifierType_ValueType;
            $$.value.set_IRValueType($1);
        }
    | ir_value_type_array
        {
            $$ = corevm::IRIdentifierType();
            $$.type = corevm::IdentifierType_Array;
            $$.value.set_IRArrayType($1);
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
    | TYPE_NAME_BOOLEAN BOOLEAN_CONSTANT
        {
            $$ = corevm::IRValue();
            $$.type = corevm::boolean;
            $$.value.set_bool($2);
        }
    | TYPE_NAME_BOOLEAN DECIMALINTEGER
        {
            $$ = corevm::IRValue();
            $$.type = corevm::boolean;
            $$.value.set_bool(static_cast<bool>($2));
        }
    ;

ir_value_type
    : ir_value_type_integer_numeric             { $$ = $1; }
    | ir_value_type_floating_numeric            { $$ = $1; }
    | ir_value_type_string                      { $$ = $1; }
    | TYPE_NAME_VOID                            { $$ = corevm::voidtype;   }
    | TYPE_NAME_BOOLEAN                         { $$ = corevm::boolean;    }
    | TYPE_NAME_OBJECT                          { $$ = corevm::object;     }
    ;

ir_value_type_array
    : ARRAY LBRACKET DECIMALINTEGER STAR ir_identifier_type RBRACKET
        {
            $$ = corevm::IRArrayType();
            $$.len = $3;
            $$.type = std::move($5);
        }
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
