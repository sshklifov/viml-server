%option noyywrap
%s COLON NAME ARGS

range "[range]"|"{range}"
N "[N]"|"{N}"
count "[count]"|"{count}"
line "[line]"|"{line}"

name [a-zA-Z_][a-zA-Z0-9_]*
abbrev_name {name}"["{name}"]"

bang "[!]"
optional "["[^]]+"]"
required "{"[^}]+"}"

%{
static const char* yylval;

enum yytokentype
{
    RANGE = 258,                   /* RANGE  */
    N = 259,                       /* N  */
    COUNT = 260,                   /* COUNT  */
    LINE = 261,                    /* LINE  */
    ID = 262,                      /* ID  */
    ABBREV_ID = 263,               /* ABBREV_ID  */
    BANG = 264,                    /* BANG  */
    OPTIONAL = 265,                /* OPTIONAL  */
    REQUIRED = 266,                /* REQUIRED  */
    COLON = 267                    /* COLON  */
};
%}

%%
<COLON>{range} {
    return RANGE;
}

<COLON>{N} {
    return N;
}

<COLON>{count} {
    return COUNT;
}

<COLON>{line} {
    return LINE;
}

<COLON>{name} {
    //yylval = yytext;
    //printf(":%s\n", yylval.c_str());

    BEGIN(NAME);
    return ID;
}

<COLON>{abbrev_name} {
    //yylval = yytext;
    //printf(":%s\n", yylval.c_str());

    BEGIN(NAME);
    return ABBREV_ID;
}

<NAME>{bang} {
    return BANG;
}

<ARGS>{optional} {
    return OPTIONAL;
}

<ARGS>{required} {
    return REQUIRED;
}

^[:] {
    BEGIN(COLON);
    return COLON;
}

<NAME>[ \t]+ {
    BEGIN(ARGS);
}

<ARGS>[ \t]+ {
    // Continue reading ARGS
}

<*>"\n" {
    BEGIN(0);
}

<*>. {
    BEGIN(0);
}
%%

struct BuiltinCommand {
};

int main() {
    int yychar = 0;
    do {
        yychar = yylex();
        switch (yychar) {
        }
        /*
        if (yychar >= 32 && yychar < 126) {
            printf("Pass-through: %c\n", yychar);
        } else {
            int tr = YYTRANSLATE(yychar);
            printf("Lex=%s\n", yytname[tr]);
        }
        */
    }
    while (yychar != 0);
}
