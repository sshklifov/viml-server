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
    IS_CASE "is#"
    IS_ICASE "is?"
    ISNOT "isnot"
    ISNOT_CASE "isnot#"
    ISNOT_ICASE "isnot?"

%token EQ_CASE "==#"
    NOT_EQ_CASE "!=#"
    GR_CASE ">#"
    GR_EQ_CASE ">=#"
    LESS_CASE "<#"
    LESS_EQ_CASE "<=#"
    MATCH_CASE "=~#"
    NOT_MATCH_CASE "!~#"

%token PLUS_EQ "+="
    MINUS_EQ "-="
    MULT_EQ "*="
    DIV_EQ "/="
    MOD_EQ "%="
    DOT_EQ ".="
    CONCAT_EQ "..="

%token EQ_ICASE "==?"
    NOT_EQ_ICASE "!=?"
    GR_ICASE ">?"
    GR_EQ_ICASE ">=?"
    LESS_ICASE "<?"
    LESS_EQ_ICASE "<=?"
    MATCH_ICASE "=~?"
    NOT_MATCH_ICASE "!~?"

%token CONCAT ".."
%token VA "..."
%token ARROW "->"

%token <FStr> NUMBER FLOAT BLOB STR
%token <FStr>
    DICT_ID "dot identifier"
    ID "identifier"
    SID_ID "<SID>"
    AUTOLOAD_ID "autoload"
    OPTION_ID "option"
    REGISTER_ID "register"
    ENV_ID "env"

%type <FStr> any_id
%type <std::vector<FStr>> any_ids any_ids_or_empty va_list any_id_list_or_empty
%type <std::vector<EvalExpr*>> expr1_list_or_empty
%type <std::vector<DictNode::Pair>> expr1_pairs_or_empty

%type <EvalExpr*> expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9

%token
    LET "let"
    UNLET "unlet"
    CONST "const"
    LOCKVAR "lockvar"
    UNLOCKVAR "unlockvar"
    FUNCTION "function"
    IF "if"
    ELSEIF "elseif"
    CALL "call"
    RETURN "return"
    FOR "for"
    EXECUTE "execute"
    WHILE "while"

%type <LetOp> any_let_op
%type <EvalCommand*> let unlet const lockvar unlockvar if elseif call return for execute while

%type <EvalCommand*> function

%type <EvalCommand*> input command

%code requires {
    #include <FStr.hpp>
    #include "EvalFactory.hpp"
    #include "EvalExprDefs.hpp"
    #include "EvalCommandDefs.hpp"

    // Used for evallex method. Both definitions can be found in the main file.
    struct EvalLexState;

    struct Location {
        int begin, end;
    };
}

%code provides {
    int evallex(eval::parser::value_type* v, eval::parser::location_type* l, EvalLexState& lexState);
}

%code {
    std::ostream& operator<<(std::ostream& o, const eval::parser::location_type& l) {
        return o;
    }
}

%language "c++"

%lex-param {EvalLexState& lexState}
%parse-param {EvalLexState& lexState}
%parse-param {EvalFactory& f}
%parse-param {EvalCommand*& result}

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
input: command { $$ = $1; result = $$; }

command: let
     | unlet
     | const
     | lockvar
     | unlockvar
     | function
     | if
     | elseif
     | call
     | return
     | for
     | execute
     | while

// Spaces:
// (OK?) 2. varname . varname (dictionary) OR concatenation...
// (OK but not trivial) 4. indexing a[1] is not the same as a [1]
// TODO additional checking!

any_let_op: '='       { $$ = LetOp::EQUAL; }
          | PLUS_EQ   { $$ = LetOp::PLUS; }
          | MINUS_EQ  { $$ = LetOp::MINUS; }
          | DIV_EQ    { $$ = LetOp::DIV; }
          | MOD_EQ    { $$ = LetOp::MOD; }
          | DOT_EQ    { $$ = LetOp::DOT; }
          | CONCAT_EQ { $$ = LetOp::CONCAT; }

