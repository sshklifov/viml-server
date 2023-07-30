%option noyywrap

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

int main() {
    int a = yylex();
    return 0;
}

