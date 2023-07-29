%token STR AU_ID SID_ID ID NUMBER
%token EQ NOT_EQ LESS_EQ GR_EQ MATCH NOT_MATCH CONCAT
%token ADD_EQ SUB_EQ MUL_EQ DIV_EQ MOD_EQ CON_EQ
%token AND OR
%token FUNCTION ENDFUNCTION IF ELSE ELSEIF ENDIF WHILE ENDWHILE FOR ENDFOR
%token LET
%token COMMAND COMMAND_ATTR COMMAND_REPLACE
%token QARGS
%token EX
%token VA_DOTS VA
%token ARROW

%code requires {
    struct Node;
}

%define parse.error custom
%define api.value.type {Node*}
//%define api.location.type {Location}

%{
#include <cstdio>
#include <string>
#include "Node.h"

#define YYDEBUG 1
#define YYFPRINTF fprintf
#define YYPRINT

//#define YYSTYPE_IS_TRIVIAL 1

void yyerror(const char*);
int yylex();

Node* root = NULL;

%}

%%
input: %empty                             { $$ = nullptr; }
     | if_block input                     { root = new LineNode($1, $2); $$ = root; }
     | while_block input                  { root = new LineNode($1, $2); $$ = root; }
     | for_block input                    { root = new LineNode($1, $2); $$ = root; }
     | function_block input               { root = new LineNode($1, $2); $$ = root; }
     | line input                         { root = new LineNode($1, $2); $$ = root; }
;

if_block: IF expr1 '\n' input ENDIF '\n'                   { $$ = new IfBlockNode($2, $4); }
        | IF expr1 '\n' input ELSE '\n' input ENDIF '\n'   { $$ = new IfBlockNode($2, $4, $7); }
        | IF expr1 '\n' input elsei_list '\n'              { $$ = new IfBlockNode($2, $4, $5); }
;

elsei_list: ELSEIF expr1 '\n' input ENDIF                      { $$ = new IfBlockNode($2, $4); }
          | ELSEIF expr1 '\n' input ELSE '\n' input ENDIF      { $$ = new IfBlockNode($2, $4, $7); }
          | ELSEIF expr1 '\n' input elsei_list                 { $$ = new IfBlockNode($2, $4, $5); }

while_block: WHILE expr1 '\n'input ENDWHILE '\n'         { $$ = new WhileBlockNode($2, $4); }
;

for_block: FOR var ID expr1 '\n' input ENDFOR '\n'          { $$ = new ForBlockNode($2, $4, $6); }
         | FOR unpack ID expr1 '\n' input ENDFOR '\n'       { $$ = new ForBlockNode($2, $4, $6); }
;

unpack: '[' var_list ']'              { $$ = $2; }
      | '[' var_list ';' var ']'      { $$ = new ParamsNode($2, $4); }

var_list: var                       { $$ = new ParamsNode($1, nullptr); }
        | var ',' var_list          { $$ = new ParamsNode($1, $3); }
;

var: ID | AU_ID | VA
;

function_block: FUNCTION fname '(' args ')' '\n' input ENDFUNCTION '\n' { $$ = new FunctionBlockNode($2, $4, $7); }
;

args: %empty                  { $$ = nullptr; }
    | VA_DOTS                 { $$ = new ParamsNode(new LexemNode("..."), nullptr); }
    | var                     { $$ = new ParamsNode($1, nullptr); }
    | var ',' args            { $$ = new ParamsNode($1, $3); }

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

let_var: var
       | var '[' expr1 ']'              { $$ = new IndexNode($1, $3); }
       | var '[' expr1 ':' expr1 ']'    { $$ = new IndexNode($1, $3); }
       | unpack                         { $$ = $1; }
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
     | fname '(' expr1_list ')'          { $$ = new FunCallNode($1, $3); }
;

fname: ID | AU_ID | SID_ID
;

expr9: NUMBER
     | STR
     | '[' expr1_list ']'                         { $$ = new ListNode($2); }
     | '{' expr1_pairs '}'                      { $$ = new DictNode($2); }
     | '(' expr1 ')'                         { $$ = $2; }
     | '{' args ARROW expr1 '}'             { $$ = new LambdaNode($2, $4); }
     | '&' ID                                { $$ = new OptionNode($2); }
     | ID
     | AU_ID
     | VA
     | COMMAND_REPLACE
;

expr1_list: %empty                 { $$ = nullptr; }
          | expr1                  { $$ = new FargsNode($1, nullptr); }
          | expr1 ',' expr1_list   { $$ = new FargsNode($1, $3); }
;

expr1_pairs: %empty                           { $$ = nullptr; }
           | expr1 ':' expr1                  { $$ = new AttrsNode(new KeyValueNode($1, $3), nullptr); }
           | expr1 ':' expr1 ',' expr1_pairs  { $$ = new AttrsNode(new KeyValueNode($1, $3), $5); }
;
%%

void yyerror (const char* s) {
    fprintf(stderr, "Fatal error: %s\n", s);
    exit(5);
}

static int yyreport_syntax_error(const yypcontext_t *ctx) {
    extern int yylineno;
    fprintf(stderr, "On line %d: syntax error. ", yylineno);
    // Report the tokens expected at this point.
    int res = 0;
    const int TOKENMAX = 10;
    yysymbol_kind_t expected[TOKENMAX];
    int n = yypcontext_expected_tokens(ctx, expected, TOKENMAX);
    if (n < 0) {
        // Forward errors to yyparse.
        res = n;
    } else {
        for (int i = 0; i < n; ++i) {
            const char* heading = (i == 0 ? "Expected" : "\nOr");
            fprintf(stderr, "%s %s", heading, yysymbol_name(expected[i]));
        }
    }

    // Report the unexpected token.
    yysymbol_kind_t lookahead = yypcontext_token(ctx);
    if (lookahead != YYSYMBOL_YYEMPTY) {
        fprintf(stderr, "\nBefore %s", yysymbol_name(lookahead));
    }
    fprintf(stderr, "\n");
    return res;
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
                yysymbol_kind_t tr = YYTRANSLATE(yychar);
                printf("Lex=%s\n", yysymbol_name(tr));
            }
        }
        while (yychar != 0);
    }


    return 0;
}
