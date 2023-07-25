%option noyywrap

comment ^["].*$
space [ ]

angle [<][^ <>]+[>] 

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
scope [gbwtslav]:
id {scope}?{name}
autoload {name}+[#]{name}+

number [0-9]+
forward .|"\n"

%{
#include "Node.h"
%}

%%
{comment} {}
{space} {}

{id} {
    yylval = new LexemNode(yytext);
    return ID;
}

{autoload} {
    yylval = new LexemNode(yytext);
    return AUTOLOAD;
}

{number} {
    yylval = new LexemNode(yytext);
    return NUMBER;
}

{str} {
    yylval = new LexemNode(yytext);
    return STR;
}

{quote} {
    yylval = new LexemNode(yytext);
    return STR;
}

{angle} {
    yylval = new LexemNode(yytext);
    return ANGLE;
}

{forward} {
    return yytext[0];
}
%%

// TODO comment ignore
// TODO empty lines ignore
