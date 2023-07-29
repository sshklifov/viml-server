%option noyywrap
%option yylineno
%x CMD_TRAILING CMD_BODY CMD_NAME

anchor ^[ \t]*
comment ^[ \t]*["].*[\n]

str_loop [ -~]{-}[\\"]
str ["]({str_loop}*|\\.)*["]
quote_loop [ -~]{-}[\\']
quote [']({quote_loop}*|\\.)*[']

name [a-zA-Z_][a-zA-Z0-9_]*
scope [gbwtslav]:
id {scope}?{name}
sid_id "<SID>"{name}
au_id {scope}?({name}[#])+{name}

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
    ID,
    NUMBER,
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
    COMMAND_ATTR,
    COMMAND_REPLACE,
    QARGS,
    EX,
    VA_DOTS,
    VA,
    ARROW
  };

bool isVimCmd(const char* name, int n, const char* vimShort, const char* vimLong) {
    int shortLen = strlen(vimShort);
    int longLen = strlen(vimLong);
    if (n < shortLen || n > longLen) {
        return false;
    }
    return strncmp(name, vimLong, n) == 0;
}
%}

%%
{comment} {}

{anchor}{id}"!"? {
    const char* id = yytext;
    int len = yyleng;
    while(id < yytext + yyleng) {
        if (*id != ' ' && *id != '\t') {
            break;
        }
        ++id;
        --len;
    }
    if (id[len - 1] == '!') {
        --len;
    }
    
    if (isVimCmd(id, len, "if", "if")) {
        return IF;
    }
    if (isVimCmd(id, len, "el", "else")) {
        return ELSE;
    }
    if (isVimCmd(id, len, "elsei", "elseif")) {
        return ELSEIF;
    }
    if (isVimCmd(id, len, "en", "endif")) {
        return ENDIF;
    }
    if (isVimCmd(id, len, "wh", "while")) {
        return WHILE;
    }
    if (isVimCmd(id, len, "endw", "endwhile")) {
        return ENDWHILE;
    }
    if (isVimCmd(id, len, "for", "for")) {
        return FOR;
    }
    if (isVimCmd(id, len, "endfo", "endfor")) {
        return ENDFOR;
    }
    if (isVimCmd(id, len, "fu", "function")) {
        return FUNCTION;
    }
    if (isVimCmd(id, len, "endf", "endfunction")) {
        return ENDFUNCTION;
    }
    if (isVimCmd(id, len, "let", "let")) {
        return LET;
    }
    if (isVimCmd(id, len, "com", "command")) {
        BEGIN(CMD_NAME);
        return COMMAND;
    }

    yylval = new LexemNode(id);
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

{sid_id} {
    yylval = new LexemNode(yytext);
    return SID_ID;
}

"a:"{number}? {
    yylval = new LexemNode(yytext);
    return VA;
}

{au_id} {
    yylval = new LexemNode(yytext);
    return AU_ID;
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

"%=" {
    return MOD_EQ;
}

".=" {
    return CON_EQ;
}

"..=" {
    return CON_EQ;
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
