%{
#define YYDEBUG 1
#define YYFPRINTF fprintf
#define YYPRINT

#define YYSTYPE_IS_TRIVIAL 1

#include <cstdio>
#include <string>
#include "Node.h"

#define YYSTYPE Node*
void yyerror(const char*);
int yylex();

Node* root = NULL;
%}

%token STR AU_ID SID_ID ID NUMBER
%token EQ NOT_EQ LESS_EQ GR_EQ MATCH NOT_MATCH CONCAT
%token ADD_EQ SUB_EQ MUL_EQ DIV_EQ MOD_EQ CON_EQ
%token AND OR
%token FUNCTION ENDFUNCTION IF ELSE ENDIF WHILE ENDWHILE FOR ENDFOR
%token LET
%token COMMAND COMMAND_ATTR COMMAND_REPLACE
%token QARGS
%token EX
%token VA_DOTS VA
%token ARROW

%%
input: %empty                             { $$ = nullptr; }
     | if_block input                     { root = new LineNode($1, $2); $$ = root; }
     | while_block input                  { root = new LineNode($1, $2); $$ = root; }
     | for_block input                    { root = new LineNode($1, $2); $$ = root; }
     | function_block input               { root = new LineNode($1, $2); $$ = root; }
     | line input                         { root = new LineNode($1, $2); $$ = root; }
;

if_block: IF expr1 '\n' input ENDIF '\n'                 { $$ = new IfBlockNode($2, $4); }
        | IF expr1 '\n' input ELSE '\n' input ENDIF '\n' { $$ = new IfBlockNode($2, $4, $7); }
;

while_block: WHILE expr1 '\n'input ENDWHILE '\n'         { $$ = new WhileBlockNode($2, $4); }
;

for_block: FOR for_var ID expr1 '\n' input ENDFOR '\n'       { $$ = new ForBlockNode($2, $4, $6); }
;

for_var: ID | '[' for_list ']'     { $$ = $2; }
;

for_list: ID | ID ',' for_list     { $$ = new ParamsNode($1, $3); }

function_block: FUNCTION fname '(' params ')' '\n' input ENDFUNCTION '\n' { $$ = new FunctionBlockNode($2, $4, $7); }
;

params: %empty                   { $$ = nullptr; }
    | VA_DOTS                    { $$ = new ParamsNode(new LexemNode("..."), nullptr); }
    | ID                         { $$ = new ParamsNode($1, nullptr); }
    | ID ',' params              { $$ = new ParamsNode($1, $3); }

line: '\n'                        { $$ = nullptr; }
    | command '\n'                { $$ = $1; }
;

command: let_command | cmd_command | ex_command
;

let_command: LET let_var '=' expr1 { $$ = new LetNode($2, $4, "="); }
           | LET let_var ADD_EQ expr1 { $$ = new LetNode($2, $4, "+="); }
           | LET let_var SUB_EQ expr1 { $$ = new LetNode($2, $4, "-="); }
           | LET let_var MUL_EQ expr1 { $$ = new LetNode($2, $4, "*="); }
           | LET let_var DIV_EQ expr1 { $$ = new LetNode($2, $4, "/="); }
           | LET let_var MOD_EQ expr1 { $$ = new LetNode($2, $4, "%="); }
           | LET let_var CON_EQ expr1 { $$ = new LetNode($2, $4, ".="); }
;

let_var: var_name
       | var_name '[' expr1 ']'              { $$ = new IndexNode($1, $3); }
       | var_name '[' expr1 ':' expr1 ']'    { $$ = new IndexNode($1, $3); }
;

var_name: ID | AU_ID
;

cmd_command: COMMAND cmd_attr_list ID QARGS            { $$ = new CommandNode($3, $2, $4); }
           | COMMAND cmd_attr_list ID                  { $$ = new CommandNode($3, $2, nullptr); }
;

cmd_attr_list: %empty                                  { $$ = nullptr; }
             | COMMAND_ATTR cmd_attr_list              { $$ = new AttrsNode($1, $2); }
