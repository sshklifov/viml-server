%option noyywrap
%option yylineno
%option stack
%option pointer
%option prefix="cmd"

%x STATE_QARGS

space [ \t]
any_space {space}*
comment ^{any_space}["].*[\n]

str_char [ -~]{-}[\\"]
str ["]({str_char}*|\\.)*["]
quote_char [ -~]{-}[\\']
quote [']({quote_char}*|\\.)*[']

name [a-zA-Z][a-zA-Z0-9]*
command {name}|"!"|"!!"|"#"|"&"|"*"|"<"|"="|">"|"@"|"@@"|"~"

radix "0x"|"0X"|"0b"|"0B"
number [+-]?{radix}?[0-9]+
float [+-]?{number}"."{number}([eE][+-]?{number})?
blob "0z"[0-9a-fA-F.]+

range_special "."|"$"|"%"|"'[a-zA-Z]"|"\\/"|"\\?"|"\\&"

pattern_char [ -~]{-}[/]
pattern "/"({pattern_char}*|\\.)*"/"

rev_pattern_char [ -~]{-}[?]
rev_pattern "?"({rev_pattern_char}*|\\.)*"?"

%{
enum {ERROR=256, RANGE_ARG, RANGE_DELIM, COMMAND_COLON, NAME, QARGS};
%}

%%
":" {
    return COMMAND_COLON;
}

{number}|{range_special}|{pattern}|{rev_pattern} {
    return RANGE_ARG;
}

","|";" {
    return RANGE_DELIM;
}

{command}"!"? {
    BEGIN(STATE_QARGS);
    return NAME;
}

<STATE_QARGS>.* {
    BEGIN(INITIAL);
    return QARGS;
}

<*>"\n" {
    BEGIN(INITIAL);
}

<*>. {
    return ERROR;
}
%%