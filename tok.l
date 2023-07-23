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
    printf("id");
}

{comment} {
    printf("com");
}

{autoload} {
    printf("aufun");
}

{number} {
    printf("num");
}

{str} {
    printf("str()");
}

{quote} {
    printf("q()");
}

{angle} {
    printf("angle");
}
%%

int main() {
    yylex();
    //printf("{\n");
    //for (std::string s : toks) {
        //printf("\'%s\'\n", s.c_str());
    //}
    //printf("}\n");
    return 0;
}
