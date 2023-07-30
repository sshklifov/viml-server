%option noyywrap
%option prefix="cont"

%{
%}

%%
[\n][ \t]+"\\" {
    // IGNORE
}

. {
    printf(yytext);
}

"\n" {
    printf(yytext);
}
%%
