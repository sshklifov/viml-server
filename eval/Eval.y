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

%token IS "is"
    ISNOT "isnot"

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
%type <std::vector<std::string>> id_list id_list_or_empty
%type <std::vector<EvalNode*>> expr1_list expr1_list_or_empty
%type <std::vector<DictNode::Pair>> expr1_pairs expr1_pairs_or_empty

%type <EvalNode*> expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9

%token
    LET "let"
    UNLET "unlet"
    CONST "const"

%type <void*> let unlet const
%type <std::string> varname
%type <std::vector<std::string>> varname_list

%code requires {
    #include "EvalNode.hpp"
    #include "Location.hpp"
}

%code provides {
    int evallex(eval::parser::value_type* v, eval::parser::location_type* l, const EvalFactory& factory);
}

%language "c++"
%param { EvalFactory& factory }

%define api.value.type variant
%define api.prefix {eval}
%define api.value.automove

%define parse.error detailed
%define parse.trace

%locations
%define api.location.type {Location}

%{
%}

%%
input: let  { $$ = nullptr; }
     | unlet { $$ = nullptr; }
     | const { $$ = nullptr; }
     | expr1 { $$ = nullptr; }

// TODO remove .. from tokens pls! (and others)
// TODO additional checking!
let: LET varname '=' expr1                                { $$ = nullptr; }
   | LET varname '[' ':' ']' '=' expr1                    { $$ = nullptr; }
   | LET varname '[' expr1 ']' '=' expr1                  { $$ = nullptr; }
   | LET varname '[' ':' expr1 ']' '=' expr1              { $$ = nullptr; }
   | LET varname '[' expr1 ':' expr1 ']' '=' expr1        { $$ = nullptr; }
   | LET varname '+' '=' expr1                            { $$ = nullptr; }
   | LET varname '-' '=' expr1                            { $$ = nullptr; }
   | LET varname '*' '=' expr1                            { $$ = nullptr; }
   | LET varname '/' '=' expr1                            { $$ = nullptr; }
   | LET varname '%' '=' expr1                            { $$ = nullptr; }
   | LET varname '.' '=' expr1                            { $$ = nullptr; }
   | LET varname '.' '.' '=' expr1                        { $$ = nullptr; }
   | LET '[' varname_list ']' '=' expr1                   { $$ = nullptr; }
   | LET '[' varname_list ']' '.' '=' expr1               { $$ = nullptr; }
   | LET '[' varname_list ']' '+' '=' expr1               { $$ = nullptr; }
   | LET '[' varname_list ']' '-' '=' expr1               { $$ = nullptr; }
   | LET '[' varname_list ';' varname ']' '=' expr1       { $$ = nullptr; }
   | LET '[' varname_list ';' varname ']' '.' '=' expr1   { $$ = nullptr; }
   | LET '[' varname_list ';' varname ']' '+' '=' expr1   { $$ = nullptr; }
   | LET '[' varname_list ';' varname ']' '-' '=' expr1   { $$ = nullptr; }
   | LET                                                  { $$ = nullptr; }
   | LET varname_list                                     { $$ = nullptr; }
;

unlet: UNLET varname_list                                 { $$ = nullptr; }
;

const: CONST varname '=' expr1                            { $$ = nullptr; }
     | CONST '[' varname_list ']' '=' expr1               { $$ = nullptr; }
     | CONST '[' varname_list ';' varname ']' '=' expr1   { $$ = nullptr; }
     | CONST                                              { $$ = nullptr; }
     | CONST varname                                      { $$ = nullptr; }

varname: VA_ID | SID_ID | AUTOLOAD_ID | OPTION_ID | REGISTER_ID | ENV_ID | ID
;

varname_list: varname                     { $$ = {}; $$.push_back($1); }
            | varname ',' varname_list       { $$ = $3; $$.push_back($1); }
;

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


     | expr5 IS expr5                         { $$ = factory.create<InfixOpNode>($1, $3, "is"); }
     | expr5 ISNOT expr5                      { $$ = factory.create<InfixOpNode>($1, $3, "isnot"); }
;

