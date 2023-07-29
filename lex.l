%option noyywrap

comment ^["].*[\n]
space [ ]

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
scope [gbwtslav]:
scoped_id {scope}{name}
au_id {name}[#]{name}
id {name}

attr -[a-z]+(=[^ ]+)?
replace "<line1>"|"<line2>"|"<range>"|"<count>"|"<bang>"|"<mods>"|"<reg>"|"<args>"|"<lt>"|"<q-args>"|"<f-args>"

number [0-9]+
forward .|"\n"

%{
#include "Node.h"

extern Node* yylval;

enum yytokentype {
    STR = 258, AU_ID, SCOPED_ID, ID, NUMBER,
    EQ, NOT_EQ, LESS_EQ, GR_EQ, MATCH, NOT_MATCH, CONCAT,
    AND, OR,
    FUNCTION, ENDFUNCTION, IF, ENDIF,
    LET,
    COMMAND, COMMAND_ATTR, COMMAND_REPLACE,
    SET_COMMAND,
};
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

"function"[!]? {
    return FUNCTION;
}

"endfunction" {
    return ENDFUNCTION;
}

"let" {
    return LET;
}

"command"[!]? {
    return COMMAND;
}

"set "[^\n]+ {
    yylval = new LexemNode(yytext);
    return SET_COMMAND;
}

{attr} {
    yylval = new LexemNode(yytext);
    return COMMAND_ATTR;
}

{replace} {
    yylval = new LexemNode(yytext);
    return COMMAND_REPLACE;
}

{str} {
    yylval = new LexemNode(yytext);
    return STR;
}

{quote} {
    yylval = new LexemNode(yytext);
    return STR;
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

"==" {
    return EQ;
}

"!=" {
    return NOT_EQ;
}

">=" {
    return GR_EQ;
}

"<=" {
    return LESS_EQ;
}

"&&" {
    return AND;
}

"||" {
    return OR;
}

"=!" {
    return MATCH;
}

"!~" {
    return NOT_MATCH;
}

".." {
    return CONCAT;
}

{number} {
    yylval = new LexemNode(yytext);
    return NUMBER;
}

{forward} {
    return yytext[0];
}
%%
