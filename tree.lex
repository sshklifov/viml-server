%option noyywrap
%x EX_LEADING EX_QARGS

space [ \t]
any_space {space}*
comment ^{any_space}["].*[\n]

if "if"
else "el"("s"|"se")?
elseif "elsi"[f]?
endif "en"("d"|"di"|"dif")?
while "wh"("i"|"il"|"ile")?
endwhile "endw"("h"|"hi"|"hil"|"hile")?
for "for"
endfor "endfo"[r]?
function "fu"("n"|"nc"|"nct"|"ncti"|"nctio"|"nction")?
endfunction "endf"("u"|"un"|"unc"|"unct"|"uncti"|"unctio"|"unction")?
try "try"
endtry "endt"("r"|"ry")?
finally "fina"("l"|"ll"|"lly")?

endcmd [ \t\n]

%{
#include "tree_parser.hpp"
#define YY_DECL int yylex_wrap()
%}

%%
<*>{comment} {}

^{any_space}":"? {}

{if}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::IF;
}

{else}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ELSE;
}

{elseif}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ELSEIF;
}

{endif}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ENDIF;
}

{while}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::WHILE;
}

{endwhile}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ENDWHILE;
}

{for}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::FOR;
}

{endfor}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ENDFOR;
}

{function}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::FUNCTION;
}

{endfunction}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ENDFUNCTION;
}

{try}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::TRY;
}

{endtry}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::ENDTRY;
}

{finally}"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::FINALLY;
}

<EX_LEADING>{space}* {
    BEGIN(EX_QARGS);
}

<EX_QARGS>[^\n]* {
    return yy::parser::token::QARGS;
}

<*>"\n" {
    BEGIN(INITIAL);
    return yytext[0];
}

[^ \t\n!]*"!"?/{endcmd} {
    BEGIN(EX_LEADING);
    return yy::parser::token::EX;
}

<*>. {
    return yy::parser::token::YYerror;
}
%%
