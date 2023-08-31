%token STR AU_ID SID_ID ID OPT_ID REG_ID ENV_ID NUMBER FLOAT BLOB
%token SPC_SQB IDX_SQB EQ NOT_EQ LESS_EQ GR_EQ MATCH NOT_MATCH CONCAT
%token ADD_EQ SUB_EQ MUL_EQ DIV_EQ MOD_EQ CON_EQ
%token AND OR
%token FUNCTION ENDFUNCTION IF ELSE ELSEIF ENDIF WHILE ENDWHILE FOR ENDFOR
%token LET
%token COMMAND COMMAND_ATTR
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
     | expr8 IDX_SQB expr1 ']'               { $$ = new IndexNode($1, $3); }
     | expr8 IDX_SQB expr1 ':' expr1 ']'     { $$ = new IndexNode($1, $3, $5); }
     | expr8 IDX_SQB expr1 ':' ']'           { $$ = new IndexNode($1, $3, new LexemNode("end")); }
     | expr8 IDX_SQB ':' expr1 ']'           { $$ = new IndexNode($1, new LexemNode("begin"), $4); }
     | fname '(' expr1_list ')'              { $$ = new FunCallNode($1, $3); }
;

expr9: NUMBER
     | FLOAT
     | BLOB
     | STR
     | SPC_SQB expr1_list ']'                   { $$ = new ListNode($2); }
     | '{' expr1_pairs '}'                      { $$ = new DictNode($2); }
     | '(' expr1 ')'                            { $$ = $2; }
     | '{' args ARROW expr1 '}'                 { $$ = new LambdaNode($2, $4); }
     | OPT_ID                                   { $$ = $1; }
     | REG_ID                                   { $$ = $1; }
     | ENV_ID                                   { $$ = $1; }
     | ID
     | AU_ID
     | VA
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
