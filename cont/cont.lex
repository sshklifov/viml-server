%option noyywrap
%option prefix="cont"

space [ \t]
any_space {space}*
comment ^{any_space}["].*[\n]

%{
%}

%%
[\n][ \t]*"\"\\ "[^\n]* {
    // line-continuation-comment
}
[\n][ \t]*"\\" {
    // line-continuation
}

. {
    return yytext[0];
}

"\n" {
    return yytext[0];
}
%%
