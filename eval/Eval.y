%token AND "&&"
    OR "||"

%token EQ "=="
    NOT_EQ "!="
    GR ">"
    GR_EQ ">="
    LESS "<"
    LESS_EQ "<="
    MATCH "=~"
    NOT_MATCH "!~"

%token EQ_CASE "==#"
    NOT_EQ_CASE "!=#"
    GR_CASE ">#"
    GR_EQ_CASE ">=#"
    LESS_CASE "<#"
    LESS_EQ_CASE "<=#"
    MATCH_CASE "=~#"
    NOT_MATCH_CASE "!~#"

%token EQ_ICASE "==?"
    NOT_EQ_ICASE "!=?"
    GR_ICASE ">?"
    GR_EQ_ICASE ">=?"
    LESS_ICASE "<?"
    LESS_EQ_ICASE "<=?"
    MATCH_ICASE "=~?"
    NOT_MATCH_ICASE "!~?"

%token CONCAT ".."

%token <std::string> NUMBER FLOAT BLOB STR
%token <int> ARROW
%token <std::string> VA_ID SID_ID AUTOLOAD_ID OPTION_ID REGISTER_ID ENV_ID ID

%type <EvalNode*> input
%type <std::string> fname
%type <std::vector<std::string>> id_list
%type <std::vector<EvalNode*>> expr1_list
%type <std::unordered_map<std::string,EvalNode*>> expr1_pairs
%type <std::string> expr1_key

%type <EvalNode*> expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9
%type <EvalNode*> expr6_sum expr6_sub expr6_dot expr6_con
%type <EvalNode*> expr7_mul expr7_div expr7_mod

%code requires {
    #include "EvalNode.hpp"
}

%code provides {
    int evallex(eval::parser::value_type* p, const EvalFactory& factory);
}

%language "c++"
%param { EvalFactory& factory }

%define api.value.type variant
// TODO %define api.pure full
%define parse.error detailed
%define api.prefix {eval}

%{
%}

%%
input: expr1                                { factory.setTopLevel($1); $$ = $1; }

expr1: expr2
     | expr2 '?' expr1 ':' expr1            { $$ = factory.create<TernaryNode>($1, $3, $5); }
;

expr2: expr3 | expr3 OR expr2               { $$ = factory.create<InfixOpNode>($1, $3, "||"); }
;

expr3: expr4 | expr4 AND expr3              { $$ = factory.create<InfixOpNode>($1, $3, "&&"); }
;

expr4: expr5
     | expr5 EQ expr5                   { $$ = factory.create<InfixOpNode>($1, $3, "=="); }
     | expr5 NOT_EQ expr5               { $$ = factory.create<InfixOpNode>($1, $3, "!="); }
     | expr5 GR expr5                   { $$ = factory.create<InfixOpNode>($1, $3, ">"); }
     | expr5 GR_EQ expr5                { $$ = factory.create<InfixOpNode>($1, $3, ">="); }
     | expr5 LESS expr5                 { $$ = factory.create<InfixOpNode>($1, $3, "<"); }
     | expr5 LESS_EQ expr5              { $$ = factory.create<InfixOpNode>($1, $3, ">"); }
     | expr5 MATCH expr5                { $$ = factory.create<InfixOpNode>($1, $3, "=~"); }
     | expr5 NOT_MATCH expr5            { $$ = factory.create<InfixOpNode>($1, $3, "!~"); }

     | expr5 EQ_CASE expr5                   { $$ = factory.create<InfixOpNode>($1, $3, "==#"); }
     | expr5 NOT_EQ_CASE expr5               { $$ = factory.create<InfixOpNode>($1, $3, "!=#"); }
     | expr5 GR_CASE expr5                   { $$ = factory.create<InfixOpNode>($1, $3, ">#"); }
     | expr5 GR_EQ_CASE expr5                { $$ = factory.create<InfixOpNode>($1, $3, ">=#"); }
     | expr5 LESS_CASE expr5                 { $$ = factory.create<InfixOpNode>($1, $3, "<#"); }
     | expr5 LESS_EQ_CASE expr5              { $$ = factory.create<InfixOpNode>($1, $3, ">#"); }
     | expr5 MATCH_CASE expr5                { $$ = factory.create<InfixOpNode>($1, $3, "=~#"); }
     | expr5 NOT_MATCH_CASE expr5            { $$ = factory.create<InfixOpNode>($1, $3, "!~#"); }

     | expr5 EQ_ICASE expr5                   { $$ = factory.create<InfixOpNode>($1, $3, "==?"); }
     | expr5 NOT_EQ_ICASE expr5               { $$ = factory.create<InfixOpNode>($1, $3, "!=?"); }
     | expr5 GR_ICASE expr5                   { $$ = factory.create<InfixOpNode>($1, $3, ">?"); }
     | expr5 GR_EQ_ICASE expr5                { $$ = factory.create<InfixOpNode>($1, $3, ">=?"); }
     | expr5 LESS_ICASE expr5                 { $$ = factory.create<InfixOpNode>($1, $3, "<?"); }
     | expr5 LESS_EQ_ICASE expr5              { $$ = factory.create<InfixOpNode>($1, $3, ">?"); }
     | expr5 MATCH_ICASE expr5                { $$ = factory.create<InfixOpNode>($1, $3, "=~?"); }
     | expr5 NOT_MATCH_ICASE expr5            { $$ = factory.create<InfixOpNode>($1, $3, "!~?"); }
