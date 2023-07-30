%option noyywrap
%option prefix="cont"

%{
#define YY_DECL int yylex()
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
