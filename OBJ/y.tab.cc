// A Bison parser, made by GNU Bison 3.7.6.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 3 "parser.y"

#include <stdio.h>
#include <string>
#include "y.tab.hh"
#include "scanner.hh"
#define yylex driver.scanner_->yylex

#line 49 "OBJ/y.tab.cc"


#include "y.tab.hh"




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


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
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
    while (false)
# endif


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
      *yycdebug_ << '\n';                       \
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
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 38 "parser.y"
namespace parse {
#line 147 "OBJ/y.tab.cc"

  /// Build a parser object.
  Parser::Parser (Driver &driver_yyarg, minishell& shell_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg),
      shell (shell_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        value.copy< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        value.copy< command* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        value.copy< std::pair<int,int> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        value.copy< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        value.copy< std::vector<std::tuple<int,int,std::string>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        value.move< bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        value.move< command* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        value.move< std::pair<int,int> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        value.move< std::vector<std::string> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        value.move< std::vector<std::tuple<int,int,std::string>> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        value.YY_MOVE_OR_COPY< command* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        value.YY_MOVE_OR_COPY< std::pair<int,int> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        value.YY_MOVE_OR_COPY< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        value.YY_MOVE_OR_COPY< std::vector<std::tuple<int,int,std::string>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        value.move< command* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        value.move< std::pair<int,int> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        value.move< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        value.move< std::vector<std::tuple<int,int,std::string>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        value.copy< command* > (that.value);
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        value.copy< std::pair<int,int> > (that.value);
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        value.copy< std::vector<std::string> > (that.value);
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        value.copy< std::vector<std::tuple<int,int,std::string>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        value.move< command* > (that.value);
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        value.move< int > (that.value);
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        value.move< std::pair<int,int> > (that.value);
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        value.move< std::vector<std::string> > (that.value);
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        value.move< std::vector<std::tuple<int,int,std::string>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
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

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location, driver, shell));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

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
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_BACKGROUND: // BACKGROUND
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_BUILT_IN: // BUILT_IN
        yylhs.value.emplace< command* > ();
        break;

      case symbol_kind::S_RD_I_AP: // RD_I_AP
      case symbol_kind::S_RD_I: // RD_I
      case symbol_kind::S_RD_O: // RD_O
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_RD_OP: // RD_OP
        yylhs.value.emplace< std::pair<int,int> > ();
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_BIN: // BIN
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_ARGUMENTS: // ARGUMENTS
        yylhs.value.emplace< std::vector<std::string> > ();
        break;

      case symbol_kind::S_REDIRECTION: // REDIRECTION
        yylhs.value.emplace< std::vector<std::tuple<int,int,std::string>> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // CMDS: %empty
#line 62 "parser.y"
                 {std::cout<<"\nMyshell By Adam Wu\n\nmyshell "<<fs::current_path().filename().string()<<" $ ";}
#line 848 "OBJ/y.tab.cc"
    break;

  case 3: // CMDS: CMDS CMD NEWLINE
#line 62 "parser.y"
                                                                                                                                  {std::cout<<"myshell "<<fs::current_path().filename().string()<<" $ ";}
#line 854 "OBJ/y.tab.cc"
    break;

  case 4: // CMD: BUILT_IN ARGUMENTS REDIRECTION BACKGROUND
#line 65 "parser.y"
{
    yystack_[3].value.as < command* > ()->set_args(yystack_[2].value.as < std::vector<std::string> > ());
    yystack_[3].value.as < command* > ()->execute();
}
#line 863 "OBJ/y.tab.cc"
    break;

  case 7: // BUILT_IN: BG
#line 75 "parser.y"
   {yylhs.value.as < command* > ()=new command(shell);}
#line 869 "OBJ/y.tab.cc"
    break;

  case 8: // BUILT_IN: CD
#line 76 "parser.y"
     {yylhs.value.as < command* > ()=new cd(shell);}
#line 875 "OBJ/y.tab.cc"
    break;

  case 9: // BUILT_IN: CLR
#line 77 "parser.y"
      {yylhs.value.as < command* > ()=new command(shell);}