;

expr5: expr6 | expr6_sum | expr6_sub | expr6_dot | expr6_con
;

expr6_sum: expr6 '+' expr6        {  $$ = factory.create<InfixOpNode>($1, $3, "+");  }
         | expr6 '+' expr6_sum    {  $$ = factory.create<InfixOpNode>($1, $3, "+");  }
;
expr6_sub: expr6 '-' expr6        {  $$ = factory.create<InfixOpNode>($1, $3, "-");  }
         | expr6 '-' expr6_sub    {  $$ = factory.create<InfixOpNode>($1, $3, "-");  }
;
expr6_dot: expr6 '.' expr6        {  $$ = factory.create<InfixOpNode>($1, $3, ".");  }
         | expr6 '.' expr6_dot    {  $$ = factory.create<InfixOpNode>($1, $3, ".");  }
;
expr6_con: expr6 CONCAT expr6     {  $$ = factory.create<InfixOpNode>($1, $3, "..");  }
         | expr6 CONCAT expr6_con {  $$ = factory.create<InfixOpNode>($1, $3, "..");  }
;

expr6: expr7 | expr7_mul | expr7_div | expr7_mod
;

expr7_mul: expr7 '*' expr7        { $$ = factory.create<InfixOpNode>($1, $3, "*"); }
         | expr7 '*' expr7_mul    { $$ = factory.create<InfixOpNode>($1, $3, "*"); }
;
expr7_div: expr7 '/' expr7        { $$ = factory.create<InfixOpNode>($1, $3, "/"); }
         | expr7 '/' expr7_mul    { $$ = factory.create<InfixOpNode>($1, $3, "/"); }
;
expr7_mod: expr7 '%' expr7        { $$ = factory.create<InfixOpNode>($1, $3, "%"); }
         | expr7 '%' expr7_mod    { $$ = factory.create<InfixOpNode>($1, $3, "%"); }
;

expr7: expr8
     | '!' expr7            { $$ = factory.create<PrefixOpNode>($2, "!"); }
     | '-' expr7            { $$ = factory.create<PrefixOpNode>($2, "-"); }
     | '+' expr7            { $$ = factory.create<PrefixOpNode>($2, "+"); }
;

expr8: expr9
     | expr8 '[' expr1 ']'               { $$ = factory.create<IndexNode>(IndexNode::Single(), $1, $3); }
     | expr8 '[' expr1 ':' expr1 ']'     { $$ = factory.create<IndexNode>(IndexNode::Double(), $1, $3, $5); }
     | expr8 '[' expr1 ':' ']'           { $$ = factory.create<IndexNode>(IndexNode::Left(), $1, $3); }
     | expr8 '[' ':' expr1 ']'           { $$ = factory.create<IndexNode>(IndexNode::Right(), $1, $4); }
     | expr8 '[' ':' ']'                 { $$ = factory.create<IndexNode>(IndexNode::None(), $1); }
     | fname '(' expr1_list ')'          { $$ = factory.create<InvokeNode>(std::move($1), std::move($3)); }
;

fname: SID_ID | AUTOLOAD_ID | ID
;

expr9: NUMBER                               { $$ = factory.create<TokenNode>(std::move($1), TokenNode::NUMBER); }
     | FLOAT                                { $$ = factory.create<TokenNode>(std::move($1), TokenNode::FLOAT); }
     | BLOB                                 { $$ = factory.create<TokenNode>(std::move($1), TokenNode::BLOB); }
     | STR                                  { $$ = factory.create<TokenNode>(std::move($1), TokenNode::STRING); }
     | '[' expr1_list ']'                   { $$ = factory.create<ListNode>(std::move($2)); }
     | '{' expr1_pairs '}'                  { $$ = factory.create<DictNode>(std::move($2)); }
     | '(' expr1 ')'                        { $$ = $2; }
     | '{' id_list ARROW expr1 '}'          { $$ = factory.create<LambdaNode>(std::move($2), $4); }
     | OPTION_ID                            { $$ = factory.create<TokenNode>(std::move($1), TokenNode::OPTION); }
     | REGISTER_ID                          { $$ = factory.create<TokenNode>(std::move($1), TokenNode::REGISTER); }
     | ENV_ID                               { $$ = factory.create<TokenNode>(std::move($1), TokenNode::ENV); }
     | ID                                   { $$ = factory.create<TokenNode>(std::move($1), TokenNode::ID); }
     | AUTOLOAD_ID                          { $$ = factory.create<TokenNode>(std::move($1), TokenNode::AUTOLOAD); }
     | VA_ID                                { $$ = factory.create<TokenNode>(std::move($1), TokenNode::VA); }
;

id_list: %empty                    { $$ = {}; }
    | ID                           { $$ = {}; $$.push_back($1); }
    | ID ',' id_list               { $$ = $3; $$.push_back($1); }
;

expr1_list: %empty                 { $$ = {}; }
          | expr1                  { $$ = {}; $$.push_back($1); }
          | expr1_list ',' expr1   { $$ = $1; $$.push_back($3); }
;

expr1_pairs: %empty                               { $$ = {}; }
           | expr1_key ':' expr1                  { $$ = {}; $$[$1] = $3; }
           | expr1_pairs ',' expr1_key ':' expr1  { $$ = $1; $$[$3] = $5; }
;

expr1_key: NUMBER | ID