let: LET any_id any_let_op expr1                                     { $$ = f.create<LetVar>($2, $4, $3); }
   | LET any_id '[' expr1 ']' any_let_op expr1                       { $$ = f.create<LetElement>($2, $4, $7, $6); }
   | LET any_id '[' ':' ']' any_let_op expr1                         { $$ = f.create<LetRange>($2, nullptr, nullptr, $7, $6); }
   | LET any_id '[' ':' expr1 ']' any_let_op expr1                   { $$ = f.create<LetRange>($2, nullptr, $5, $8, $7); }
   | LET any_id '[' expr1 ':' expr1 ']' any_let_op expr1             { $$ = f.create<LetRange>($2, $4, $6, $9, $8); }
   | LET '[' any_id_list_or_empty ']' any_let_op expr1               { $$ = f.create<LetUnpack>($3, $6, $5); }
   | LET '[' any_id_list_or_empty ';' any_id ']' any_let_op expr1    { $$ = f.create<LetRemainder>($3, $5, $8, $7); }
   | LET                                                             { $$ = f.create<LetPrint>(); }
   | LET any_ids                                                     { $$ = f.create<LetPrint>($2); }
;

unlet: UNLET any_id_list_or_empty                                 { $$ = f.create<Unlet>($2); }
     | UNLET any_id '[' expr1 ']'                                 { $$ = f.create<UnletElement>($2, $4); }
     | UNLET any_id '[' ':' ']'                                   { $$ = f.create<UnletRange>($2, nullptr, nullptr); }
     | UNLET any_id '[' ':' expr1 ']'                             { $$ = f.create<UnletRange>($2, nullptr, $5); }
     | UNLET any_id '[' expr1 ':' expr1 ']'                       { $$ = f.create<UnletRange>($2, $4, $6); }
;

const: CONST any_id '=' expr1                                 { $$ = f.create<ConstVar>($2, $4); }
     | CONST '[' any_id_list_or_empty ']' '=' expr1               { $$ = f.create<ConstUnpack>($3, $6); }
     | CONST '[' any_id_list_or_empty ';' any_id ']' '=' expr1    { $$ = f.create<ConstRemainder>($3, $5, $8); }
     | CONST                                                  { $$ = f.create<LetPrint>(); }
     | CONST any_ids                                          { $$ = f.create<LetPrint>($2); }
;

lockvar: LOCKVAR any_ids                         { $$ = f.create<LockVar>($2); }
       | LOCKVAR NUMBER any_ids                  { $$ = f.create<LockVar>($3, $2); }
;

unlockvar: UNLOCKVAR any_ids                         { $$ = f.create<UnlockVar>($2); }
         | UNLOCKVAR NUMBER any_ids                  { $$ = f.create<UnlockVar>($3, $2); }
;

any_id: SID_ID | AUTOLOAD_ID | OPTION_ID | REGISTER_ID | ENV_ID | ID | DICT_ID
;

any_id_list_or_empty: %empty                          { $$ = {}; }
                    | any_id                          { $$ = {}; $$.push_back($1); }
                    | any_id ',' any_id_list_or_empty { $$ = $3; $$.push_back($1); }

va_list: VA                              { $$ = {}; }
       | any_id ',' va_list              { $$ = $3; $$.push_back($1); }

any_ids_or_empty: %empty     { $$ = {}; }
                | any_ids    { $$ = $1; }

any_ids: any_id                    { $$ = {}; $$.push_back($1); }
       | any_id any_ids            { $$ = {}; $$.push_back($1); }

// TODO function matching pattern
// TODO function attributes
function: FUNCTION                                                                    { $$ = new FunctionPrint(); }
        | FUNCTION any_id '(' any_id_list_or_empty ')' any_ids_or_empty               { $$ = new Function($2, $4, false, $6); }
        | FUNCTION any_id '(' va_list ')' any_ids_or_empty                            { $$ = new Function($2, $4, true, $6); }
        | FUNCTION any_id '.' any_id '(' any_id_list_or_empty ')' any_ids_or_empty    { $$ = new FunctionDict($2, $4, $6, false, $8); }
        | FUNCTION any_id '.' any_id '(' va_list ')' any_ids_or_empty                 { $$ = new FunctionDict($2, $4, $6, true, $8); }
;

if: IF expr1         { $$ = new If($2); }
;

elseif: ELSEIF expr1 { $$ = new ElseIf($2); }
;

call: CALL expr1 { $$ = new Call($2); }
;

