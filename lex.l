%option noyywrap
%x CMD_TRAILING CMD_BODY CMD_NAME

comment ^["].*[\n]
anchor ^[ \t]*

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

qarg [^ \t\n]+[ \t\n]

%{
#include "Node.h"

extern Node* yylval;

  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    STR = 258,                     /* STR  */
    AU_ID = 259,                   /* AU_ID  */
    SCOPED_ID = 260,               /* SCOPED_ID  */
    ID = 261,                      /* ID  */
    NUMBER = 262,                  /* NUMBER  */
    EQ = 263,                      /* EQ  */
    NOT_EQ = 264,                  /* NOT_EQ  */
    LESS_EQ = 265,                 /* LESS_EQ  */
    GR_EQ = 266,                   /* GR_EQ  */
    MATCH = 267,                   /* MATCH  */
    NOT_MATCH = 268,               /* NOT_MATCH  */
    CONCAT = 269,                  /* CONCAT  */
    AND = 270,                     /* AND  */
    OR = 271,                      /* OR  */
    FUNCTION = 272,                /* FUNCTION  */
    ENDFUNCTION = 273,             /* ENDFUNCTION  */
    IF = 274,                      /* IF  */
    ELSE = 275,                    /* ELSE  */
    ENDIF = 276,                   /* ENDIF  */
    LET = 277,                     /* LET  */
    COMMAND = 278,                 /* COMMAND  */
    COMMAND_ATTR = 279,            /* COMMAND_ATTR  */
    COMMAND_REPLACE = 280,         /* COMMAND_REPLACE  */
    QARGS = 281,                   /* QARGS  */
    EX = 282                       /* EX */
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