#line 881 "OBJ/y.tab.cc"
    break;

  case 10: // BUILT_IN: DIR
#line 78 "parser.y"
      {yylhs.value.as < command* > ()=new command(shell);}
#line 887 "OBJ/y.tab.cc"
    break;

  case 11: // BUILT_IN: _ECHO
#line 79 "parser.y"
        {yylhs.value.as < command* > ()=new command(shell);}
#line 893 "OBJ/y.tab.cc"
    break;

  case 12: // BUILT_IN: EXEC
#line 80 "parser.y"
       {yylhs.value.as < command* > ()=new command(shell);}
#line 899 "OBJ/y.tab.cc"
    break;

  case 13: // BUILT_IN: EXIT
#line 81 "parser.y"
       {YYACCEPT;}
#line 905 "OBJ/y.tab.cc"
    break;

  case 14: // BUILT_IN: FG
#line 82 "parser.y"
     {yylhs.value.as < command* > ()=new command(shell);}
#line 911 "OBJ/y.tab.cc"
    break;

  case 15: // BUILT_IN: MAN
#line 83 "parser.y"
      {yylhs.value.as < command* > ()=new command(shell);}
#line 917 "OBJ/y.tab.cc"
    break;

  case 16: // BUILT_IN: JOBS
#line 84 "parser.y"
       {yylhs.value.as < command* > ()=new command(shell);}
#line 923 "OBJ/y.tab.cc"
    break;

  case 17: // BUILT_IN: SET
#line 85 "parser.y"
      {yylhs.value.as < command* > ()=new command(shell);}
#line 929 "OBJ/y.tab.cc"
    break;

  case 18: // BUILT_IN: SHIFT
#line 86 "parser.y"
        {yylhs.value.as < command* > ()=new command(shell);}
#line 935 "OBJ/y.tab.cc"
    break;

  case 19: // BUILT_IN: TEST
#line 87 "parser.y"
       {yylhs.value.as < command* > ()=new command(shell);}
#line 941 "OBJ/y.tab.cc"
    break;

  case 20: // BUILT_IN: TIME
#line 88 "parser.y"
       {yylhs.value.as < command* > ()=new command(shell);}
#line 947 "OBJ/y.tab.cc"
    break;

  case 21: // BUILT_IN: UNMASK
#line 89 "parser.y"
         {yylhs.value.as < command* > ()=new command(shell);}
#line 953 "OBJ/y.tab.cc"
    break;

  case 22: // BUILT_IN: UNSET
#line 90 "parser.y"
        {yylhs.value.as < command* > ()=new command(shell);}
#line 959 "OBJ/y.tab.cc"
    break;

  case 23: // BIN: NAME
#line 96 "parser.y"
{
    yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
    std::cout<<yystack_[0].value.as < std::string > ()<<std::endl;
}
#line 968 "OBJ/y.tab.cc"
    break;

  case 24: // ARGUMENTS: %empty
#line 102 "parser.y"
            {yylhs.value.as < std::vector<std::string> > () = std::vector<std::string>();}
#line 974 "OBJ/y.tab.cc"
    break;

  case 25: // ARGUMENTS: ARGUMENTS NAME
#line 102 "parser.y"
                                                                {std::swap(yylhs.value.as < std::vector<std::string> > (), yystack_[1].value.as < std::vector<std::string> > ());yylhs.value.as < std::vector<std::string> > ().push_back(yystack_[0].value.as < std::string > ());}
#line 980 "OBJ/y.tab.cc"
    break;

  case 26: // REDIRECTION: %empty
#line 106 "parser.y"
                          {yylhs.value.as < std::vector<std::tuple<int,int,std::string>> > () = std::vector<std::tuple<int,int,std::string>>();}
#line 986 "OBJ/y.tab.cc"
    break;

  case 27: // REDIRECTION: REDIRECTION RD_OP NAME
