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

number [0-9]+
float [+-]?{number}[.]{number}([eE][+-]?{number})?
blob "0z"[0-9a-fA-F.]+

attr -[a-z]+(=[^ ]+)?
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
    COMMAND_ATTR,
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

^{any_space}":"?{name}"!"?/{space} {
    // Return any_space to the lexer, IDX_SQB will consume it
    const char* id = yytext;
    int len = yyleng;
    // Remove leading spaces
    while(id < yytext + yyleng) {
        if (*id != ' ' && *id != '\t') {
            break;
        }
        ++id;
        --len;
    }
    // Optional ':' before command name
    if (*id == ':') {
        ++id;
        --len;
    }
    // Remove trailing spaces. Optional '!' after command name
    while (id[len - 1] == ' ' || id[len - 1] == '\t' || id[len - 1] == '!') {
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
        BEGIN(COM);
        return COMMAND;
    }

    yylval = new LexemNode(id);
    BEGIN(EX_LEADING);
    return EX;
}

<COM>{attr}\{space} {
    yylval = new LexemNode(yytext);
    return COMMAND_ATTR;
}

<COM>{name} {
    yylval = new LexemNode(yytext);
    BEGIN(EX_LEADING);
    return ID;
}

<EX_LEADING>[ \t]+ {
    BEGIN(EX_BODY);
}

<EX_BODY>[^\n]+ {
    yylval = new LexemNode(yytext);
    return QARGS;
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
