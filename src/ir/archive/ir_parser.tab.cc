// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.
#line 103 "ir_parser.yy" // lalr1.cc:404

    /**
     * TODO: Grammar currently cannot handle the following:
     *
     *     - Module metadata.
     *     - Use identifiers and string literlas in instructions.
     *
     * TODO: [COREVM-571] Refine format and grammar of Intermediate Representation
     */

#line 47 "ir_parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "ir_parser.tab.hh"

// User implementation prologue.

#line 61 "ir_parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 27 "ir_parser.yy" // lalr1.cc:413

# include "ir_parser_driver.h"

#line 67 "ir_parser.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 153 "ir_parser.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ir_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  ir_parser::ir_parser (IRParserDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  ir_parser::~ir_parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  ir_parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  ir_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  ir_parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  ir_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  ir_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  ir_parser::symbol_number_type
  ir_parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  ir_parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  ir_parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 63: // ir_array_value
        value.move< corevm::IRArrayValue > (that.value);
        break;

      case 52: // function_block
        value.move< corevm::IRBasicBlock > (that.value);
        break;

      case 47: // function_def
        value.move< corevm::IRClosure > (that.value);
        break;

      case 54: // stmt
      case 55: // expr
        value.move< corevm::IRInstruction > (that.value);
        break;

      case 58: // label
        value.move< corevm::IRLabel > (that.value);
        break;

      case 41: // input
        value.move< corevm::IRModule > (that.value);
        break;

      case 19: // OPCODE
        value.move< corevm::IROpcode > (that.value);
        break;

      case 60: // operand
        value.move< corevm::IROperand > (that.value);
        break;

      case 50: // function_arg
        value.move< corevm::IRParameter > (that.value);
        break;

      case 44: // structure_def
        value.move< corevm::IRStructDecl > (that.value);
        break;

      case 46: // structure_field
        value.move< corevm::IRStructField > (that.value);
        break;

      case 62: // ir_value
        value.move< corevm::IRValue > (that.value);
        break;

      case 64: // ir_value_type
      case 65: // ir_value_type_array
      case 66: // ir_value_type_string
      case 67: // ir_value_type_floating_numeric
      case 68: // ir_value_type_integer_numeric
      case 69: // ir_value_type_long_integer_numeric
      case 70: // ir_value_type_short_integer_numeric
        value.move< corevm::IRValueType > (that.value);
        break;

      case 25: // FLOATINGNUM
        value.move< double > (that.value);
        break;

      case 24: // DECIMALINTEGER
        value.move< int64_t > (that.value);
        break;

      case 3: // TYPE_NAME_I8
      case 4: // TYPE_NAME_UI8
      case 5: // TYPE_NAME_I16
      case 6: // TYPE_NAME_UI16
      case 7: // TYPE_NAME_I32
      case 8: // TYPE_NAME_UI32
      case 9: // TYPE_NAME_I64
      case 10: // TYPE_NAME_UI64
      case 11: // TYPE_NAME_SPF
      case 12: // TYPE_NAME_DPF
      case 13: // TYPE_NAME_BOOLEAN
      case 14: // TYPE_NAME_VOID
      case 15: // TYPE_NAME_STRING
      case 16: // TYPE_NAME_ARRAY
      case 17: // TYPE_NAME_STRUCTTYPE
      case 18: // TYPE_NAME_PTRTYPE
      case 20: // DEF
      case 21: // STRUCTURE
      case 22: // IDENTIFIER
      case 23: // STRING_LITERAL
      case 26: // COMMA
      case 27: // COLON
      case 28: // SEMICOLON
      case 29: // PERCENT
      case 30: // STAR
      case 31: // POND
      case 32: // ASSIGN
      case 33: // LPAREN
      case 34: // RPAREN
      case 35: // LBRACKET
      case 36: // RBRACKET
      case 37: // LBRACE
      case 38: // RBRACE
      case 39: // LABEL
      case 61: // identifier
        value.move< std::string > (that.value);
        break;

      case 51: // function_block_list
        value.move< std::vector<corevm::IRBasicBlock> > (that.value);
        break;

      case 42: // function_def_list
        value.move< std::vector<corevm::IRClosure> > (that.value);
        break;

      case 53: // stmt_group
        value.move< std::vector<corevm::IRInstruction > > (that.value);
        break;

      case 56: // label_list
      case 57: // label_list_core
        value.move< std::vector<corevm::IRLabel> > (that.value);
        break;

      case 59: // operand_list
        value.move< std::vector<corevm::IROperand> > (that.value);
        break;

      case 48: // function_arg_list
      case 49: // function_arg_list_core
        value.move< std::vector<corevm::IRParameter> > (that.value);
        break;

      case 43: // structure_def_list
        value.move< std::vector<corevm::IRStructDecl> > (that.value);
        break;

      case 45: // structure_field_list
        value.move< std::vector<corevm::IRStructField> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  ir_parser::stack_symbol_type&
  ir_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 63: // ir_array_value
        value.copy< corevm::IRArrayValue > (that.value);
        break;

      case 52: // function_block
        value.copy< corevm::IRBasicBlock > (that.value);
        break;

      case 47: // function_def
        value.copy< corevm::IRClosure > (that.value);
        break;

      case 54: // stmt
      case 55: // expr
        value.copy< corevm::IRInstruction > (that.value);
        break;

      case 58: // label
        value.copy< corevm::IRLabel > (that.value);
        break;

      case 41: // input
        value.copy< corevm::IRModule > (that.value);
        break;

      case 19: // OPCODE
        value.copy< corevm::IROpcode > (that.value);
        break;

      case 60: // operand
        value.copy< corevm::IROperand > (that.value);
        break;

      case 50: // function_arg
        value.copy< corevm::IRParameter > (that.value);
        break;

      case 44: // structure_def
        value.copy< corevm::IRStructDecl > (that.value);
        break;

      case 46: // structure_field
        value.copy< corevm::IRStructField > (that.value);
        break;

      case 62: // ir_value
        value.copy< corevm::IRValue > (that.value);
        break;

      case 64: // ir_value_type
      case 65: // ir_value_type_array
      case 66: // ir_value_type_string
      case 67: // ir_value_type_floating_numeric
      case 68: // ir_value_type_integer_numeric
      case 69: // ir_value_type_long_integer_numeric
      case 70: // ir_value_type_short_integer_numeric
        value.copy< corevm::IRValueType > (that.value);
        break;

      case 25: // FLOATINGNUM
        value.copy< double > (that.value);
        break;

      case 24: // DECIMALINTEGER
        value.copy< int64_t > (that.value);
        break;

      case 3: // TYPE_NAME_I8
      case 4: // TYPE_NAME_UI8
      case 5: // TYPE_NAME_I16
      case 6: // TYPE_NAME_UI16
      case 7: // TYPE_NAME_I32
      case 8: // TYPE_NAME_UI32
      case 9: // TYPE_NAME_I64
      case 10: // TYPE_NAME_UI64
      case 11: // TYPE_NAME_SPF
      case 12: // TYPE_NAME_DPF
      case 13: // TYPE_NAME_BOOLEAN
      case 14: // TYPE_NAME_VOID
      case 15: // TYPE_NAME_STRING
      case 16: // TYPE_NAME_ARRAY
      case 17: // TYPE_NAME_STRUCTTYPE
      case 18: // TYPE_NAME_PTRTYPE
      case 20: // DEF
      case 21: // STRUCTURE
      case 22: // IDENTIFIER
      case 23: // STRING_LITERAL
      case 26: // COMMA
      case 27: // COLON
      case 28: // SEMICOLON
      case 29: // PERCENT
      case 30: // STAR
      case 31: // POND
      case 32: // ASSIGN
      case 33: // LPAREN
      case 34: // RPAREN
      case 35: // LBRACKET
      case 36: // RBRACKET
      case 37: // LBRACE
      case 38: // RBRACE
      case 39: // LABEL
      case 61: // identifier
        value.copy< std::string > (that.value);
        break;

      case 51: // function_block_list
        value.copy< std::vector<corevm::IRBasicBlock> > (that.value);
        break;

      case 42: // function_def_list
        value.copy< std::vector<corevm::IRClosure> > (that.value);
        break;

      case 53: // stmt_group
        value.copy< std::vector<corevm::IRInstruction > > (that.value);
        break;

      case 56: // label_list
      case 57: // label_list_core
        value.copy< std::vector<corevm::IRLabel> > (that.value);
        break;

      case 59: // operand_list
        value.copy< std::vector<corevm::IROperand> > (that.value);
        break;

      case 48: // function_arg_list
      case 49: // function_arg_list_core
        value.copy< std::vector<corevm::IRParameter> > (that.value);
        break;

      case 43: // structure_def_list
        value.copy< std::vector<corevm::IRStructDecl> > (that.value);
        break;

      case 45: // structure_field_list
        value.copy< std::vector<corevm::IRStructField> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  ir_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  ir_parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  ir_parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  ir_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  ir_parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ir_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ir_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ir_parser::debug_level_type
  ir_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  ir_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline ir_parser::state_type
  ir_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  ir_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  ir_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  ir_parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 19 "ir_parser.yy" // lalr1.cc:745
{
  // Initialize the initial location.
  if (!driver.input_file().empty())
  {
    yyla.location.begin.filename = yyla.location.end.filename = &driver.input_file();
  }
}

