// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file src/ir/ir_parser.tab.hh
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_SRC_IR_IR_PARSER_TAB_HH_INCLUDED
# define YY_YY_SRC_IR_IR_PARSER_TAB_HH_INCLUDED
// //                    "%code requires" blocks.
#line 9 "src/ir/ir_parser.yy" // lalr1.cc:392

# include <string>
# include <utility>
# include "format.h"
# include "format_util.h"
namespace corevm {
namespace ir {
class IRParserDriver;
} /* end namespace ir */
} /* end namespace corevm */

#line 56 "src/ir/ir_parser.tab.hh" // lalr1.cc:392

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace yy {
#line 133 "src/ir/ir_parser.tab.hh" // lalr1.cc:392



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
      : yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytypeid_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *other.yytypeid_);
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };


  /// A Bison parser.
  class ir_parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // BOOLEAN_CONSTANT
      char dummy1[sizeof(bool)];

      // ir_value_type_array
      char dummy2[sizeof(corevm::IRArrayType)];

      // basic_block
      char dummy3[sizeof(corevm::IRBasicBlock)];

      // function_def
      char dummy4[sizeof(corevm::IRClosure)];

      // ir_identifier_type
      char dummy5[sizeof(corevm::IRIdentifierType)];

      // instruction
      // expr
      char dummy6[sizeof(corevm::IRInstruction)];

      // label
      char dummy7[sizeof(corevm::IRLabel)];

      // input
      char dummy8[sizeof(corevm::IRModule)];

      // OPCODE
      char dummy9[sizeof(corevm::IROpcode)];

      // operand
      char dummy10[sizeof(corevm::IROperand)];

      // function_arg
      char dummy11[sizeof(corevm::IRParameter)];

      // type_def
      char dummy12[sizeof(corevm::IRTypeDecl)];

      // type_field
      char dummy13[sizeof(corevm::IRTypeField)];

      // ir_value
      char dummy14[sizeof(corevm::IRValue)];

      // ir_value_type
      // ir_value_type_string
      // ir_value_type_floating_numeric
      // ir_value_type_integer_numeric
      // ir_value_type_long_integer_numeric
      // ir_value_type_short_integer_numeric
      char dummy15[sizeof(corevm::IRValueType)];

      // metadata_def
      char dummy16[sizeof(corevm::ir::MetadataPair)];

      // FLOATINGNUM
      char dummy17[sizeof(double)];

      // DECIMALINTEGER
      char dummy18[sizeof(int64_t)];

      // TYPE_NAME_I8
      // TYPE_NAME_UI8
      // TYPE_NAME_I16
      // TYPE_NAME_UI16
      // TYPE_NAME_I32
      // TYPE_NAME_UI32
      // TYPE_NAME_I64
      // TYPE_NAME_UI64
      // TYPE_NAME_SPF
      // TYPE_NAME_DPF
      // TYPE_NAME_BOOLEAN
      // TYPE_NAME_VOID
      // TYPE_NAME_STRING
      // TYPE_NAME_OBJECT
      // ARRAY
      // DEF
      // TYPE
      // IDENTIFIER
      // STRING_LITERAL
      // COMMA
      // COLON
      // SEMICOLON
      // PERCENT
      // STAR
      // POND
      // ASSIGN
      // LPAREN
      // RPAREN
      // LBRACKET
      // RBRACKET
      // LBRACE
      // RBRACE
      // LABEL
      // identifier
      char dummy19[sizeof(std::string)];

      // basic_block_list
      char dummy20[sizeof(std::vector<corevm::IRBasicBlock>)];

      // function_def_list
      char dummy21[sizeof(std::vector<corevm::IRClosure>)];

      // instruction_group
      char dummy22[sizeof(std::vector<corevm::IRInstruction >)];

      // label_list
      // label_list_core
      char dummy23[sizeof(std::vector<corevm::IRLabel>)];

      // operand_list
      char dummy24[sizeof(std::vector<corevm::IROperand>)];

      // function_arg_list
      // function_arg_list_core
      char dummy25[sizeof(std::vector<corevm::IRParameter>)];

      // type_def_list
      char dummy26[sizeof(std::vector<corevm::IRTypeDecl>)];

      // type_field_list
      char dummy27[sizeof(std::vector<corevm::IRTypeField>)];

      // metadata_def_list
      char dummy28[sizeof(std::vector<corevm::ir::MetadataPair>)];

      // instruction_options
      // instruction_option_list
      char dummy29[sizeof(std::vector<std::string>)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        END = 0,
        TYPE_NAME_I8 = 258,
        TYPE_NAME_UI8 = 259,
        TYPE_NAME_I16 = 260,
        TYPE_NAME_UI16 = 261,
        TYPE_NAME_I32 = 262,
        TYPE_NAME_UI32 = 263,
        TYPE_NAME_I64 = 264,
        TYPE_NAME_UI64 = 265,
        TYPE_NAME_SPF = 266,
        TYPE_NAME_DPF = 267,
        TYPE_NAME_BOOLEAN = 268,
        TYPE_NAME_VOID = 269,
        TYPE_NAME_STRING = 270,
        TYPE_NAME_OBJECT = 271,
        OPCODE = 272,
        ARRAY = 273,
        DEF = 274,
        TYPE = 275,
        IDENTIFIER = 276,
        STRING_LITERAL = 277,
        DECIMALINTEGER = 278,
        FLOATINGNUM = 279,
        BOOLEAN_CONSTANT = 280,
        COMMA = 281,
        COLON = 282,
        SEMICOLON = 283,
        PERCENT = 284,
        STAR = 285,
        POND = 286,
        ASSIGN = 287,
        LPAREN = 288,
        RPAREN = 289,
        LBRACKET = 290,
        RBRACKET = 291,
        LBRACE = 292,
        RBRACE = 293,
        LABEL = 294
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRArrayType v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRBasicBlock v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRClosure v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRIdentifierType v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRInstruction v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRLabel v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRModule v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IROpcode v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IROperand v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRParameter v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRTypeDecl v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRTypeField v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRValue v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::IRValueType v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const corevm::ir::MetadataPair v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const double v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int64_t v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRBasicBlock> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRClosure> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRInstruction > v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRLabel> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IROperand> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRParameter> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRTypeDecl> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRTypeField> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<corevm::ir::MetadataPair> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<std::string> v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_I8 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_UI8 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_I16 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_UI16 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_I32 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_UI32 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_I64 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_UI64 (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_SPF (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_DPF (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_BOOLEAN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_VOID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_STRING (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE_NAME_OBJECT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_OPCODE (const corevm::IROpcode& v, const location_type& l);

    static inline
    symbol_type
    make_ARRAY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DEF (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TYPE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_IDENTIFIER (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_STRING_LITERAL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DECIMALINTEGER (const int64_t& v, const location_type& l);

    static inline
    symbol_type
    make_FLOATINGNUM (const double& v, const location_type& l);

    static inline
    symbol_type
    make_BOOLEAN_CONSTANT (const bool& v, const location_type& l);

    static inline
    symbol_type
    make_COMMA (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_COLON (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_SEMICOLON (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_PERCENT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_STAR (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_POND (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ASSIGN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_LPAREN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RPAREN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_LBRACKET (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RBRACKET (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_LBRACE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RBRACE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_LABEL (const std::string& v, const location_type& l);


    /// Build a parser object.
    ir_parser (corevm::ir::IRParserDriver& driver_yyarg, void* yyscanner_yyarg);
    virtual ~ir_parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    ir_parser (const ir_parser&);
    ir_parser& operator= (const ir_parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const signed char yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 194,     ///< Last index in yytable_.
      yynnts_ = 35,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 40  ///< Number of tokens.
    };


    // User arguments.
    corevm::ir::IRParserDriver& driver;
    void* yyscanner;
  };

  // Symbol number corresponding to token number t.
  inline
  ir_parser::token_number_type
  ir_parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
    };
    const unsigned int user_token_number_max_ = 294;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  ir_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  ir_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  ir_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 25: // BOOLEAN_CONSTANT
        value.copy< bool > (other.value);
        break;

      case 69: // ir_value_type_array
        value.copy< corevm::IRArrayType > (other.value);
        break;

      case 54: // basic_block
        value.copy< corevm::IRBasicBlock > (other.value);
        break;

      case 49: // function_def
        value.copy< corevm::IRClosure > (other.value);
        break;

      case 66: // ir_identifier_type
        value.copy< corevm::IRIdentifierType > (other.value);
        break;

      case 56: // instruction
      case 57: // expr
        value.copy< corevm::IRInstruction > (other.value);
        break;

      case 62: // label
        value.copy< corevm::IRLabel > (other.value);
        break;

      case 41: // input
        value.copy< corevm::IRModule > (other.value);
        break;

      case 17: // OPCODE
        value.copy< corevm::IROpcode > (other.value);
        break;

      case 64: // operand
        value.copy< corevm::IROperand > (other.value);
        break;

      case 52: // function_arg
        value.copy< corevm::IRParameter > (other.value);
        break;

      case 46: // type_def
        value.copy< corevm::IRTypeDecl > (other.value);
        break;

      case 48: // type_field
        value.copy< corevm::IRTypeField > (other.value);
        break;

      case 67: // ir_value
        value.copy< corevm::IRValue > (other.value);
        break;

      case 68: // ir_value_type
      case 70: // ir_value_type_string
      case 71: // ir_value_type_floating_numeric
      case 72: // ir_value_type_integer_numeric
      case 73: // ir_value_type_long_integer_numeric
      case 74: // ir_value_type_short_integer_numeric
        value.copy< corevm::IRValueType > (other.value);
        break;

      case 43: // metadata_def
        value.copy< corevm::ir::MetadataPair > (other.value);
        break;

      case 24: // FLOATINGNUM
        value.copy< double > (other.value);
        break;

      case 23: // DECIMALINTEGER
        value.copy< int64_t > (other.value);
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
      case 16: // TYPE_NAME_OBJECT
      case 18: // ARRAY
      case 19: // DEF
      case 20: // TYPE
      case 21: // IDENTIFIER
      case 22: // STRING_LITERAL
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
      case 65: // identifier
        value.copy< std::string > (other.value);
        break;

      case 53: // basic_block_list
        value.copy< std::vector<corevm::IRBasicBlock> > (other.value);
        break;

      case 44: // function_def_list
        value.copy< std::vector<corevm::IRClosure> > (other.value);
        break;

      case 55: // instruction_group
        value.copy< std::vector<corevm::IRInstruction > > (other.value);
        break;

      case 60: // label_list
      case 61: // label_list_core
        value.copy< std::vector<corevm::IRLabel> > (other.value);
        break;

      case 63: // operand_list
        value.copy< std::vector<corevm::IROperand> > (other.value);
        break;

      case 50: // function_arg_list
      case 51: // function_arg_list_core
        value.copy< std::vector<corevm::IRParameter> > (other.value);
        break;

      case 45: // type_def_list
        value.copy< std::vector<corevm::IRTypeDecl> > (other.value);
        break;

      case 47: // type_field_list
        value.copy< std::vector<corevm::IRTypeField> > (other.value);
        break;

      case 42: // metadata_def_list
        value.copy< std::vector<corevm::ir::MetadataPair> > (other.value);
        break;

      case 58: // instruction_options
      case 59: // instruction_option_list
        value.copy< std::vector<std::string> > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 25: // BOOLEAN_CONSTANT
        value.copy< bool > (v);
        break;

      case 69: // ir_value_type_array
        value.copy< corevm::IRArrayType > (v);
        break;

      case 54: // basic_block
        value.copy< corevm::IRBasicBlock > (v);
        break;

      case 49: // function_def
        value.copy< corevm::IRClosure > (v);
        break;

      case 66: // ir_identifier_type
        value.copy< corevm::IRIdentifierType > (v);
        break;

      case 56: // instruction
      case 57: // expr
        value.copy< corevm::IRInstruction > (v);
        break;

      case 62: // label
        value.copy< corevm::IRLabel > (v);
        break;

      case 41: // input
        value.copy< corevm::IRModule > (v);
        break;

      case 17: // OPCODE
        value.copy< corevm::IROpcode > (v);
        break;

      case 64: // operand
        value.copy< corevm::IROperand > (v);
        break;

      case 52: // function_arg
        value.copy< corevm::IRParameter > (v);
        break;

      case 46: // type_def
        value.copy< corevm::IRTypeDecl > (v);
        break;

      case 48: // type_field
        value.copy< corevm::IRTypeField > (v);
        break;

      case 67: // ir_value
        value.copy< corevm::IRValue > (v);
        break;

      case 68: // ir_value_type
      case 70: // ir_value_type_string
      case 71: // ir_value_type_floating_numeric
      case 72: // ir_value_type_integer_numeric
      case 73: // ir_value_type_long_integer_numeric
      case 74: // ir_value_type_short_integer_numeric
        value.copy< corevm::IRValueType > (v);
        break;

      case 43: // metadata_def
        value.copy< corevm::ir::MetadataPair > (v);
        break;

      case 24: // FLOATINGNUM
        value.copy< double > (v);
        break;

      case 23: // DECIMALINTEGER
        value.copy< int64_t > (v);
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
      case 16: // TYPE_NAME_OBJECT
      case 18: // ARRAY
      case 19: // DEF
      case 20: // TYPE
      case 21: // IDENTIFIER
      case 22: // STRING_LITERAL
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
      case 65: // identifier
        value.copy< std::string > (v);
        break;

      case 53: // basic_block_list
        value.copy< std::vector<corevm::IRBasicBlock> > (v);
        break;

      case 44: // function_def_list
        value.copy< std::vector<corevm::IRClosure> > (v);
        break;

      case 55: // instruction_group
        value.copy< std::vector<corevm::IRInstruction > > (v);
        break;

      case 60: // label_list
      case 61: // label_list_core
        value.copy< std::vector<corevm::IRLabel> > (v);
        break;

      case 63: // operand_list
        value.copy< std::vector<corevm::IROperand> > (v);
        break;

      case 50: // function_arg_list
      case 51: // function_arg_list_core
        value.copy< std::vector<corevm::IRParameter> > (v);
        break;

      case 45: // type_def_list
        value.copy< std::vector<corevm::IRTypeDecl> > (v);
        break;

      case 47: // type_field_list
        value.copy< std::vector<corevm::IRTypeField> > (v);
        break;

      case 42: // metadata_def_list
        value.copy< std::vector<corevm::ir::MetadataPair> > (v);
        break;

      case 58: // instruction_options
      case 59: // instruction_option_list
        value.copy< std::vector<std::string> > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRArrayType v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRBasicBlock v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRClosure v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRIdentifierType v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRInstruction v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRLabel v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRModule v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IROpcode v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IROperand v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRParameter v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRTypeDecl v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRTypeField v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRValue v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::IRValueType v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const corevm::ir::MetadataPair v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const double v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int64_t v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRBasicBlock> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRClosure> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRInstruction > v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRLabel> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IROperand> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRParameter> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRTypeDecl> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::IRTypeField> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<corevm::ir::MetadataPair> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  ir_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<std::string> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  ir_parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  ir_parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 25: // BOOLEAN_CONSTANT
        value.template destroy< bool > ();
        break;

      case 69: // ir_value_type_array
        value.template destroy< corevm::IRArrayType > ();
        break;

      case 54: // basic_block
        value.template destroy< corevm::IRBasicBlock > ();
        break;

      case 49: // function_def
        value.template destroy< corevm::IRClosure > ();
        break;

      case 66: // ir_identifier_type
        value.template destroy< corevm::IRIdentifierType > ();
        break;

      case 56: // instruction
      case 57: // expr
        value.template destroy< corevm::IRInstruction > ();
        break;

      case 62: // label
        value.template destroy< corevm::IRLabel > ();
        break;

      case 41: // input
        value.template destroy< corevm::IRModule > ();
        break;

      case 17: // OPCODE
        value.template destroy< corevm::IROpcode > ();
        break;

      case 64: // operand
        value.template destroy< corevm::IROperand > ();
        break;

      case 52: // function_arg
        value.template destroy< corevm::IRParameter > ();
        break;

      case 46: // type_def
        value.template destroy< corevm::IRTypeDecl > ();
        break;

      case 48: // type_field
        value.template destroy< corevm::IRTypeField > ();
        break;

      case 67: // ir_value
        value.template destroy< corevm::IRValue > ();
        break;

      case 68: // ir_value_type
      case 70: // ir_value_type_string
      case 71: // ir_value_type_floating_numeric
      case 72: // ir_value_type_integer_numeric
      case 73: // ir_value_type_long_integer_numeric
      case 74: // ir_value_type_short_integer_numeric
        value.template destroy< corevm::IRValueType > ();
        break;

      case 43: // metadata_def
        value.template destroy< corevm::ir::MetadataPair > ();
        break;

      case 24: // FLOATINGNUM
        value.template destroy< double > ();
        break;

      case 23: // DECIMALINTEGER
        value.template destroy< int64_t > ();
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
      case 16: // TYPE_NAME_OBJECT
      case 18: // ARRAY
      case 19: // DEF
      case 20: // TYPE
      case 21: // IDENTIFIER
      case 22: // STRING_LITERAL
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
      case 65: // identifier
        value.template destroy< std::string > ();
        break;

      case 53: // basic_block_list
        value.template destroy< std::vector<corevm::IRBasicBlock> > ();
        break;

      case 44: // function_def_list
        value.template destroy< std::vector<corevm::IRClosure> > ();
        break;

      case 55: // instruction_group
        value.template destroy< std::vector<corevm::IRInstruction > > ();
        break;

      case 60: // label_list
      case 61: // label_list_core
        value.template destroy< std::vector<corevm::IRLabel> > ();
        break;

      case 63: // operand_list
        value.template destroy< std::vector<corevm::IROperand> > ();
        break;

      case 50: // function_arg_list
      case 51: // function_arg_list_core
        value.template destroy< std::vector<corevm::IRParameter> > ();
        break;

      case 45: // type_def_list
        value.template destroy< std::vector<corevm::IRTypeDecl> > ();
        break;

      case 47: // type_field_list
        value.template destroy< std::vector<corevm::IRTypeField> > ();
        break;

      case 42: // metadata_def_list
        value.template destroy< std::vector<corevm::ir::MetadataPair> > ();
        break;

      case 58: // instruction_options
      case 59: // instruction_option_list
        value.template destroy< std::vector<std::string> > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  ir_parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  ir_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 25: // BOOLEAN_CONSTANT
        value.move< bool > (s.value);
        break;

      case 69: // ir_value_type_array
        value.move< corevm::IRArrayType > (s.value);
        break;

      case 54: // basic_block
        value.move< corevm::IRBasicBlock > (s.value);
        break;

      case 49: // function_def
        value.move< corevm::IRClosure > (s.value);
        break;

      case 66: // ir_identifier_type
        value.move< corevm::IRIdentifierType > (s.value);
        break;

      case 56: // instruction
      case 57: // expr
        value.move< corevm::IRInstruction > (s.value);
        break;

      case 62: // label
        value.move< corevm::IRLabel > (s.value);
        break;

      case 41: // input
        value.move< corevm::IRModule > (s.value);
        break;

      case 17: // OPCODE
        value.move< corevm::IROpcode > (s.value);
        break;

      case 64: // operand
        value.move< corevm::IROperand > (s.value);
        break;

      case 52: // function_arg
        value.move< corevm::IRParameter > (s.value);
        break;

      case 46: // type_def
        value.move< corevm::IRTypeDecl > (s.value);
        break;

      case 48: // type_field
        value.move< corevm::IRTypeField > (s.value);
        break;

      case 67: // ir_value
        value.move< corevm::IRValue > (s.value);
        break;

      case 68: // ir_value_type
      case 70: // ir_value_type_string
      case 71: // ir_value_type_floating_numeric
      case 72: // ir_value_type_integer_numeric
      case 73: // ir_value_type_long_integer_numeric
      case 74: // ir_value_type_short_integer_numeric
        value.move< corevm::IRValueType > (s.value);
        break;

      case 43: // metadata_def
        value.move< corevm::ir::MetadataPair > (s.value);
        break;

      case 24: // FLOATINGNUM
        value.move< double > (s.value);
        break;

      case 23: // DECIMALINTEGER
        value.move< int64_t > (s.value);
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
      case 16: // TYPE_NAME_OBJECT
      case 18: // ARRAY
      case 19: // DEF
      case 20: // TYPE
      case 21: // IDENTIFIER
      case 22: // STRING_LITERAL
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
      case 65: // identifier
        value.move< std::string > (s.value);
        break;

      case 53: // basic_block_list
        value.move< std::vector<corevm::IRBasicBlock> > (s.value);
        break;

      case 44: // function_def_list
        value.move< std::vector<corevm::IRClosure> > (s.value);
        break;

      case 55: // instruction_group
        value.move< std::vector<corevm::IRInstruction > > (s.value);
        break;

      case 60: // label_list
      case 61: // label_list_core
        value.move< std::vector<corevm::IRLabel> > (s.value);
        break;

      case 63: // operand_list
        value.move< std::vector<corevm::IROperand> > (s.value);
        break;

      case 50: // function_arg_list
      case 51: // function_arg_list_core
        value.move< std::vector<corevm::IRParameter> > (s.value);
        break;

      case 45: // type_def_list
        value.move< std::vector<corevm::IRTypeDecl> > (s.value);
        break;

      case 47: // type_field_list
        value.move< std::vector<corevm::IRTypeField> > (s.value);
        break;

      case 42: // metadata_def_list
        value.move< std::vector<corevm::ir::MetadataPair> > (s.value);
        break;

      case 58: // instruction_options
      case 59: // instruction_option_list
        value.move< std::vector<std::string> > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  ir_parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  ir_parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  ir_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  ir_parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  ir_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  ir_parser::by_type::type_get () const
  {
    return type;
  }

  inline
  ir_parser::token_type
  ir_parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  ir_parser::symbol_type
  ir_parser::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_I8 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_I8, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_UI8 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_UI8, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_I16 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_I16, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_UI16 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_UI16, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_I32 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_I32, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_UI32 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_UI32, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_I64 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_I64, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_UI64 (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_UI64, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_SPF (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_SPF, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_DPF (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_DPF, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_BOOLEAN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_BOOLEAN, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_VOID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_VOID, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_STRING, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE_NAME_OBJECT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE_NAME_OBJECT, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_OPCODE (const corevm::IROpcode& v, const location_type& l)
  {
    return symbol_type (token::OPCODE, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_ARRAY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::ARRAY, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_DEF (const std::string& v, const location_type& l)
  {
    return symbol_type (token::DEF, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_TYPE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TYPE, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_IDENTIFIER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::IDENTIFIER, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_STRING_LITERAL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::STRING_LITERAL, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_DECIMALINTEGER (const int64_t& v, const location_type& l)
  {
    return symbol_type (token::DECIMALINTEGER, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_FLOATINGNUM (const double& v, const location_type& l)
  {
    return symbol_type (token::FLOATINGNUM, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_BOOLEAN_CONSTANT (const bool& v, const location_type& l)
  {
    return symbol_type (token::BOOLEAN_CONSTANT, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_COMMA (const std::string& v, const location_type& l)
  {
    return symbol_type (token::COMMA, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_COLON (const std::string& v, const location_type& l)
  {
    return symbol_type (token::COLON, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_SEMICOLON (const std::string& v, const location_type& l)
  {
    return symbol_type (token::SEMICOLON, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_PERCENT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::PERCENT, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_STAR (const std::string& v, const location_type& l)
  {
    return symbol_type (token::STAR, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_POND (const std::string& v, const location_type& l)
  {
    return symbol_type (token::POND, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_ASSIGN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::ASSIGN, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_LPAREN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::LPAREN, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_RPAREN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::RPAREN, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_LBRACKET (const std::string& v, const location_type& l)
  {
    return symbol_type (token::LBRACKET, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_RBRACKET (const std::string& v, const location_type& l)
  {
    return symbol_type (token::RBRACKET, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_LBRACE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::LBRACE, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_RBRACE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::RBRACE, v, l);
  }

  ir_parser::symbol_type
  ir_parser::make_LABEL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::LABEL, v, l);
  }



} // yy
#line 2304 "src/ir/ir_parser.tab.hh" // lalr1.cc:392




#endif // !YY_YY_SRC_IR_IR_PARSER_TAB_HH_INCLUDED
