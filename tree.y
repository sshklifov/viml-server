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
%token EQ NOT_EQ LESS_EQ GR_EQ
%token AND OR
%token BANG_FUNCTION FUNCTION ENDFUNCTION IF ENDIF
%token LET

%left '*'
%left '/'
%left '%'
%left '.'
%left '+'
%left '-'

%left AND
%left OR

%nonassoc '!'
%nonassoc '<'
%nonassoc '>'
%nonassoc EQ
%nonassoc NOT_EQ
%nonassoc LESS_EQ
%nonassoc GR_EQ

%%
input: %empty                             { $$ = nullptr; }
     | if_block input                     { root = new LineNode($1, $2); $$ = root; }
     | function_block input               { root = new LineNode($1, $2); $$ = root; }
     | line input                         { root = new LineNode($1, $2); $$ = root; }
;

if_block: IF term '\n' input ENDIF '\n'     { $$ = new IfBlockNode($2, $4); }
;

function_block: FUNCTION fname '(' params ')' '\n' input ENDFUNCTION '\n' { $$ = new FunctionBlockNode($2, $4, $7); }
function_block: BANG_FUNCTION fname '(' params ')' '\n' input ENDFUNCTION '\n' { $$ = new FunctionBlockNode($2, $4, $7); }
;

params: %empty                   { $$ = nullptr; }
    | ID                         { $$ = new ParamsNode($1, nullptr); }
    | ID ',' params              { $$ = new ParamsNode($1, $3); }

line: '\n'                        { $$ = nullptr; }
    | command '\n'                { $$ = $1; }
;

command: ID qargs                  { $$ = new CommandNode($1, $2); }
       | BANG_ID qargs             { $$ = new CommandNode($1, $2); }
       | let_command               { $$ = $1; }
;

let_command: LET var '=' term { $$ = new LetNode($2, $4); } 

qargs: %empty            { $$ = nullptr; }
     | term qargs        { $$ = new QargsNode($1, $2); }
;

term: val_term | arith_op | logical_op | cmp_op

arith_op: term '*' term            { $$ = new InfixOpNode($1, $3, "*"); }
        | term '/' term            { $$ = new InfixOpNode($1, $3, "/"); }
        | term '%' term            { $$ = new InfixOpNode($1, $3, "%"); }
        | term '.' term            { $$ = new InfixOpNode($1, $3, "."); }
        | term '+' term            { $$ = new InfixOpNode($1, $3, "+"); }
        | term '-' term            { $$ = new InfixOpNode($1, $3, "-"); }
;

logical_op: '!' term                 { $$ = new PrefixOpNode($2, "!"); }
          | term AND term           { $$ = new InfixOpNode($1, $3, "&&"); }
          | term OR term           { $$ = new InfixOpNode($1, $3, "||"); }
;

cmp_op: term '<' term             { $$ = new InfixOpNode($1, $3, "<"); }
      | term '>' term             { $$ = new InfixOpNode($1, $3, ">"); }
      | term LESS_EQ term            { $$ = new InfixOpNode($1, $3, ">"); }
      | term GR_EQ term            { $$ = new InfixOpNode($1, $3, ">="); }
      | term EQ term            { $$ = new InfixOpNode($1, $3, "=="); }
      | term NOT_EQ term            { $$ = new InfixOpNode($1, $3, "!="); }
;

val_term: fname '(' fargs ')'      { $$ = new FunCallNode($1, $3); }
        | '[' fargs ']'            { $$ = new ListNode($2); }
        | var                      { $$ = $1; }
        | STR                      { $$ = $1; }
        | ANGLE                    { $$ = $1; }
        | NUMBER                   { $$ = $1; }
;

var: ID              { $$ = $1; }
   | SCOPED_ID       { $$ = $1; }
;

fargs: %empty                { $$ = nullptr; }
     | term                  { $$ = new FargsNode($1, nullptr); }
     | term ',' fargs        { $$ = new FargsNode($1, $3); }
;

fname: ID                   { $$ = $1; }
     | AU_ID                { $$ = $1; }
     | SCOPED_ID            { $$ = $1; }
;
%%

#include "lex.c"

void yyerror (const char* s) {
    printf ("%s\n", s);
}

#include <cstring>

int main() {
    yydebug = 0;
    
    const int flex_debug = 0;
    if (!flex_debug) {
        yyparse();
        std::string s = root->getString();
        printf("%s", s.c_str());
    } else {
        int yychar = 0;
        do {
            yychar = yylex();
            if (yychar >= 32 && yychar < 126) {
                printf("Pass-through: %c\n", yychar);
            } else {
                int tr = YYTRANSLATE(yychar);
                printf("Lex=%s\n", yytname[tr]);
            }
        }
        while (yychar != 0);
    }


    return 0;
}
