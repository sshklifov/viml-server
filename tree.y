%{
#define YYDEBUG 1
#define YYFPRINTF fprintf

#include <cstdio>
#include <string>
#include "Node.h"

#define YYSTYPE Node*
void yyerror(const char*);
int yylex();

Node* root = NULL;
%}

%token ID AUTOLOAD NUMBER STR ANGLE

%%
input:
     %empty                   { $$ = nullptr; }
     | line input             { root = new LineNode($1, $2); $$ = root; }
;

line:
    '\n'                          { $$ = nullptr; }
    | command '\n'                { $$ = $1; }
;

command:
       tok qargs           { $$ = new CommandNode($1, $2); }
;

qargs:
     %empty              { $$ = nullptr; }
     | term qargs        { $$ = new QargsNode($1, $2); }
;

term:
    tok '(' fargs ')'    { $$ = new TermNode($1, $3); }
    | tok               { $$ = $1; }

fargs:
    %empty                  { $$ = nullptr; }
    | term                  { $$ = new FargsNode($1, nullptr); }
    | term ',' fargs        { $$ = new FargsNode($1, $3); }

tok:
   ID                        { $$ = yylval; }
   | AUTOLOAD                { $$ = yylval; }
   | NUMBER                  { $$ = yylval; }
   | STR                     { $$ = yylval; }
   | ANGLE                   { $$ = yylval; }
;
%%

#include "tok.c"

void yyerror (const char* s) {
    printf ("%s\n", s);
}

#include <cstring>

int main() {
    yydebug = 0;
    
    const int do_parse = 1;
    if (do_parse) {
        yyparse();
        std::string s = root->getString();
        printf("%s\n", s.c_str());
    } else {
        int yychar = 0;
        do {
            yychar = yylex();
            yysymbol_kind_t tr = YYTRANSLATE(yychar);
            
            printf("Lex=%s\n", yysymbol_name(tr));
        }
        while (yychar != 0);
    }


    return 0;
}