return: RETURN expr1 { $$ = new Return($2); }
;

for: FOR any_id any_id expr1 { $$ = new For($2, $3, $4); }
;

execute: EXECUTE expr1 { $$ = new Execute($2); }
;

while: WHILE expr1 { $$ = new While($2); }
;

expr1: expr2
     | expr2 '?' expr1 ':' expr1            { $$ = f.create<TernaryNode>($1, $3, $5); }
;

expr2: expr3 | expr3 OR expr2               { $$ = f.create<InfixOpNode>($1, $3, "||"); }
;

expr3: expr4 | expr4 AND expr3              { $$ = f.create<InfixOpNode>($1, $3, "&&"); }
;

expr4: expr5
     | expr5 EQ expr5                   { $$ = f.create<InfixOpNode>($1, $3, "=="); }
     | expr5 NOT_EQ expr5               { $$ = f.create<InfixOpNode>($1, $3, "!="); }
     | expr5 GR expr5                   { $$ = f.create<InfixOpNode>($1, $3, ">"); }
     | expr5 GR_EQ expr5                { $$ = f.create<InfixOpNode>($1, $3, ">="); }
     | expr5 LESS expr5                 { $$ = f.create<InfixOpNode>($1, $3, "<"); }
     | expr5 LESS_EQ expr5              { $$ = f.create<InfixOpNode>($1, $3, ">"); }
     | expr5 MATCH expr5                { $$ = f.create<InfixOpNode>($1, $3, "=~"); }
     | expr5 NOT_MATCH expr5            { $$ = f.create<InfixOpNode>($1, $3, "!~"); }

     | expr5 EQ_CASE expr5                   { $$ = f.create<InfixOpNode>($1, $3, "==#"); }
     | expr5 NOT_EQ_CASE expr5               { $$ = f.create<InfixOpNode>($1, $3, "!=#"); }
     | expr5 GR_CASE expr5                   { $$ = f.create<InfixOpNode>($1, $3, ">#"); }
     | expr5 GR_EQ_CASE expr5                { $$ = f.create<InfixOpNode>($1, $3, ">=#"); }
     | expr5 LESS_CASE expr5                 { $$ = f.create<InfixOpNode>($1, $3, "<#"); }
     | expr5 LESS_EQ_CASE expr5              { $$ = f.create<InfixOpNode>($1, $3, ">#"); }
     | expr5 MATCH_CASE expr5                { $$ = f.create<InfixOpNode>($1, $3, "=~#"); }
     | expr5 NOT_MATCH_CASE expr5            { $$ = f.create<InfixOpNode>($1, $3, "!~#"); }

     | expr5 EQ_ICASE expr5                   { $$ = f.create<InfixOpNode>($1, $3, "==?"); }
     | expr5 NOT_EQ_ICASE expr5               { $$ = f.create<InfixOpNode>($1, $3, "!=?"); }
     | expr5 GR_ICASE expr5                   { $$ = f.create<InfixOpNode>($1, $3, ">?"); }
     | expr5 GR_EQ_ICASE expr5                { $$ = f.create<InfixOpNode>($1, $3, ">=?"); }
     | expr5 LESS_ICASE expr5                 { $$ = f.create<InfixOpNode>($1, $3, "<?"); }
     | expr5 LESS_EQ_ICASE expr5              { $$ = f.create<InfixOpNode>($1, $3, ">?"); }
     | expr5 MATCH_ICASE expr5                { $$ = f.create<InfixOpNode>($1, $3, "=~?"); }
     | expr5 NOT_MATCH_ICASE expr5            { $$ = f.create<InfixOpNode>($1, $3, "!~?"); }

     | expr5 IS expr5                         { $$ = f.create<InfixOpNode>($1, $3, "is"); }
     | expr5 IS_CASE expr5                    { $$ = f.create<InfixOpNode>($1, $3, "is#"); }
     | expr5 IS_ICASE expr5                   { $$ = f.create<InfixOpNode>($1, $3, "is?"); }
     | expr5 ISNOT expr5                      { $$ = f.create<InfixOpNode>($1, $3, "isnot"); }
     | expr5 ISNOT_CASE expr5                 { $$ = f.create<InfixOpNode>($1, $3, "isnot#"); }
     | expr5 ISNOT_ICASE expr5                { $$ = f.create<InfixOpNode>($1, $3, "isnot?"); }
