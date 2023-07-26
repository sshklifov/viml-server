%{
#define YYDEBUG 1
#define YYFPRINTF fprintf
#define YYPRINT

#include <cstdio>
#include <string>
#include "Node.h"

#define YYSTYPE Node*
void yyerror(const char*);
int yylex();

Node* root = NULL;
%}

%token STR ANGLE BANG_ID AU_ID SCOPED_ID ID NUMBER

%%
input: %empty                 { $$ = nullptr; }
     | line input             { root = new LineNode($1, $2); $$ = root; }
;

line: '\n'                        { $$ = nullptr; }
    | command '\n'                { $$ = $1; }
;

command: ID qargs                  { $$ = new CommandNode($1, $2); }
       | BANG_ID qargs             { $$ = new CommandNode($1, $2); }
;

qargs: %empty            { $$ = nullptr; }
     | term qargs        { $$ = new QargsNode($1, $2); }
;

term: fname '(' fargs ')'      { $$ = new FunCallNode($1, $3); }
    | '!' term                 { $$ = new PrefixOpNode($2, "!"); }
    | term '*' term            { $$ = new InfixOpNode($1, $3, "*"); }
    | term '/' term            { $$ = new InfixOpNode($1, $3, "/"); }
    | term '%' term            { $$ = new InfixOpNode($1, $3, "%"); }
    | term '.' term            { $$ = new InfixOpNode($1, $3, "."); }
    | term '+' term            { $$ = new InfixOpNode($1, $3, "+"); }
    | term '-' term            { $$ = new InfixOpNode($1, $3, "-"); }
    | term '<' term            { $$ = new InfixOpNode($1, $3, "<"); }
    | term '>' term            { $$ = new InfixOpNode($1, $3, ">"); }
    | term '<' '=' term        { $$ = new InfixOpNode($1, $4, "<="); }
    | term '>' '=' term        { $$ = new InfixOpNode($1, $4, ">="); }
    | term '=' '=' term        { $$ = new InfixOpNode($1, $4, "=="); }
    | term '!' '=' term        { $$ = new InfixOpNode($1, $4, "!="); }
    // TODO MORE with regex matching shiet
    | term '&' '&' term        { $$ = new InfixOpNode($1, $4, "&&"); }
    | term '|' '|' term        { $$ = new InfixOpNode($1, $4, "&&"); }
    | var                  { $$ = $1; }
    | STR                  { $$ = $1; }
    | ANGLE                { $$ = $1; }
    | NUMBER               { $$ = $1; }
;

var: ID              { $$ = $1; }
   | SCOPED_ID       { $$ = $1; }

fargs: %empty                { $$ = nullptr; }
     | term                  { $$ = new FargsNode($1, nullptr); }
     | term ',' fargs        { $$ = new FargsNode($1, $3); }
;

fname: ID                   { $$ = $1; }
     | AU_ID                { $$ = $1; }
     | SCOPED_ID            { $$ = $1; }
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
            int tr = yytoknum[yychar];
            
            printf("Lex=%s\n", yytname[tr]);
        }
        while (yychar != 0);
    }


    return 0;
}