expr5: expr6
     | expr6 '+' expr5        {  $$ = factory.create<InfixOpNode>($1, $3, "+");  }
     | expr6 '-' expr5        {  $$ = factory.create<InfixOpNode>($1, $3, "-");  }
     | expr6 '.' expr5        {  $$ = factory.create<InfixOpNode>($1, $3, ".");  }
     | expr6 CONCAT expr5     {  $$ = factory.create<InfixOpNode>($1, $3, "..");  }
;

expr6: expr7
     | expr7 '*' expr6        { $$ = factory.create<InfixOpNode>($1, $3, "*"); }
     | expr7 '/' expr6        { $$ = factory.create<InfixOpNode>($1, $3, "/"); }
     | expr7 '%' expr6        { $$ = factory.create<InfixOpNode>($1, $3, "%"); }
;

expr7: expr8
     | '!' expr7            { $$ = factory.create<PrefixOpNode>($2, "!"); }
     | '-' expr7            { $$ = factory.create<PrefixOpNode>($2, "-"); }
     | '+' expr7            { $$ = factory.create<PrefixOpNode>($2, "+"); }
;

// TODO subscript
// TODO method
expr8: expr9
     | expr8 '[' expr1 ']'               { $$ = factory.create<IndexNode>($1, $3); }
     | expr8 '[' expr1 ':' expr1 ']'     { $$ = factory.create<IndexRangeNode>($1, $3, $5); }
     | expr8 '[' expr1 ':' ']'           { $$ = factory.create<IndexRangeNode>($1, $3, nullptr); }
     | expr8 '[' ':' expr1 ']'           { $$ = factory.create<IndexRangeNode>($1, nullptr, $4); }
     | expr8 '[' ':' ']'                 { $$ = factory.create<IndexRangeNode>($1, nullptr, nullptr); }
     | expr8 '(' expr1_list ')'          { $$ = factory.create<InvokeNode>($1, $3); }
;

// TODO Dictionary #{}
expr9: NUMBER                               { $$ = factory.create<TokenNode>($1, TokenNode::NUMBER); }
     | FLOAT                                { $$ = factory.create<TokenNode>($1, TokenNode::FLOAT); }
     | BLOB                                 { $$ = factory.create<TokenNode>($1, TokenNode::BLOB); }
     | STR                                  { $$ = factory.create<TokenNode>($1, TokenNode::STRING); }
     | '[' expr1_list_or_empty ']'          { $$ = factory.create<ListNode>($2); }
     | '{' expr1_pairs_or_empty '}'         { $$ = factory.create<DictNode>($2); }
     | '(' expr1 ')'                        { $$ = factory.create<NestedNode>($2); }
     | '{' id_list_or_empty ARROW expr1 '}' { $$ = factory.create<LambdaNode>($2, $4); }
     | OPTION_ID                            { $$ = factory.create<TokenNode>($1, TokenNode::OPTION); }
     | REGISTER_ID                          { $$ = factory.create<TokenNode>($1, TokenNode::REGISTER); }
     | ENV_ID                               { $$ = factory.create<TokenNode>($1, TokenNode::ENV); }
     | ID                                   { $$ = factory.create<TokenNode>($1, TokenNode::ID); }
     | AUTOLOAD_ID                          { $$ = factory.create<TokenNode>($1, TokenNode::AUTOLOAD); }
     | VA_ID                                { $$ = factory.create<TokenNode>($1, TokenNode::VA); }
;

id_list_or_empty: %empty           { $$ = {}; }
                | id_list          { $$ = $1; }

id_list: ID                        { $$ = {}; $$.push_back($1); }
       | ID ',' id_list            { $$ = $3; $$.push_back($1); }
;

expr1_list_or_empty: %empty        { $$ = {}; }
                   | expr1_list    { $$ = $1; }
;

expr1_list: expr1                  { $$ = {}; $$.push_back($1); }
          | expr1 ',' expr1_list   { $$ = $3; $$.push_back($1); }
;

expr1_pairs_or_empty: %empty       { $$ = {}; }
                    | expr1_pairs  { $$ = $1; }

expr1_pairs: expr1 ':' expr1                      { $$ = {}; $$.push_back(DictNode::Pair($1, $3)); }
           | expr1 ':' expr1 ',' expr1_pairs      { $$ = $5; $$.push_back(DictNode::Pair($1, $3)); }
;