#line 708 "ir_parser.tab.cc" // lalr1.cc:745

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 63: // ir_array_value
        yylhs.value.build< corevm::IRArrayValue > ();
        break;

      case 52: // function_block
        yylhs.value.build< corevm::IRBasicBlock > ();
        break;

      case 47: // function_def
        yylhs.value.build< corevm::IRClosure > ();
        break;

      case 54: // stmt
      case 55: // expr
        yylhs.value.build< corevm::IRInstruction > ();
        break;

      case 58: // label
        yylhs.value.build< corevm::IRLabel > ();
        break;

      case 41: // input
        yylhs.value.build< corevm::IRModule > ();
        break;

      case 19: // OPCODE
        yylhs.value.build< corevm::IROpcode > ();
        break;

      case 60: // operand
        yylhs.value.build< corevm::IROperand > ();
        break;

      case 50: // function_arg
        yylhs.value.build< corevm::IRParameter > ();
        break;

      case 44: // structure_def
        yylhs.value.build< corevm::IRStructDecl > ();
        break;

      case 46: // structure_field
        yylhs.value.build< corevm::IRStructField > ();
        break;

      case 62: // ir_value
        yylhs.value.build< corevm::IRValue > ();
        break;

      case 64: // ir_value_type
      case 65: // ir_value_type_array
      case 66: // ir_value_type_string
      case 67: // ir_value_type_floating_numeric
      case 68: // ir_value_type_integer_numeric
      case 69: // ir_value_type_long_integer_numeric
      case 70: // ir_value_type_short_integer_numeric
        yylhs.value.build< corevm::IRValueType > ();
        break;

      case 25: // FLOATINGNUM
        yylhs.value.build< double > ();
        break;

      case 24: // DECIMALINTEGER
        yylhs.value.build< int64_t > ();
        break;

      case 3: // TYPE_NAME_I8
      case 4: // TYPE_NAME_UI8
      case 5: // TYPE_NAME_I16
      case 6: // TYPE_NAME_UI16
      case 7: // TYPE_NAME_I32
      case 8: // TYPE_NAME_UI32
      case 9: // TYPE_NAME_I64
      case 10: // TYPE_NAME_UI64
      case 11: // TYPE_NAME_SPF
      case 12: // TYPE_NAME_DPF
      case 13: // TYPE_NAME_BOOLEAN
      case 14: // TYPE_NAME_VOID
      case 15: // TYPE_NAME_STRING
      case 16: // TYPE_NAME_ARRAY
      case 17: // TYPE_NAME_STRUCTTYPE
      case 18: // TYPE_NAME_PTRTYPE
      case 20: // DEF
      case 21: // STRUCTURE
      case 22: // IDENTIFIER
      case 23: // STRING_LITERAL
      case 26: // COMMA
      case 27: // COLON
      case 28: // SEMICOLON
      case 29: // PERCENT
      case 30: // STAR
      case 31: // POND
      case 32: // ASSIGN
      case 33: // LPAREN
      case 34: // RPAREN
      case 35: // LBRACKET
      case 36: // RBRACKET
      case 37: // LBRACE
      case 38: // RBRACE
      case 39: // LABEL
      case 61: // identifier
        yylhs.value.build< std::string > ();
        break;

      case 51: // function_block_list
        yylhs.value.build< std::vector<corevm::IRBasicBlock> > ();
        break;

      case 42: // function_def_list
        yylhs.value.build< std::vector<corevm::IRClosure> > ();
        break;

      case 53: // stmt_group
        yylhs.value.build< std::vector<corevm::IRInstruction > > ();
        break;

      case 56: // label_list
      case 57: // label_list_core
        yylhs.value.build< std::vector<corevm::IRLabel> > ();
        break;

      case 59: // operand_list
        yylhs.value.build< std::vector<corevm::IROperand> > ();
        break;

      case 48: // function_arg_list
      case 49: // function_arg_list_core
        yylhs.value.build< std::vector<corevm::IRParameter> > ();
        break;

      case 43: // structure_def_list
        yylhs.value.build< std::vector<corevm::IRStructDecl> > ();
        break;

      case 45: // structure_field_list
        yylhs.value.build< std::vector<corevm::IRStructField> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 120 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRModule > () = corevm::IRModule();
            yylhs.value.as< corevm::IRModule > ().types = yystack_[1].value.as< std::vector<corevm::IRStructDecl> > ();
            yylhs.value.as< corevm::IRModule > ().closures = yystack_[0].value.as< std::vector<corevm::IRClosure> > ();
        }
