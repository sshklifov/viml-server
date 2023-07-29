%option noyywrap
%x CMD_TRAILING CMD_BODY CMD_NAME

anchor ^[ \t]*
comment ^[ \t]*["].*[\n]

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
scope [gbwtslav]:
scoped_id {scope}{name}
au_id {name}[#]{name}
sid_id "<SID>"{name}
id {name}

attr -[a-z]+(=[^ ]+)?
replace "<line1>"|"<line2>"|"<range>"|"<count>"|"<bang>"|"<mods>"|"<reg>"|"<args>"|"<lt>"|"<q-args>"|"<f-args>"

number [0-9]+

qarg [^ \t\n]+[ \t\n]

%{
#include "Node.h"

extern Node* yylval;

  enum yytokentype
  {
    STR = 258,
    AU_ID,
    SID_ID,
    SCOPED_ID,
    ID,
    NUMBER,
    EQ,
    NOT_EQ,
    LESS_EQ,
    GR_EQ,
    MATCH,
    NOT_MATCH,
    CONCAT,
    AND,
    OR,
    FUNCTION,
    ENDFUNCTION,
    IF,
    ELSE,
    ENDIF,
    LET,
    COMMAND,
    COMMAND_ATTR,
    COMMAND_REPLACE,
    QARGS,
    EX,
    VA_DOTS,
    VA,
    ARROW
  };
%}

%%
{comment} {}

{anchor}"if" {
    return IF;
}

{anchor}"else" {
    return ELSE;
}

{anchor}"endif" {
    return ENDIF;
}

{anchor}"function"[!]? {
    return FUNCTION;
}

{anchor}"endfunction" {
    return ENDFUNCTION;
}

{anchor}"let" {
    return LET;
}

{anchor}"command"[!]? {
    BEGIN(CMD_NAME);
    return COMMAND;
}

{anchor}{id} {
    const char* text = yytext;
    while(text < yytext + yyleng) {
        if (*text != ' ' && *text != '\t') {
            break;
        }
        ++text;
    }
    yylval = new LexemNode(text);
    BEGIN(CMD_TRAILING);
    return EX;
}

<CMD_NAME>{attr} {
    yylval = new LexemNode(yytext);
    return COMMAND_ATTR;
}

<CMD_NAME>{id} {
    yylval = new LexemNode(yytext);
    BEGIN(CMD_TRAILING);
    return ID;
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

{sid_id} {
    yylval = new LexemNode(yytext);
    return SID_ID;
}

"a:"{number}? {
    yylval = new LexemNode(yytext);
    return VA;
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

"=~" {
    return MATCH;
}

"!~" {
    return NOT_MATCH;
}

".." {
    return CONCAT;
}

"..." {
    return VA_DOTS;
}

"->" {
    return ARROW;
}

{number} {
    yylval = new LexemNode(yytext);
    return NUMBER;
}

<CMD_TRAILING>[ \t]+ {
    BEGIN(CMD_BODY);
}

<CMD_BODY>[^\n]+ {
    yylval = new LexemNode(yytext);
    return QARGS;
}

<*>[ \t] {
    // IGNORE
}

<*>"\n" {
    BEGIN(INITIAL);
    return yytext[0];
}

. {
    return yytext[0];
}
%%
