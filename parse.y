%{
#define YYDEBUG 1
#define YYFPRINTF fprintf
#define YYPRINT

#define YYFREE

#include <cstdio>
#include <string>
#include "Node.h"

#define YYSTYPE Node*
void yyerror(const char*);
int yylex();

Node* root = NULL;
%}

%token STR BANG_ID AU_ID SCOPED_ID ID NUMBER
%token EQ NOT_EQ LESS_EQ GR_EQ MATCH NOT_MATCH CONCAT
%token AND OR
%token BANG_FUNCTION FUNCTION ENDFUNCTION IF ENDIF
%token LET
%token BANG_COMMAND COMMAND COMMAND_ATTR COMMAND_REPLACE
%token SET_COMMAND

//%left '%' '/' '*' '.' ' 

//%left '*'
//%left '/'
//%left '%'
//%left '.'
//%left '+'
//%left '-'

//%left AND
//%left OR

//%nonassoc '!'
//%nonassoc '<'
//%nonassoc '>'
//%nonassoc EQ
//%nonassoc NOT_EQ
//%nonassoc LESS_EQ
//%nonassoc GR_EQ

%%
input: %empty                             { $$ = nullptr; }
     | if_block input                     { root = new LineNode($1, $2); $$ = root; }
     | function_block input               { root = new LineNode($1, $2); $$ = root; }
     | line input                         { root = new LineNode($1, $2); $$ = root; }
;

if_block: IF expr1 '\n' input ENDIF '\n'     { $$ = new IfBlockNode($2, $4); }
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

command: ex_command | let_command | cmd_command | SET_COMMAND
;

ex_command: ID qargs       { $$ = new ExNode($1, $2); }
          | BANG_ID qargs  { $$ = new ExNode($1, $2); }
;

qargs: %empty             { $$ = nullptr; }
     | expr1 qargs        { $$ = new QargsNode($1, $2); }
;

let_command: LET let_var '=' expr1 { $$ = new LetNode($2, $4); } 
;

let_var: var_name
       | var_name '[' expr1 ']'              { $$ = new IndexNode($1, $3); }
       | var_name '[' expr1 ':' expr1 ']'    { $$ = new IndexNode($1, $3); }
;

var_name: ID | SCOPED_ID
;

cmd_command: COMMAND cmd_attr_list ID ex_command            { $$ = new CommandNode($3, $2, $4); }
           | BANG_COMMAND cmd_attr_list ID ex_command       { $$ = new CommandNode($3, $2, $4); }
;

cmd_attr_list: %empty                                  { $$ = nullptr; }
             | COMMAND_ATTR cmd_attr_list              { $$ = new AttrsNode($1, $2); }
;

expr1: expr2
     | expr2 '?' expr1 ':' expr1       { $$ = new TernaryNode($1, $3, $5); }
;

expr2: expr3
     | expr3 OR expr3               { $$ = new InfixOpNode($1, $3, "||"); }
;

expr3: expr4
     | expr4 AND expr4              { $$ = new InfixOpNode($1, $3, "&&"); }
;