#line 961 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 129 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRClosure> > () = std::vector<corevm::IRClosure>();
        }
#line 969 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 133 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRClosure> > () = yystack_[1].value.as< std::vector<corevm::IRClosure> > ();
            yylhs.value.as< std::vector<corevm::IRClosure> > ().push_back(yystack_[0].value.as< corevm::IRClosure > ());
        }
#line 978 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 141 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRStructDecl> > () = std::vector<corevm::IRStructDecl>();
        }
#line 986 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 145 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRStructDecl> > () = yystack_[1].value.as< std::vector<corevm::IRStructDecl> > ();
            yylhs.value.as< std::vector<corevm::IRStructDecl> > ().push_back(yystack_[0].value.as< corevm::IRStructDecl > ());
        }
#line 995 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 153 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRStructDecl > () = corevm::IRStructDecl();
            yylhs.value.as< corevm::IRStructDecl > ().name = yystack_[3].value.as< std::string > ();
            yylhs.value.as< corevm::IRStructDecl > ().fields = yystack_[1].value.as< std::vector<corevm::IRStructField> > ();
        }
#line 1005 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 162 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRStructField> > () = std::vector<corevm::IRStructField>();
        }
#line 1013 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 166 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRStructField> > () = yystack_[1].value.as< std::vector<corevm::IRStructField> > ();
            yylhs.value.as< std::vector<corevm::IRStructField> > ().push_back(yystack_[0].value.as< corevm::IRStructField > ());
        }
