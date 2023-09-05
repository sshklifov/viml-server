%option noyywrap
%option prefix="eval"

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
curly_name [a-zA-Z_{}][a-zA-Z0-9_{}]*
scope [a-zA-Z]:

id {scope}?{curly_name}
sid_id "<SID>"{curly_name}
au_id {scope}?({curly_name}[#])+{curly_name}
opt_id "&"{scope}?{curly_name}
reg_id "@"{curly_name}
env_id "$"{curly_name}
va_id "a:"{number}?

radix "0x"|"0X"|"0b"|"0B"
number {radix}?[0-9]+
float {number}"."{number}([eE][+-]?{number})?
blob "0z"[0-9a-fA-F.]+

%{
#include "EvalParser.hpp"
%}

%%

"&&" {
    return eval::parser::token::AND;
}

"||" {
    return eval::parser::token::OR;
}

"==" {
    return eval::parser::token::EQ;
}

"!=" {
    return eval::parser::token::NOT_EQ;
}

">" {
    return eval::parser::token::GR;
}

">=" {
    return eval::parser::token::GR_EQ;
}

"<" {
    return eval::parser::token::LESS;
}

"<=" {
    return eval::parser::token::LESS_EQ;
}

"=~" {
    return eval::parser::token::MATCH;
}

"!~" {
    return eval::parser::token::NOT_MATCH;
}

"==#" {
    return eval::parser::token::EQ_CASE;
}

"!=#" {
    return eval::parser::token::NOT_EQ_CASE;
}

">#" {
    return eval::parser::token::GR_CASE;
}

">=#" {
    return eval::parser::token::GR_EQ_CASE;
}

"<#" {
    return eval::parser::token::LESS_CASE;
}

"<=#" {
    return eval::parser::token::LESS_EQ_CASE;
}

"=~#" {
    return eval::parser::token::MATCH_CASE;
}

"!~#" {
    return eval::parser::token::NOT_MATCH_CASE;
}

"==?" {
    return eval::parser::token::EQ_ICASE;
}

"!=?" {
    return eval::parser::token::NOT_EQ_ICASE;
}

">?" {
    return eval::parser::token::GR_ICASE;
}

">=?" {
    return eval::parser::token::GR_EQ_ICASE;
}

"<?" {
    return eval::parser::token::LESS_ICASE;
}

"<=?" {
    return eval::parser::token::LESS_EQ_ICASE;
}

"=~?" {
    return eval::parser::token::MATCH_ICASE;
}

"!~?" {
    return eval::parser::token::NOT_MATCH_ICASE;
}

"is" {
    return eval::parser::token::IS;
}

"isnot" {
    return eval::parser::token::ISNOT;
}

".." {
    return eval::parser::token::CONCAT;
}

{number} {
    return eval::parser::token::NUMBER;
}

{float} {
    return eval::parser::token::FLOAT;
}

{blob} {
    return eval::parser::token::BLOB;
}

{str} {
    return eval::parser::token::STR;
}

{quote} {
    return eval::parser::token::STR;
}

"->" {
    return eval::parser::token::ARROW;
}

{va_id} {
    return eval::parser::token::VA_ID;
}

{sid_id} {
    return eval::parser::token::SID_ID;
}

{au_id} {
    return eval::parser::token::AUTOLOAD_ID;
}

{opt_id} {
    return eval::parser::token::OPTION_ID;
}

{reg_id} {
    return eval::parser::token::REGISTER_ID;
}

{env_id} {
    return eval::parser::token::ENV_ID;
}

{id} {
    return eval::parser::token::ID;
}

"\n" {
    BEGIN(INITIAL);
    return yytext[0];
}

. {
    return yytext[0];
}
%%
