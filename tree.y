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
     | input line             { printf("4.2\n"); }
;

line:
    '\n'                       { printf("3.1\n"); }
    | expr '\n'                { printf("3.2\n"); }
;

expr:
    tok tok               { printf("2.2\n"); }

tok:
   ID                        { printf("1.1\n"); }
   | AUTOLOAD                { printf("1.2\n"); }
   | NUMBER                  { printf("1.3\n"); }
   | STR                     { printf("1.4\n"); }
   | ANGLE                   { printf("1.5\n"); }
;
%%

#include "tok.c"

void yyerror (const char* s) {
    printf ("%s\n", s);
}

#include <cstring>

int main() {
    yydebug = 0;

    char program[512];
    memset(program, 0, 512);

    const char* s = "function\n";
    strcpy(program, s);
    
    //yy_scan_buffer(program, strlen(program) + 2);
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