#line 1022 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 174 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRStructField > () = corevm::IRStructField();
            yylhs.value.as< corevm::IRStructField > ().type.set_string(yystack_[2].value.as< std::string > ());
            yylhs.value.as< corevm::IRStructField > ().ref_type = corevm::value;
            yylhs.value.as< corevm::IRStructField > ().identifier = yystack_[1].value.as< std::string > ();
        }
#line 1033 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 181 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRStructField > () = corevm::IRStructField();
            yylhs.value.as< corevm::IRStructField > ().type.set_string(yystack_[3].value.as< std::string > ());
            yylhs.value.as< corevm::IRStructField > ().ref_type = corevm::pointer;
            yylhs.value.as< corevm::IRStructField > ().identifier = yystack_[1].value.as< std::string > ();
        }
#line 1044 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 188 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRStructField > () = corevm::IRStructField();
            yylhs.value.as< corevm::IRStructField > ().type.set_IRValueType(yystack_[2].value.as< corevm::IRValueType > ());
            yylhs.value.as< corevm::IRStructField > ().ref_type = corevm::value;
            yylhs.value.as< corevm::IRStructField > ().identifier = yystack_[1].value.as< std::string > ();
        }
#line 1055 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 195 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRStructField > () = corevm::IRStructField();
            yylhs.value.as< corevm::IRStructField > ().type.set_IRValueType(yystack_[3].value.as< corevm::IRValueType > ());
            yylhs.value.as< corevm::IRStructField > ().ref_type = corevm::pointer;
            yylhs.value.as< corevm::IRStructField > ().identifier = yystack_[1].value.as< std::string > ();
        }
#line 1066 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 205 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRClosure > () = corevm::IRClosure();
            yylhs.value.as< corevm::IRClosure > ().rettype = yystack_[5].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRClosure > ().name = yystack_[4].value.as< std::string > ();
            yylhs.value.as< corevm::IRClosure > ().parameters = yystack_[3].value.as< std::vector<corevm::IRParameter> > ();
            yylhs.value.as< corevm::IRClosure > ().blocks = yystack_[1].value.as< std::vector<corevm::IRBasicBlock> > ();
            yylhs.value.as< corevm::IRClosure > ().ret_reftype = corevm::value;
        }
#line 1079 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 214 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRClosure > () = corevm::IRClosure();
            yylhs.value.as< corevm::IRClosure > ().rettype = yystack_[6].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRClosure > ().name = yystack_[4].value.as< std::string > ();
            yylhs.value.as< corevm::IRClosure > ().parameters = yystack_[3].value.as< std::vector<corevm::IRParameter> > ();
            yylhs.value.as< corevm::IRClosure > ().blocks = yystack_[1].value.as< std::vector<corevm::IRBasicBlock> > ();
            yylhs.value.as< corevm::IRClosure > ().ret_reftype = corevm::pointer;
        }
#line 1092 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 223 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRClosure > () = corevm::IRClosure();
            yylhs.value.as< corevm::IRClosure > ().rettype = yystack_[7].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRClosure > ().name = yystack_[6].value.as< std::string > ();
            yylhs.value.as< corevm::IRClosure > ().parameters = yystack_[5].value.as< std::vector<corevm::IRParameter> > ();
            yylhs.value.as< corevm::IRClosure > ().parent.set_string(yystack_[3].value.as< std::string > ());
            yylhs.value.as< corevm::IRClosure > ().blocks = yystack_[1].value.as< std::vector<corevm::IRBasicBlock> > ();
            yylhs.value.as< corevm::IRClosure > ().ret_reftype = corevm::value;
        }
#line 1106 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 233 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRClosure > () = corevm::IRClosure();
            yylhs.value.as< corevm::IRClosure > ().rettype = yystack_[8].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRClosure > ().name = yystack_[6].value.as< std::string > ();
            yylhs.value.as< corevm::IRClosure > ().parameters = yystack_[5].value.as< std::vector<corevm::IRParameter> > ();
            yylhs.value.as< corevm::IRClosure > ().parent.set_string(yystack_[3].value.as< std::string > ());
            yylhs.value.as< corevm::IRClosure > ().blocks = yystack_[1].value.as< std::vector<corevm::IRBasicBlock> > ();
            yylhs.value.as< corevm::IRClosure > ().ret_reftype = corevm::pointer;
        }
