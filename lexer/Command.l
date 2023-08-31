%option noyywrap
%option yylineno
%option stack
%option pointer
%option prefix="cmd"

%x STATE_COMMAND

command_special "!"|"!!"|"#"|"#!"|"&"|"*"|"<"|"="|">"|"@"|"@@"|"~"

radix "0x"|"0X"|"0b"|"0B"
number [+-]?{radix}?[0-9]+

range_special "."|"$"|"%"|"'[a-zA-Z]"|"\\/"|"\\?"|"\\&"

pattern_char [ -~]{-}[/]
pattern "/"({pattern_char}*|\\.)*"/"

rev_pattern_char [ -~]{-}[?]
rev_pattern "?"({rev_pattern_char}*|\\.)*"?"

%{
enum {ERROR=256, RANGE_ARG, RANGE_DELIM, COMMAND_COLON, COMMAND, COMMAND_SPECIAL};
%}

%%
":" {
    BEGIN(STATE_COMMAND);
    return COMMAND_COLON;
}

{number}|{range_special}|{pattern}|{rev_pattern} {
    return RANGE_ARG;
}

","|";" {
    return RANGE_DELIM;
}

[a-zA-Z].* {
    return COMMAND;
}

{command_special}.* {
    return COMMAND_SPECIAL;
}

<*>"\n" {
    BEGIN(INITIAL);
}

<*>. {
    return ERROR;
}
%%
