%option noyywrap

comment ^["].*$

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

%{
#include <string>
#include <vector>
std::vector<std::string> toks;
%}

%%
{id} {
    return ID;
    // printf("id(%s)", yytext);
}

{autoload} {
    return AUTOLOAD;
    // printf("aufun(%s)", yytext);
}

{number} {
    return NUMBER;
    // printf("num(%s)", yytext);
}

{str} {
    return STR;
    // printf("str(%s)", yytext);
}

{quote} {
    return STR;
    // printf("str(%s)", yytext);
}

{angle} {
    return ANGLE;
    // printf("angle(%s)", yytext);
}
%%

// TODO comment ignore