#line 1120 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 246 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRParameter> > () = std::vector<corevm::IRParameter>();
        }
#line 1128 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 250 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRParameter> > () = yystack_[1].value.as< std::vector<corevm::IRParameter> > ();
        }
#line 1136 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 257 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRParameter> > () = std::vector<corevm::IRParameter>();
            yylhs.value.as< std::vector<corevm::IRParameter> > ().push_back(yystack_[0].value.as< corevm::IRParameter > ());
        }
#line 1145 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 262 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRParameter> > () = yystack_[2].value.as< std::vector<corevm::IRParameter> > ();
            yylhs.value.as< std::vector<corevm::IRParameter> > ().push_back(yystack_[0].value.as< corevm::IRParameter > ());
        }
#line 1154 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 270 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRParameter > () = corevm::IRParameter();
            yylhs.value.as< corevm::IRParameter > ().type = yystack_[1].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRParameter > ().identifier = yystack_[0].value.as< std::string > ();
            yylhs.value.as< corevm::IRParameter > ().ref_type = corevm::value;
        }
#line 1165 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 277 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRParameter > () = corevm::IRParameter();
            yylhs.value.as< corevm::IRParameter > ().type = yystack_[2].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRParameter > ().identifier = yystack_[0].value.as< std::string > ();
            yylhs.value.as< corevm::IRParameter > ().ref_type = corevm::pointer;
        }
#line 1176 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 287 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRBasicBlock> > () = std::vector<corevm::IRBasicBlock>();
        }
#line 1184 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 291 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRBasicBlock> > () = yystack_[1].value.as< std::vector<corevm::IRBasicBlock> > ();
            yylhs.value.as< std::vector<corevm::IRBasicBlock> > ().push_back(yystack_[0].value.as< corevm::IRBasicBlock > ());
        }
#line 1193 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 299 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRBasicBlock > () = corevm::IRBasicBlock();
            yylhs.value.as< corevm::IRBasicBlock > ().label = yystack_[2].value.as< std::string > ();
            yylhs.value.as< corevm::IRBasicBlock > ().body = yystack_[0].value.as< std::vector<corevm::IRInstruction > > ();
        }
#line 1203 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 308 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRInstruction > > () = std::vector<corevm::IRInstruction>();
        }
#line 1211 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 312 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRInstruction > > () = yystack_[1].value.as< std::vector<corevm::IRInstruction > > ();
            yylhs.value.as< std::vector<corevm::IRInstruction > > ().push_back(yystack_[0].value.as< corevm::IRInstruction > ());
        }
#line 1220 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 320 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRInstruction > () = yystack_[1].value.as< corevm::IRInstruction > ();
            yylhs.value.as< corevm::IRInstruction > ().target.set_string(yystack_[3].value.as< std::string > ());
        }
#line 1229 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 325 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRInstruction > () = yystack_[1].value.as< corevm::IRInstruction > ();
            yylhs.value.as< corevm::IRInstruction > ().target.set_null();
        }
#line 1238 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 335 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRInstruction > () = corevm::IRInstruction();
            yylhs.value.as< corevm::IRInstruction > ().opcode = yystack_[0].value.as< corevm::IROpcode > ();
        }
#line 1247 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 340 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRInstruction > () = corevm::IRInstruction();
            yylhs.value.as< corevm::IRInstruction > ().opcode = yystack_[1].value.as< corevm::IROpcode > ();
            yylhs.value.as< corevm::IRInstruction > ().opcodeval = yystack_[0].value.as< corevm::IRValue > ();
        }
#line 1257 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 346 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRInstruction > () = corevm::IRInstruction();
            yylhs.value.as< corevm::IRInstruction > ().opcode = yystack_[3].value.as< corevm::IROpcode > ();
            yylhs.value.as< corevm::IRInstruction > ().opcodeval = yystack_[2].value.as< corevm::IRValue > ();
            yylhs.value.as< corevm::IRInstruction > ().oprds = yystack_[0].value.as< std::vector<corevm::IROperand> > ();
        }
#line 1268 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 353 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRInstruction > () = corevm::IRInstruction();
            yylhs.value.as< corevm::IRInstruction > ().opcode = yystack_[4].value.as< corevm::IROpcode > ();
            yylhs.value.as< corevm::IRInstruction > ().opcodeval = yystack_[3].value.as< corevm::IRValue > ();
            yylhs.value.as< corevm::IRInstruction > ().oprds = yystack_[1].value.as< std::vector<corevm::IROperand> > ();
            yylhs.value.as< corevm::IRInstruction > ().labels.set_array(yystack_[0].value.as< std::vector<corevm::IRLabel> > ());
        }
