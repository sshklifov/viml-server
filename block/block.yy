%token <int> IF "if"
%token <int> ELSE "else"
%token <int> ELSEIF "elseif"
%token <int> ENDIF "endif"

%token <int> WHILE "while"
%token <int> ENDWHILE "endwhile"

%token <int> FOR "for"
%token <int> ENDFOR "endfor"

%token <int> FUNCTION "function"
%token <int> ENDFUNCTION "endfunction"

%token <int> TRY "try"
%token <int> ENDTRY "endtry"
%token <int> FINALLY "finally"

%token <std::string> EX "Ex command"
%token <std::string> QARGS "<q-args>"

%type <ExBlock*> command
%type <TryBlock*> try_block
%type <ForBlock*> for_block
%type <WhileBlock*> while_block
%type <IfBlock*> elsei_list
%type <IfBlock*> if_block
%type <FunBlock*> function_block
%type <GroupBlock*> input

%code requires {
    #include "Blocks.h"
    extern GroupBlock* root;
}

%code provides {
    int yylex(yy::parser::value_type* p);
}

%language "c++"
%header

%define api.value.type variant
%define parse.error detailed

%{
%}

%%
input: %empty                               { $$ = new GroupBlock(); root = $$; }
     | if_block input                       { $$ = $2; $2->prependBlock($1); }
     | while_block input                    { $$ = $2; $2->prependBlock($1); }
     | for_block input                      { $$ = $2; $2->prependBlock($1); }
     | function_block input                 { $$ = $2; $2->prependBlock($1); }
     | try_block input                      { $$ = $2; $2->prependBlock($1); }
     | command input                        { $$ = $2; $2->prependBlock($1); }
     | '\n' input                           { $$ = $2; }
;

if_block: IF QARGS '\n' input ENDIF '\n'                   { $$ = new IfBlock($2, $4); }
        | IF QARGS '\n' input ELSE '\n' input ENDIF '\n'   { $$ = new IfBlock($2, $4, $7); }
        | IF QARGS '\n' input elsei_list '\n'              { $$ = new IfBlock($2, $4, $5); }
;

elsei_list: ELSEIF QARGS '\n' input ENDIF                  { $$ = new IfBlock($2, $4); }
          | ELSEIF QARGS '\n' input ELSE '\n' input ENDIF  { $$ = new IfBlock($2, $4, $7); }
          | ELSEIF QARGS '\n' input elsei_list             { $$ = new IfBlock($2, $4, $5); }

while_block: WHILE QARGS '\n'input ENDWHILE '\n'           { $$ = new WhileBlock($2, $4); }
;

for_block: FOR QARGS '\n' input ENDFOR '\n'                { $$ = new ForBlock($2, $4); }
;

function_block: FUNCTION QARGS '\n' input ENDFUNCTION '\n'        { $$ = new FunBlock($2, $4); }
;

try_block: TRY '\n' input ENDTRY '\n'                { $$ = new TryBlock($3); }
         | TRY '\n' input FINALLY input ENDTRY '\n'  { $$ = new TryBlock($3, $5); }

command: EX '\n'                { $$ = new ExBlock($1); }
       | EX QARGS '\n'          { $$ = new ExBlock($1, $2); }
;
%%