#line 108 "parser.y"
{
    std::swap(yylhs.value.as < std::vector<std::tuple<int,int,std::string>> > (),yystack_[2].value.as < std::vector<std::tuple<int,int,std::string>> > ());
    yylhs.value.as < std::vector<std::tuple<int,int,std::string>> > ().push_back(std::tuple<int,int,std::string>(yystack_[1].value.as < std::pair<int,int> > ().first,yystack_[1].value.as < std::pair<int,int> > ().second,yystack_[0].value.as < std::string > ()));

}
#line 996 "OBJ/y.tab.cc"
    break;

  case 28: // RD_OP: RD_I
#line 114 "parser.y"
             {yylhs.value.as < std::pair<int,int> > () = std::make_pair(yystack_[0].value.as < int > (),0);}
#line 1002 "OBJ/y.tab.cc"
    break;

  case 29: // RD_OP: RD_O
#line 114 "parser.y"
                                                 {yylhs.value.as < std::pair<int,int> > () = std::make_pair(yystack_[0].value.as < int > (),1);}
#line 1008 "OBJ/y.tab.cc"
    break;

  case 30: // RD_OP: RD_I_AP
#line 114 "parser.y"
                                                                                        {yylhs.value.as < std::pair<int,int> > () = std::make_pair(yystack_[0].value.as < int > (),2);}
#line 1014 "OBJ/y.tab.cc"
    break;

  case 31: // BACKGROUND: %empty
#line 118 "parser.y"
             {yylhs.value.as < bool > () = false;}
#line 1020 "OBJ/y.tab.cc"
    break;

  case 32: // BACKGROUND: BACK
#line 118 "parser.y"
                                  {yylhs.value.as < bool > () = true;}
#line 1026 "OBJ/y.tab.cc"
    break;


#line 1030 "OBJ/y.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
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
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
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
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              else
                goto append;

            append:
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

  std::string
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
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
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -26;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
     -26,     0,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -18,   -26,   -26,   -26,   -25,   -25,   -26,    -1,    -1,   -26,
     -26,   -26,   -26,    -7,   -26,   -26,   -26
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     6,     1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,    24,    24,     3,    26,    26,    25,    31,    31,    32,
      30,    28,    29,     0,     4,     5,    27
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -26,   -26,   -26,   -26,   -26,     3,    -4,   -26,    -2
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     1,    20,    21,    22,    24,    27,    33,    34
  };

  const signed char
  Parser::yytable_[] =
  {
       2,    23,    26,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    29,
      36,    28,    30,    31,    32,    25,    35,    19
  };

  const signed char
  Parser::yycheck_[] =
  {
       0,    19,    27,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    20,
      27,    25,    23,    24,    25,    22,    28,    27
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    29,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    27,
      30,    31,    32,    19,    33,    33,    27,    34,    34,    20,
      23,    24,    25,    35,    36,    36,    27
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    28,    29,    29,    30,    30,    30,    31,    31,    31,
      31,    31,    31,    31,    31,    31,    31,    31,    31,    31,
      31,    31,    31,    32,    33,    33,    34,    34,    35,    35,
      35,    36,    36
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     3,     4,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     2,     0,     3,     1,     1,
       1,     0,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "BG", "CD", "CLR",
  "DIR", "_ECHO", "EXEC", "EXIT", "FG", "MAN", "JOBS", "SET", "SHIFT",
  "TEST", "TIME", "UNMASK", "UNSET", "NEWLINE", "BACK", "PIPE", "UNKNOWN",
  "RD_I_AP", "RD_I", "RD_O", "END", "NAME", "$accept", "CMDS", "CMD",
  "BUILT_IN", "BIN", "ARGUMENTS", "REDIRECTION", "RD_OP", "BACKGROUND", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const signed char
  Parser::yyrline_[] =
  {
       0,    62,    62,    62,    64,    70,    70,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    95,   102,   102,   106,   106,   114,   114,
     114,   118,   118
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
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
      25,    26,    27
    };
    // Last valid token kind.
    const int code_max = 282;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 38 "parser.y"
} // parse
#line 1561 "OBJ/y.tab.cc"

#line 120 "parser.y"


namespace parse
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cout << *driver.location_ << ": " << m << std::flush;
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