#line 1280 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 364 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRLabel> > () = yystack_[1].value.as< std::vector<corevm::IRLabel> > ();
        }
#line 1288 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 371 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRLabel> > () = std::vector<corevm::IRLabel>();
            yylhs.value.as< std::vector<corevm::IRLabel> > ().push_back(yystack_[0].value.as< corevm::IRLabel > ());
        }
#line 1297 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 376 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IRLabel> > () = yystack_[2].value.as< std::vector<corevm::IRLabel> > ();
            yylhs.value.as< std::vector<corevm::IRLabel> > ().push_back(yystack_[0].value.as< corevm::IRLabel > ());
        }
#line 1306 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 384 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRLabel > () = corevm::IRLabel();
            yylhs.value.as< corevm::IRLabel > ().name = yystack_[0].value.as< std::string > ();
        }
#line 1315 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 392 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IROperand> > () = std::vector<corevm::IROperand>();
            yylhs.value.as< std::vector<corevm::IROperand> > ().push_back(yystack_[0].value.as< corevm::IROperand > ());
        }
#line 1324 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 397 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<corevm::IROperand> > () = yystack_[1].value.as< std::vector<corevm::IROperand> > ();
            yylhs.value.as< std::vector<corevm::IROperand> > ().push_back(yystack_[0].value.as< corevm::IROperand > ());
        }
#line 1333 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 405 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IROperand > () = corevm::IROperand();
            yylhs.value.as< corevm::IROperand > ().type = corevm::constant;
            yylhs.value.as< corevm::IROperand > ().value.set_IRValue(yystack_[0].value.as< corevm::IRValue > ());
        }
#line 1343 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 411 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IROperand > () = corevm::IROperand();
            yylhs.value.as< corevm::IROperand > ().type = corevm::ref;
            yylhs.value.as< corevm::IROperand > ().value.set_string(yystack_[0].value.as< std::string > ());
        }
#line 1353 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 420 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
        }
#line 1361 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 427 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = yystack_[1].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRValue > ().value.set_int(yystack_[0].value.as< int64_t > ());
        }
#line 1371 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 433 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = yystack_[1].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRValue > ().value.set_long(yystack_[0].value.as< int64_t > ());
        }
#line 1381 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 439 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = corevm::spf;
            yylhs.value.as< corevm::IRValue > ().value.set_float(yystack_[0].value.as< double > ());
        }
#line 1391 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 445 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = corevm::dpf;
            yylhs.value.as< corevm::IRValue > ().value.set_double(yystack_[0].value.as< double > ());
        }
#line 1401 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 451 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = yystack_[1].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRValue > ().value.set_string(yystack_[0].value.as< std::string > ());
        }
#line 1411 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 457 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = yystack_[1].value.as< corevm::IRValueType > ();
            yylhs.value.as< corevm::IRValue > ().value.set_IRArrayValue(yystack_[0].value.as< corevm::IRArrayValue > ());
        }
#line 1421 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 463 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRValue > () = corevm::IRValue();
            yylhs.value.as< corevm::IRValue > ().type = corevm::boolean;
            // TODO: consider adding "true" and "false" as tokens.
            yylhs.value.as< corevm::IRValue > ().value.set_bool(static_cast<bool>(yystack_[0].value.as< int64_t > ()));
        }
#line 1432 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 473 "ir_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< corevm::IRArrayValue > () = corevm::IRArrayValue();
            yylhs.value.as< corevm::IRArrayValue > ().len = yystack_[3].value.as< int64_t > ();
            yylhs.value.as< corevm::IRArrayValue > ().type = yystack_[1].value.as< corevm::IRValueType > ();
        }
#line 1442 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 481 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = yystack_[0].value.as< corevm::IRValueType > (); }
#line 1448 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 482 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = yystack_[0].value.as< corevm::IRValueType > (); }
#line 1454 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 483 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = yystack_[0].value.as< corevm::IRValueType > (); }
#line 1460 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 484 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = yystack_[0].value.as< corevm::IRValueType > (); }
#line 1466 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 485 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::voidtype;   }
#line 1472 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 486 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::structtype; }
#line 1478 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 487 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::ptrtype;    }
#line 1484 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 488 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::boolean;    }
#line 1490 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 492 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::array; }
#line 1496 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 496 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::string; }
#line 1502 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 500 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::spf; }
#line 1508 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 501 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::dpf; }
#line 1514 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 505 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = yystack_[0].value.as< corevm::IRValueType > (); }
#line 1520 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 506 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = yystack_[0].value.as< corevm::IRValueType > (); }
#line 1526 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 510 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::i64;  }
#line 1532 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 511 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::ui64; }
#line 1538 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 515 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::i8;   }
#line 1544 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 516 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::ui8;  }
#line 1550 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 517 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::i16;  }
#line 1556 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 518 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::ui16; }
#line 1562 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 519 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::i32;  }
#line 1568 "ir_parser.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 520 "ir_parser.yy" // lalr1.cc:859
    { yylhs.value.as< corevm::IRValueType > () = corevm::ui32; }