expr4: expr5
     | expr5 EQ expr5                 { $$ = new InfixOpNode($1, $3, "=="); }
     | expr5 NOT_EQ expr5             { $$ = new InfixOpNode($1, $3, "!="); }
     | expr5 '>' expr5                  { $$ = new InfixOpNode($1, $3, ">"); }
     | expr5 GR_EQ expr5                { $$ = new InfixOpNode($1, $3, ">="); }
     | expr5 '<' expr5                  { $$ = new InfixOpNode($1, $3, "<"); }
     | expr5 LESS_EQ expr5              { $$ = new InfixOpNode($1, $3, ">"); }
     | expr5 MATCH expr5                { $$ = new InfixOpNode($1, $3, "=~"); }
     | expr5 NOT_MATCH expr5            { $$ = new InfixOpNode($1, $3, "!~"); }

     | expr5 EQ '#' expr5                   { $$ = new InfixOpNode($1, $4, "==#"); }
     | expr5 NOT_EQ '#' expr5               { $$ = new InfixOpNode($1, $4, "!=#"); }
     | expr5 '>' '#' expr5                  { $$ = new InfixOpNode($1, $4, ">#"); }
     | expr5 GR_EQ '#' expr5                { $$ = new InfixOpNode($1, $4, ">=#"); }
     | expr5 '<' '#' expr5                  { $$ = new InfixOpNode($1, $4, "<#"); }
     | expr5 LESS_EQ '#' expr5              { $$ = new InfixOpNode($1, $4, ">#"); }
     | expr5 MATCH '#' expr5                { $$ = new InfixOpNode($1, $4, "=~#"); }
     | expr5 NOT_MATCH '#' expr5            { $$ = new InfixOpNode($1, $4, "!~#"); }

     | expr5 EQ '?' expr5                   { $$ = new InfixOpNode($1, $4, "==?"); }
     | expr5 NOT_EQ '?' expr5               { $$ = new InfixOpNode($1, $4, "!=?"); }
     | expr5 '>' '?' expr5                  { $$ = new InfixOpNode($1, $4, ">?"); }
     | expr5 GR_EQ '?' expr5                { $$ = new InfixOpNode($1, $4, ">=?"); }
     | expr5 '<' '?' expr5                  { $$ = new InfixOpNode($1, $4, "<?"); }
     | expr5 LESS_EQ '?' expr5              { $$ = new InfixOpNode($1, $4, ">?"); }
     | expr5 MATCH '?' expr5                { $$ = new InfixOpNode($1, $4, "=~?"); }
     | expr5 NOT_MATCH '?' expr5            { $$ = new InfixOpNode($1, $4, "!~?"); }
;

expr5: expr6
     | expr6 '+' expr6            { $$ = new InfixOpNode($1, $3, "+"); }
     | expr6 '-' expr6            { $$ = new InfixOpNode($1, $3, "-"); }
     | expr6 '.' expr6            { $$ = new InfixOpNode($1, $3, "."); }
     | expr6 CONCAT expr6         { $$ = new InfixOpNode($1, $3, ".."); }
;

expr6: expr7
     | expr7 '*' expr6            { $$ = new InfixOpNode($1, $3, "*"); }
     | expr7 '/' expr6            { $$ = new InfixOpNode($1, $3, "/"); }
     | expr7 '%' expr6            { $$ = new InfixOpNode($1, $3, "%"); }
;

expr7: expr8
     | '!' expr8            { $$ = new PrefixOpNode($2, "!"); }
     | '-' expr8            { $$ = new PrefixOpNode($2, "-"); }
     | '+' expr8            { $$ = new PrefixOpNode($2, "+"); }
;

expr8: expr9
     | expr9 '[' expr1 ']'               { $$ = new IndexNode($1, $3); }
     | expr9 '[' expr1 ':' expr1 ']'     { $$ = new IndexNode($1, $3); }
     | expr9 '.' ID                      { $$ = new IndexNode($1, $3); }
     | fname '(' fargs ')'               { $$ = new FunCallNode($1, $3); }
;

fname: ID | SCOPED_ID | AU_ID
;

expr9: NUMBER
     | STR
     | '[' fargs ']'                         { $$ = new ListNode($2); }
     | '{' kv_pairs '}'                      { $$ = new DictNode($2); }
     | '#' '{' kv_pairs '}'                  { $$ = new DictNode($3); }
     | '(' expr1 ')'                         { $$ = $2; }
     | ID
     | SCOPED_ID
     | AU_ID
     | COMMAND_REPLACE
;

fargs: %empty                 { $$ = nullptr; }
     | expr1                  { $$ = new FargsNode($1, nullptr); }
     | expr1 ',' expr1        { $$ = new FargsNode($1, $3); }
;

kv_pairs: %empty           { $$ = nullptr; }
        | kv               { $$ = new AttrsNode($1, nullptr); }
        | kv ',' kv_pairs  { $$ = new AttrsNode($1, $3); }
;

kv: key ':' expr1          { $$ = new KeyValueNode($1, $3); }
;

key: ID | STR | NUMBER
;
%%

void yyerror (const char* s) {
    printf ("%s\n", s);
}

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