;

ex_command: EX QARGS          { $$ = new ExNode($1, $2); }
          | EX                { $$ = new ExNode($1, nullptr); }
;

expr1: expr2
     | expr2 '?' expr1 ':' expr1       { $$ = new TernaryNode($1, $3, $5); }
;

expr2: expr3 | expr3 OR expr2               { $$ = new InfixOpNode($1, $3, "||"); }
;

expr3: expr4 | expr4 AND expr3              { $$ = new InfixOpNode($1, $3, "&&"); }
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

expr5: expr6 | expr6_sum | expr6_sub | expr6_dot | expr6_con
;

expr6_sum: expr6 '+' expr6        {  $$ = new InfixOpNode($1, $3, "+");  }
         | expr6 '+' expr6_sum    {  $$ = new InfixOpNode($1, $3, "+");  }
;
expr6_sub: expr6 '-' expr6        {  $$ = new InfixOpNode($1, $3, "-");  }
         | expr6 '-' expr6_sub    {  $$ = new InfixOpNode($1, $3, "-");  }
;
expr6_dot: expr6 '.' expr6        {  $$ = new InfixOpNode($1, $3, ".");  }
         | expr6 '.' expr6_dot    {  $$ = new InfixOpNode($1, $3, ".");  }
;
expr6_con: expr6 CONCAT expr6     {  $$ = new InfixOpNode($1, $3, "..");  }
         | expr6 CONCAT expr6_con {  $$ = new InfixOpNode($1, $3, "..");  }
;

expr6: expr7 | expr7_mul | expr7_div | expr7_mod
;

expr7_mul: expr7 '*' expr7        { $$ = new InfixOpNode($1, $3, "*"); }
         | expr7 '*' expr7_mul    { $$ = new InfixOpNode($1, $3, "*"); }
;
expr7_div: expr7 '/' expr7        { $$ = new InfixOpNode($1, $3, "/"); }
         | expr7 '/' expr7_mul    { $$ = new InfixOpNode($1, $3, "/"); }
;
expr7_mod: expr7 '%' expr7        { $$ = new InfixOpNode($1, $3, "%"); }
         | expr7 '%' expr7_mod    { $$ = new InfixOpNode($1, $3, "%"); }
;

expr7: expr8
     | '!' expr7            { $$ = new PrefixOpNode($2, "!"); }
     | '-' expr7            { $$ = new PrefixOpNode($2, "-"); }
     | '+' expr7            { $$ = new PrefixOpNode($2, "+"); }
;

expr8: expr9
     | expr8 '[' expr1 ']'               { $$ = new IndexNode($1, $3); }
     | expr8 '[' expr1 ':' expr1 ']'     { $$ = new IndexNode($1, $3, $5); }
     | expr8 '[' expr1 ':' ']'           { $$ = new IndexNode($1, $3, new LexemNode("end")); }
     | expr8 '[' ':' expr1 ']'           { $$ = new IndexNode($1, new LexemNode("begin"), $4); }
     | fname '(' fargs ')'               { $$ = new FunCallNode($1, $3); }
;

fname: ID | AU_ID | SID_ID
;

expr9: NUMBER
     | STR
     | '[' fargs ']'                         { $$ = new ListNode($2); }
     | '{' kv_pairs '}'                      { $$ = new DictNode($2); }
     | '(' expr1 ')'                         { $$ = $2; }
     | '{' fargs ARROW expr1 '}'             { $$ = new LambdaNode($2, $4); }
     | '&' ID                                { $$ = new OptionNode($2); }
     | ID
     | AU_ID
     | VA
     | COMMAND_REPLACE
;

fargs: %empty                 { $$ = nullptr; }
     | expr1                  { $$ = new FargsNode($1, nullptr); }
     | expr1 ',' fargs        { $$ = new FargsNode($1, $3); }
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
        if (root) {
            std::string s = root->getString();
            printf("%s", s.c_str());
        }
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