#line 1574 "ir_parser.tab.cc" // lalr1.cc:859
    break;


#line 1578 "ir_parser.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  ir_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  ir_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char ir_parser::yypact_ninf_ = -58;

  const signed char ir_parser::yytable_ninf_ = -1;

  const short int
  ir_parser::yypact_[] =
  {
     -58,    23,     6,   -58,     2,    42,   -58,    68,   114,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,    35,   -58,   -58,
     -58,   -58,   -58,   -58,    -2,    71,    45,    81,   -58,   -58,
      84,    67,    -6,    71,    80,    87,   119,   126,   -58,   118,
     -58,    85,   127,   -58,    27,   -58,   122,   -58,   123,   114,
     -58,   -58,   131,   117,    -5,   133,   -58,   -58,   -58,   -58,
     -58,   -58,   129,   -58,   -58,   120,    -4,    -3,     3,   -58,
     -58,   -58,   130,   136,     3,   -58,   132,   134,    78,   137,
     138,   135,   139,   140,   141,   143,   144,   -58,   -58,   -58,
     142,   -58,   -58,   -58,   -58,    83,   145,   -58,   -58,   -58,
     -58,   146,    34,   -58,   -58,   -58,   147,   -58,   148,   -58,
     -58,   114,   149,    30,   -58,   150,   151,   148,   -58,   -58,
     -58,   -58
  };

  const unsigned char
  ir_parser::yydefact_[] =
  {
       5,     0,     3,     1,     0,     2,     6,     0,     0,     4,
       8,    68,    69,    70,    71,    72,    73,    66,    67,    62,
      63,    59,    56,    61,    60,    57,    58,     0,    55,    54,
      53,    52,    65,    64,     0,     0,     0,     0,     7,     9,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      20,     0,     0,    24,     0,    10,     0,    12,     0,     0,
      19,    22,     0,     0,     0,     0,    24,    11,    13,    21,
      23,    24,     0,    14,    25,     0,     0,     0,     0,    24,
      15,    16,    31,     0,    26,    27,     0,     0,     0,     0,
       0,     0,    32,     0,     0,     0,     0,    43,    28,    30,
       0,    17,    46,    47,    50,     0,     0,    49,    48,    45,
      44,     0,    33,    39,    42,    41,     0,    29,     0,    34,
      40,     0,     0,     0,    36,     0,     0,     0,    35,    51,
      38,    37
  };

  const short int
  ir_parser::yypgoto_[] =
  {
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   128,   -58,
     111,    31,   -58,   -58,    88,    76,   -58,   -58,    51,   -58,
      69,   -44,    97,   -58,    -8,   -57,   -54,   -58,   -58,   -53,
     -52
  };

  const signed char
  ir_parser::yydefgoto_[] =
  {
      -1,     1,     5,     2,     6,    34,    39,     9,    42,    49,
      50,    64,    74,    84,    85,    86,   119,   123,   124,   112,
     113,    87,   115,   107,    51,    28,    29,    30,    31,    32,
      33
  };

  const unsigned char
  ir_parser::yytable_[] =
  {
      27,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    72,    72,    72,
      37,    52,    82,     3,     7,    93,    40,     4,    94,    95,
      96,    53,    83,    73,    80,    81,    38,    11,    12,    13,
      14,    15,    16,    17,    18,    89,    90,    91,    93,    23,
      24,    94,    95,    96,    65,    93,   127,    35,    94,    95,
      96,   114,     8,    83,    66,    36,   128,    43,   114,   118,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    11,    12,    13,    14,
      15,    16,    17,    18,    89,    90,    91,    76,    23,    24,
      72,    48,    77,    44,    41,    10,    46,    61,    55,    56,
      88,    45,    83,   125,    47,    62,   101,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    11,    12,    13,    14,    15,    16,    17,
      18,    89,    90,    91,    59,    23,    24,    57,    58,    63,
      67,    68,    60,    70,    71,    75,    78,    79,    97,   104,
      99,    82,   102,   103,   108,   105,   100,   109,   110,   116,
      69,    54,    98,   130,   117,   106,   111,   121,   131,    92,
     126,   120,     0,     0,     0,     0,   129,   122
  };

  const signed char
  ir_parser::yycheck_[] =
  {
       8,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    22,    22,    22,
      22,    27,    19,     0,    22,    82,    34,    21,    82,    82,
      82,    37,    29,    38,    38,    38,    38,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,   105,    15,
      16,   105,   105,   105,    27,   112,    26,    22,   112,   112,
     112,   105,    20,    29,    37,    30,    36,    22,   112,    35,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    66,    15,    16,
      22,    34,    71,    22,    33,    37,    22,    22,    28,    22,
      79,    30,    29,   121,    30,    30,    38,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    26,    15,    16,    28,    22,    22,
      28,    28,    34,    22,    37,    22,    27,    37,    22,    24,
      28,    19,    25,    25,    23,    26,    32,    24,    24,    24,
      59,    43,    84,    22,    28,    35,   100,    30,   127,    82,
      31,   112,    -1,    -1,    -1,    -1,    36,    39
  };

  const unsigned char
  ir_parser::yystos_[] =
  {
       0,    41,    43,     0,    21,    42,    44,    22,    20,    47,
      37,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    64,    65,    66,
      67,    68,    69,    70,    45,    22,    30,    22,    38,    46,
      64,    33,    48,    22,    22,    30,    22,    30,    34,    49,
      50,    64,    27,    37,    48,    28,    22,    28,    22,    26,
      34,    22,    30,    22,    51,    27,    37,    28,    28,    50,
      22,    37,    22,    38,    52,    22,    51,    51,    27,    37,
      38,    38,    19,    29,    53,    54,    55,    61,    51,    11,
      12,    13,    62,    65,    66,    69,    70,    22,    54,    28,
      32,    38,    25,    25,    24,    26,    35,    63,    23,    24,
      24,    55,    59,    60,    61,    62,    24,    28,    35,    56,
      60,    30,    39,    57,    58,    64,    31,    26,    36,    36,
      22,    58
  };

  const unsigned char
  ir_parser::yyr1_[] =
  {
       0,    40,    41,    42,    42,    43,    43,    44,    45,    45,
      46,    46,    46,    46,    47,    47,    47,    47,    48,    48,
      49,    49,    50,    50,    51,    51,    52,    53,    53,    54,
      54,    55,    55,    55,    55,    56,    57,    57,    58,    59,
      59,    60,    60,    61,    62,    62,    62,    62,    62,    62,
      62,    63,    64,    64,    64,    64,    64,    64,    64,    64,
      65,    66,    67,    67,    68,    68,    69,    69,    70,    70,
      70,    70,    70,    70
  };

  const unsigned char
  ir_parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     0,     2,     5,     0,     2,
       3,     4,     3,     4,     7,     8,     9,    10,     2,     3,
       1,     3,     2,     3,     0,     2,     3,     1,     2,     4,
       2,     1,     2,     4,     5,     3,     1,     3,     3,     1,
       2,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const ir_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "TYPE_NAME_I8",
  "TYPE_NAME_UI8", "TYPE_NAME_I16", "TYPE_NAME_UI16", "TYPE_NAME_I32",
  "TYPE_NAME_UI32", "TYPE_NAME_I64", "TYPE_NAME_UI64", "TYPE_NAME_SPF",
  "TYPE_NAME_DPF", "TYPE_NAME_BOOLEAN", "TYPE_NAME_VOID",
  "TYPE_NAME_STRING", "TYPE_NAME_ARRAY", "TYPE_NAME_STRUCTTYPE",
  "TYPE_NAME_PTRTYPE", "OPCODE", "DEF", "STRUCTURE", "IDENTIFIER",
  "STRING_LITERAL", "DECIMALINTEGER", "FLOATINGNUM", "COMMA", "COLON",
  "SEMICOLON", "PERCENT", "STAR", "POND", "ASSIGN", "LPAREN", "RPAREN",
  "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", "LABEL", "$accept", "input",
  "function_def_list", "structure_def_list", "structure_def",
  "structure_field_list", "structure_field", "function_def",
  "function_arg_list", "function_arg_list_core", "function_arg",
  "function_block_list", "function_block", "stmt_group", "stmt", "expr",
  "label_list", "label_list_core", "label", "operand_list", "operand",
  "identifier", "ir_value", "ir_array_value", "ir_value_type",
  "ir_value_type_array", "ir_value_type_string",
  "ir_value_type_floating_numeric", "ir_value_type_integer_numeric",
  "ir_value_type_long_integer_numeric",
  "ir_value_type_short_integer_numeric", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  ir_parser::yyrline_[] =
  {
       0,   119,   119,   129,   132,   141,   144,   152,   162,   165,
     173,   180,   187,   194,   204,   213,   222,   232,   245,   249,
     256,   261,   269,   276,   287,   290,   298,   307,   311,   319,
     324,   334,   339,   345,   352,   363,   370,   375,   383,   391,
     396,   404,   410,   419,   426,   432,   438,   444,   450,   456,
     462,   472,   481,   482,   483,   484,   485,   486,   487,   488,
     492,   496,   500,   501,   505,   506,   510,   511,   515,   516,
     517,   518,   519,   520
  };

  // Print the state stack on the debug stream.
  void
  ir_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  ir_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 2060 "ir_parser.tab.cc" // lalr1.cc:1167
#line 523 "ir_parser.yy" // lalr1.cc:1168


void
yy::ir_parser::error(const location_type& l, const std::string& m)
{
  driver.error(l, m);
}
