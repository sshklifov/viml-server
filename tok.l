%option noyywrap

comment ^["].*[\n]
space [ ]

angle [<][^ <>]+[>] 

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
scope [gbwtslav]:
scoped_id {scope}{name}
bang_id {name}[!]
au_id {name}[#]{name}
id {name}

number [0-9]+
forward .|"\n"

%{
#include "Node.h"
%}

%%
{comment} {}
{space} {}

"if" {
    return IF;
}

"endif" {
    return ENDIF;
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

{bang_id} {
    yylval = new LexemNode(yytext);
    return BANG_ID;
}

{au_id} {
    yylval = new LexemNode(yytext);
    return AU_ID;
}

{scoped_id} {
    yylval = new LexemNode(yytext);
    return SCOPED_ID;
}

{id} {
    yylval = new LexemNode(yytext);
    return ID;
}

{number} {
    yylval = new LexemNode(yytext);
    return NUMBER;
}

{forward} {
    return yytext[0];
}
%%

// TODO comment ignore
// TODO empty lines ignore
