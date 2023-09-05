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

%type <std::vector<std::string>> id_list id_list_or_empty
%type <std::vector<EvalNode*>> expr1_list expr1_list_or_empty
%type <std::vector<DictNode::Pair>> expr1_pairs expr1_pairs_or_empty

%type <EvalNode*> expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9

%token
    LET "let"
    UNLET "unlet"
    CONST "const"
    LOCKVAR "lockvar"
    UNLOCKVAR "unlockvar"
    FUNCTION "function"

%type <EvalCommand*> let unlet const lockvar unlockvar
%type <std::string> varname
%type <std::vector<std::string>> varname_list varname_multiple

%type <EvalCommand*> function

%type <EvalCommand*> input command

%code requires {
    #include "EvalNode.hpp"
    #include "EvalLocation.hpp"
    #include "EvalCommand.hpp"
    struct EvalState;
}

%code provides {
    int evallex(eval::parser::value_type* v, eval::parser::location_type* l, EvalState* state);
}

%language "c++"
%param { EvalState* state }
%parse-param { EvalFactory& factory }
%parse-param { EvalCommand*& result }

%define api.value.type variant
%define api.prefix {eval}
%define api.value.automove

%define parse.error detailed
%define parse.trace

%locations
%define api.location.type {EvalLocation}

%{
%}

%%
input: command { $$ = $1; result = $$; }

command: let
     | unlet
     | const
     | lockvar
     | unlockvar
     | function

// TODO remove .. from tokens pls! (and others)
// TODO additional checking!
let: LET varname '=' expr1                                { $$ = new LetVar($2, $4); }
   | LET varname '[' expr1 ']' '=' expr1                  { $$ = new LetElement($2, $4, $7); }
   | LET varname '[' ':' ']' '=' expr1                    { $$ = new LetRange($2, nullptr, nullptr, $7); }
   | LET varname '[' ':' expr1 ']' '=' expr1              { $$ = new LetRange($2, nullptr, $5, $8); }
   | LET varname '[' expr1 ':' expr1 ']' '=' expr1        { $$ = new LetRange($2, $4, $6, $9); }
   | LET varname '+' '=' expr1                            { $$ = new LetVar($2, $5, LetOp::PLUS); }
   | LET varname '-' '=' expr1                            { $$ = new LetVar($2, $5, LetOp::MINUS); }
   | LET varname '*' '=' expr1                            { $$ = new LetVar($2, $5, LetOp::MULT); }
   | LET varname '/' '=' expr1                            { $$ = new LetVar($2, $5, LetOp::DIV); }
   | LET varname '%' '=' expr1                            { $$ = new LetVar($2, $5, LetOp::MOD); }
   | LET varname '.' '=' expr1                            { $$ = new LetVar($2, $5, LetOp::DOT); }
   | LET varname '.' '.' '=' expr1                        { $$ = new LetVar($2, $6, LetOp::DOT2); }
   | LET '[' varname_list ']' '=' expr1                   { $$ = new LetUnpack($3, $6); }
   | LET '[' varname_list ']' '.' '=' expr1               { $$ = new LetUnpack($3, $7, LetOp::DOT); }
   | LET '[' varname_list ']' '+' '=' expr1               { $$ = new LetUnpack($3, $7, LetOp::PLUS); }
   | LET '[' varname_list ']' '-' '=' expr1               { $$ = new LetUnpack($3, $7, LetOp::MINUS); }
   | LET '[' varname_list ';' varname ']' '=' expr1       { $$ = new LetRemainder($3, $5, $8); }
   | LET '[' varname_list ';' varname ']' '+' '=' expr1   { $$ = new LetRemainder($3, $5, $9, LetOp::PLUS); }
   | LET '[' varname_list ';' varname ']' '-' '=' expr1   { $$ = new LetRemainder($3, $5, $9, LetOp::MINUS); }
   | LET '[' varname_list ';' varname ']' '.' '=' expr1   { $$ = new LetRemainder($3, $5, $9, LetOp::DOT); }
   | LET                                                  { $$ = new LetPrint(); }
   | LET varname_multiple                                 { $$ = new LetPrint($2); }
;

unlet: UNLET varname_list                                 { $$ = new Unlet($2); }
;

const: CONST varname '=' expr1                            { $$ = new ConstVar($2, $4); }
     | CONST '[' varname_list ']' '=' expr1               { $$ = new ConstUnpack($3, $6); }
     | CONST '[' varname_list ';' varname ']' '=' expr1   { $$ = new ConstRemainder($3, $5, $8); }
     | CONST                                              { $$ = new LetPrint(); }
     | CONST varname_multiple                             { $$ = new LetPrint($2); }
;

lockvar: LOCKVAR varname_multiple                         { $$ = new LockVar($2); }
       | LOCKVAR NUMBER varname_multiple                  { $$ = new LockVar($3, $2); }
;

unlockvar: UNLOCKVAR varname_multiple                         { $$ = new UnlockVar($2); }
         | UNLOCKVAR NUMBER varname_multiple                  { $$ = new UnlockVar($3, $2); }
;

varname: VA_ID | SID_ID | AUTOLOAD_ID | OPTION_ID | REGISTER_ID | ENV_ID | ID
;

varname_list: varname                        { $$ = {}; $$.push_back($1); }
            | varname ',' varname_list       { $$ = $3; $$.push_back($1); }
;

varname_multiple: varname                    { $$ = {}; $$.push_back($1); }
                | varname varname_multiple   { $$ = {}; $$.push_back($1); }

// TODO function matching pattern
// TODO function attributes
function: FUNCTION                                            { $$ = new FunctionPrint(); }
        | FUNCTION varname '(' varname_list ')'               { $$ = new Function($2, $4); }
        | FUNCTION varname '.' varname '(' varname_list ')'   { $$ = new FunctionDict($2, $4, $6); }

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
