%option noyywrap
%option yylineno
%option stack
%x EX_LEADING EX_BODY COM

space [ \t]
any_space {space}*
comment ^{any_space}["].*[\n]

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
number [+-]?{radix}?[0-9]+

float [+-]?{number}"."{number}([eE][+-]?{number})?

blob "0z"[0-9a-fA-F.]+

%{
#include "Node.h"

extern Node* yylval;

  enum yytokentype
  {
    STR = 258,
    AU_ID,
    SID_ID,
    ID,
    OPT_ID,
    REG_ID,
    ENV_ID,
    NUMBER,
    FLOAT,
    BLOB,
    SPC_SQB,
    IDX_SQB,
    EQ,
    NOT_EQ,
    LESS_EQ,
    GR_EQ,
    MATCH,
    NOT_MATCH,
    CONCAT,
    ADD_EQ,
    SUB_EQ,
    MUL_EQ,
    DIV_EQ,
    MOD_EQ,
    CON_EQ,
    AND,
    OR,
    FUNCTION,
    ENDFUNCTION,
    IF,
    ELSE,
    ELSEIF,
    ENDIF,
    WHILE,
    ENDWHILE,
    FOR,
    ENDFOR,
    LET,
    COMMAND,
    QARGS,
    EX,
    VA_DOTS,
    VA,
    ARROW
  };
%}

%%
<COM>{name} {
    yylval = new LexemNode(yytext);
    BEGIN(EX_LEADING);
    return ID;
}

{str} {
    yylval = new LexemNode(yytext);
    return STR;
}

{quote} {
    yylval = new LexemNode(yytext);
    return STR;
}

{va_id} {
    yylval = new LexemNode(yytext);
    return VA;
}

{sid_id} {
    yylval = new LexemNode(yytext);
    return SID_ID;
}

{au_id} {
    yylval = new LexemNode(yytext);
    return AU_ID;
}

{opt_id} {
    yylval = new LexemNode(yytext);
    return OPT_ID;
}

{reg_id} {
    yylval = new LexemNode(yytext);
    return REG_ID;
}

{env_id} {
    yylval = new LexemNode(yytext);
    return ENV_ID;
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

"+=" {
    return ADD_EQ;
}

"-=" {
    return SUB_EQ;
}

"*=" {
    return MUL_EQ;
}

"/=" {
    return DIV_EQ;
}

".." {
    return CONCAT;
}

"->" {
    return ARROW;
}

{space}"[" {
    return SPC_SQB;
}

"[" {
    return IDX_SQB;
}

{number} {
    yylval = new LexemNode(yytext);
    return NUMBER;
}

{float} {
    yylval = new LexemNode(yytext);
    return FLOAT;
}

{blob} {
    yylval = new LexemNode(yytext);
    return BLOB;
}

<*>{space} {
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
