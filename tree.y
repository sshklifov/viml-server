%{
#define YYDEBUG 1
#define YYFPRINTF fprintf
#include <cstdio>
void yyerror(const char*);
int yylex();
%}

%token ID AUTOLOAD NUMBER STR ANGLE

%%
input:
     %empty
     | input line             { printf("4\n"); }
;

line:
    '\n'
    | expr '\n'                { printf("3\n"); }
;

expr:
    tok                      { printf("2\n"); }
    | expr tok               { printf("2\n"); }

tok:
   ID                        { printf("1\n"); }
   | AUTOLOAD
   | NUMBER
   | STR
   | ANGLE                   { printf("1\n"); }
;
%%

// #include "tok.c"
int yylex() {
    static int invocation = 0;
    int cnt = invocation;
    ++invocation;

    if (cnt == 0) {
        return ID;
    } else if (cnt == 1) {
        //return ' ';
        return '\n';
    } else if (cnt == 2) {
        //return ID;
        return YYEOF;
    } else if (cnt == 3) {
        //return '\n';
        return YYEOF;
    } else if (cnt == 4) {
        return YYEOF;
    } else if (cnt == 5) {
        return YYEOF;
    } else if (cnt == 6) {
        return YYEOF;
    } else {
        return YYEOF;
    }
}

void yyerror (const char* s) {
    printf ("%s\n", s);
}

#include <cstring>

int main() {
    yydebug = 0;

    // char program[512];
    // memset(program, 0, 512);

    // const char* s = "function\n";
    // strcpy(program, s);
    
    // yy_scan_buffer(program, strlen(program) + 2);
    yyparse();

    // int a = yylex();
    // printf("a=%d\n", a);
    // int b = yylex();
    // printf("b=%d\n", b);
    // int c = yylex();
    // printf("c=%d\n", c);
    // int d = yylex();
    // printf("d=%d\n", d);
    // int e = yylex();
    // printf("e=%d\n", e);
    // int f = yylex();
    // printf("f=%d\n", f);

    return 0;
}
