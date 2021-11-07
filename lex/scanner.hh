
#ifndef SCANPIT_HH_
# define SCANPIT_HH_

# include "y.tab.hh"
# include "minishell.hpp"


# ifndef YY_DECL
#  define YY_DECL parse::Parser::symbol_type                         \
parse::Scanner::yylex(    \
parse::Driver& driver, minishell& shell)
# endif


# ifndef __FLEX_LEXER_H
#  define yyFlexLexer parseFlexLexer
#  include <FlexLexer.h>
#  undef yyFlexLexer
# endif


namespace parse
{
    
    class Scanner : public parseFlexLexer
    {
        public:
            Scanner();

            virtual ~Scanner();

            virtual Parser::symbol_type yylex(
                Driver& driver,
                minishell& shell);

            void set_debug(bool b);

            std::string current_line="";
            size_t current_col = 0, current_col_end = 0;
            ssize_t current_line_sent = 0;
            ssize_t current_line_len = 0;

        virtual size_t LexerInput( char* buf, size_t max_size );
            void reset_current_col();
            const char* logfile = "./history.log";
            FILE* log;

    };
}


#endif // SCANPIT_HH_
