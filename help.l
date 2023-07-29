%option noyywrap

blank [ \t]*
comment ^{blank}"--"[^\n]*[\n]

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
number [0-9]+

%{
enum yytokentype
{
    COMMAND = 258,
    ENUM,
    FLAGS_FUN,
    ADDR_TYPE,
    FUNC,
    PREVIEW_FUNC,
    BEGIN_BLOCK,
    BIT_OR,
    ID,
    STR
};

#include <string>
std::string match_capture;
%}

%%
{comment} {
    // Ignore
}

"command" {
    return COMMAND;
}

"enum" {
    return ENUM;
}

"flags" {
    return FLAGS_FUN;
}

"addr_type" {
    return ADDR_TYPE;
}

"func" {
    return FUNC;
}

"preview_func" {
    return PREVIEW_FUNC;
}

"module.cmds" {
    return BEGIN_BLOCK;
}

"bit.bor" {
    return BIT_OR;
}

{name} {
    match_capture = yytext;
    return ID;
}

{str} {
    match_capture.assign(yytext + 1, yyleng - 2);
    return STR;
}

{quote} {
    match_capture.assign(yytext + 1, yyleng - 2);
    return STR;
}

"\n" {
    // IGNORE
}

[ \t] {
    // IGNORE
}

. {
    return yytext[0];
}
%%
