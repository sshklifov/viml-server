%option noyywrap

option_name [a-z]+
no_space [!-~]
option_val ({no_space}*|\\.)+

show_option {option}[?]
no_option "no"{option_name}
inv_option ({option_name}[!])|("inv"{option_name})
reset_option {option_name}[&]("vi"|"vim")?
set_option {option_name}[=:]{option_val}
add_option {option_name}"+="{option_val}
mult_option {option_name}"^="{option_val}
sub_option {option_name}"-="{option_val}

%{
%}

%%
{option_name} {
}

{show_option} {

}

{no_option} {

}

{inv_option} {

}

{reset_option} {

}

{set_option} {

}

{add_option} {

}

{mult_option} {

}

{sub_option} {

}
%%

int parseSetCommand(const char* str) {
    YY_BUFFER_STATE b = yy_scan_string(str);
    int res = yylex();
    yy_delete_buffer(b);
    return res;
}
