/* extern int yylex(); */
/* extern int yyparse(); */

#include "tree_parser.hpp"
GroupBlock* root;

extern int yylex_wrap();
extern const char* yytext;
extern int yyleng;

void yy::parser::error(const std::string& msg) {
    fprintf(stderr, "Erorr: %s\n", msg.c_str());
    exit(5);
}

int yyparse() {
    root = new GroupBlock;
    yy::parser parser;

    int res =  parser();
    std::string program = root->toString();
    fputs(program.c_str(), stdout);
    return res;
}


int yylex(yy::parser::value_type* p) {
    using kind_type = yy::parser::token::token_kind_type;
    int kt = yylex_wrap();

    switch (kt) {
    case kind_type::EX:
    case kind_type::QARGS:
        p->build<std::string>(yytext);
        return kt;

    case kind_type::YYEOF:
    case kind_type::YYerror:
    case kind_type::YYUNDEF:
        return kt;

    default:
        p->build<int>(kt);
        return kt;
    }
}

void yylex_debug() {
    using kind_type = yy::parser::token::token_kind_type;
    int kt;

    do {
        kt = yylex_wrap();
        printf("(%d): %s", kt, yytext);
        if (kt != '\n') {
            printf("\n");
        }
    }
    while (kt != kind_type::YYEOF && kt != kind_type::YYerror);
}

int main () {
    /* yylex_debug(); */
    yyparse();
}
