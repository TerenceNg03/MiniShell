%{
#include <iostream>
#include "y.tab.hh"
#include "scanner.hh"
#include <filesystem>
namespace fs = std::filesystem;
#define yyterminate() return( parse::Parser::make_END )
#define loc (*driver.location_)
#ifndef register
#define register
#endif

#define STEP()                                      \
do {                                              \
loc.step ();                      \
} while (0)

#define COL(Col)                                    \
loc.columns (Col)

#define LINE(Line)                                    \
do{                                                \
loc.lines (Line);                    \
} while (0)

#define YY_USER_ACTION                                \
{COL(yyleng); driver.scanner_->current_col =driver.scanner_->current_col_end; driver.scanner_->current_col_end += yyleng;}


%}

%option noinput nounput 8bit nodefault
%option debug
%option noyywrap
%option prefix="parse"
%option yylineno
%option c++
%%

bg return parse::Parser::make_BG(loc);
cd return parse::Parser::make_CD(loc);
echo return parse::Parser::make__ECHO(loc);
exec return parse::Parser::make_EXEC(loc);
exit return parse::Parser::make_EXIT(loc);
fg return parse::Parser::make_FG(loc);
set return parse::Parser::make_SET(loc);
shift return parse::Parser::make_SHIFT(loc);
test return parse::Parser::make_TEST(loc);
time return parse::Parser::make_TIME(loc);
umask return parse::Parser::make_UNMASK(loc);
unset return parse::Parser::make_UNSET(loc);
jobs return parse::Parser::make_JOBS(loc);

[0-9]{1,} return parse::Parser::make_NUM(atoi(yytext),loc);

[\'\"][.\\]*[\'\"]  return parse::Parser::make_STR(std::string(yytext),loc);

[a-zA-Z0-9\-\+_]{1,} return parse::Parser::make_ID(std::string(yytext),loc);

"$"[a-zA-Z0-9\-]+ return parse::Parser::make_VAR(std::string(yytext+1),loc);

[a-zA-Z0-9\-\./~\+]{1,} return parse::Parser::make__PATH(std::string(yytext),loc);


"&" return parse::Parser::make_BACK(loc);
"|" return parse::Parser::make_PIPE(loc);
"=" return parse::Parser::make_ASSIGN(loc);
">" return parse::Parser::make_GREATER(loc);
"<" return parse::Parser::make_LESSER(loc);
">>" return parse::Parser::make_RIGHT_SHIFT(loc);
"<<" return parse::Parser::make_LEFT_SHIFT(loc);
";" return parse::Parser::make_SEMICOLON(loc);


\n return parse::Parser::make_NEWLINE(loc);

<<EOF>> unput('t');unput('i');unput('x');unput('e');//std::cout<<"exit\n";
[ \t]+ /* ignore whitespace */
. return parse::Parser::make_UNKNOWN(std::string(yytext),loc);

%%

namespace parse
{

    Scanner::Scanner()
    {
        log = fopen(logfile,"w");
        if(!log)perror("Scanner Error : Cannot create log!\n");
    }

    Scanner::~Scanner()
    {
        fclose(log);
    }

    void Scanner::set_debug(bool b)
    {
        yy_flex_debug = b;
    }
    
    void Scanner::reset_current_col() {
        current_col = current_col_end = 0;
        fprintf(log,"%s",current_line.c_str());
        current_line_sent = current_line_len = 0;
        current_line = "";
    }

    size_t Scanner::LexerInput(char* buf, size_t max_size) {
        ssize_t avail = current_line_len - current_line_sent;
        if (!avail) {
            current_line_sent = 0;
            getline(yyin,current_line);
            avail = current_line.length();
            if(yyin.bad()){
                avail = 0;
                return -1;
            }else{
                if(!yyin.eof()){
                    current_line+='\n';
                    avail++;
                }
            }
            current_line_len = avail;
        }
        if (avail > max_size) avail = max_size;
        memcpy(buf, current_line.c_str() + current_line_sent, avail);

        current_line_sent += avail;
        if (!avail) reset_current_col();
        return avail;
    }



}

#ifdef yylex
# undef yylex
#endif


int parseFlexLexer::yylex()
{
  std::cerr << "call parseFlexLexer::yylex()!" << std::endl;
  return 0;
}
