#include "EvalLexer.hpp"
#include "EvalParser.hpp"
#include "EvalNode.hpp"


/* GroupBlock* root; */

void eval::parser::error(const std::string& msg) {
    fprintf(stderr, "Erorr: %s\n", msg.c_str());
    exit(5);
}

int evallex(eval::parser::value_type* p, const EvalFactory& factory) {
    using kind_type = eval::parser::token::token_kind_type;

    int t = evallex();
    switch (t) {
    case kind_type::NUMBER:
    case kind_type::FLOAT:
    case kind_type::BLOB:
    case kind_type::STR:
    case kind_type::VA_ID:
    case kind_type::SID_ID:
    case kind_type::AUTOLOAD_ID:
    case kind_type::OPTION_ID:
    case kind_type::REGISTER_ID:
    case kind_type::ENV_ID:
    case kind_type::ID:
        p->build<std::string>(evalget_text());
        return t;

    default:
        p->build<int>(t);
        return t;
    }
}

void debug() {
    using token_kind_type = eval::parser::token::token_kind_type;
    int kt;

    do {
        kt = evallex();
        printf("(%d): %s", kt, evalget_text());
        if (kt != '\n') {
            printf("\n");
        }
    }
    while (kt != token_kind_type::EVALEOF && kt != token_kind_type::EVALerror);
}

int main (int argc, char** argv) {
    YY_BUFFER_STATE buf = eval_scan_string("3 + 123 - 2");
    const bool lexdbg = true;
    if (lexdbg) {
        debug();
    } else {
        EvalFactory factory;
        eval::parser parser(factory);
        /* parser.set_debug_level(1); */
        int res = parser();
        printf("Result code: %d\n", res);
        EvalNode* node = factory.getTopLevel();
        std::string str = node->toString();
        printf("Result string: %s\n", str.c_str());
    }

    eval_delete_buffer(buf);
    return 0;
}