;

expr5: expr6
     | expr6 '+' expr5        {  $$ = f.create<InfixOpNode>($1, $3, "+");  }
     | expr6 '-' expr5        {  $$ = f.create<InfixOpNode>($1, $3, "-");  }
     | expr6 '.' expr5        {  $$ = f.create<InfixOpNode>($1, $3, ".");  }
     | expr6 CONCAT expr5     {  $$ = f.create<InfixOpNode>($1, $3, "..");  }
;

expr6: expr7
     | expr7 '*' expr6        { $$ = f.create<InfixOpNode>($1, $3, "*"); }
     | expr7 '/' expr6        { $$ = f.create<InfixOpNode>($1, $3, "/"); }
     | expr7 '%' expr6        { $$ = f.create<InfixOpNode>($1, $3, "%"); }
;

expr7: expr8
     | '!' expr7            { $$ = f.create<PrefixOpNode>($2, "!"); }
     | '-' expr7            { $$ = f.create<PrefixOpNode>($2, "-"); }
     | '+' expr7            { $$ = f.create<PrefixOpNode>($2, "+"); }
;

// TODO help cmdline-special

// TODO subscript
// TODO method
expr8: expr9
     | expr8 '[' expr1 ']'               { $$ = f.create<IndexNode>($1, $3); }
     | expr8 '[' expr1 ':' expr1 ']'     { $$ = f.create<IndexRangeNode>($1, $3, $5); }
     | expr8 '[' expr1 ':' ']'           { $$ = f.create<IndexRangeNode>($1, $3, nullptr); }
     | expr8 '[' ':' expr1 ']'           { $$ = f.create<IndexRangeNode>($1, nullptr, $4); }
     | expr8 '[' ':' ']'                 { $$ = f.create<IndexRangeNode>($1, nullptr, nullptr); }
     | expr8 '(' expr1_list_or_empty ')' { $$ = f.create<InvokeNode>($1, $3); }
;

// TODO Dictionary #{}
expr9: NUMBER                                     { $$ = f.create<TokenNode>($1, TokenNode::NUMBER); }
     | FLOAT                                      { $$ = f.create<TokenNode>($1, TokenNode::FLOAT); }
     | BLOB                                       { $$ = f.create<TokenNode>($1, TokenNode::BLOB); }
     | STR                                        { $$ = f.create<TokenNode>($1, TokenNode::STRING); }
     | '[' expr1_list_or_empty ']'                { $$ = f.create<ListNode>($2); }
     | '{' expr1_pairs_or_empty '}'               { $$ = f.create<DictNode>($2); }
     | '(' expr1 ')'                              { $$ = f.create<NestedNode>($2); }
     | '{' any_id_list_or_empty ARROW expr1 '}'   { $$ = f.create<LambdaNode>($2, $4); }
     | OPTION_ID                                  { $$ = f.create<TokenNode>($1, TokenNode::OPTION); }
     | REGISTER_ID                                { $$ = f.create<TokenNode>($1, TokenNode::REGISTER); }
     | ENV_ID                                     { $$ = f.create<TokenNode>($1, TokenNode::ENV); }
     | ID                                         { $$ = f.create<TokenNode>($1, TokenNode::ID); }
     | AUTOLOAD_ID                                { $$ = f.create<TokenNode>($1, TokenNode::AUTOLOAD); }
     | SID_ID                                     { $$ = f.create<TokenNode>($1, TokenNode::SID); }
     | DICT_ID                                    { $$ = f.create<TokenNode>($1, TokenNode::DICT); }
;

expr1_list_or_empty: %empty                          { $$ = {}; }
                   | expr1                           { $$ = {}; $$.push_back($1); }
                   | expr1 ',' expr1_list_or_empty   { $$ = $3; $$.push_back($1); }
;

expr1_pairs_or_empty: %empty                                    { $$ = {}; }
                    | expr1 ':' expr1                           { $$ = {}; $$.push_back(DictNode::Pair($1, $3)); }
                    | expr1 ':' expr1 ',' expr1_pairs_or_empty  { $$ = $5; $$.push_back(DictNode::Pair($1, $3)); }
;
