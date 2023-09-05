#include "EvalLexer.hpp"
#include "EvalParser.hpp"
#include "EvalNode.hpp"
#include <ExConstants.hpp>
#include <ExLexer.hpp>

struct LexerState {
    LexerState(const ExLexem& lexem) : lexem(lexem), yycol(0) {
        yybuffer = eval_scan_bytes(lexem.qargs.begin, lexem.qargs.length());
    }
    
    // TODO more

    ~LexerState() {
        eval_delete_buffer(yybuffer);
    }

    const ExLexem& lexem;
    int yycol;
    YY_BUFFER_STATE yybuffer;
};

int evallex(eval::parser::value_type* v, eval::parser::location_type* l, LexerState* state) {
    using kind_type = eval::parser::token::token_kind_type;

    if (state->yycol == 0) {
        switch (state->lexem.exDictIdx) {
            case LET:
                l->begin = state->lexem.nameOffset;
                l->end = state->lexem.qargsOffset;
                state->yycol = l->end;
                v->build<int>(kind_type::LET);
                return kind_type::LET;

            default:
                return kind_type::EVALerror;
        }
    }

    while (true) {
        int t = evallex();
        l->begin = state->yycol;
        l->end = l->begin + evalget_leng();
        state->yycol = l->end;

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
            v->build<std::string>(evalget_text());
            return t;

        case ' ':
        case '\t':
            // Loop again
            break;

        default:
            v->build<int>(t);
            return t;
        }
    }
}

void eval::parser::error(const Location& l, const std::string& msg) {
    fprintf(stderr, "Erorr (%d-%d): %s\n", l.begin, l.end, msg.c_str());
    exit(5);
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

// void EvalResult* res = parse(ExLexem, EvalFactory)
// void EvalResult* res = factory.parse(ExLexem)

void* parse(const ExLexem& lexem, EvalFactory& factory) {
    LexerState state(lexem);
    eval::parser parser(&state, factory);
    int res = parser.parse();
    EvalNode* node = factory.getTopLevel();
    return node;
    /* std::string str = node->toString(); */
    /* printf("Result string: %s\n", str.c_str()); */
}

int main() {

}

/* int main (int argc, char** argv) { */
/*     YY_BUFFER_STATE buf = eval_scan_string("3+123"); */
/*     const bool lexdbg = false; */
/*     if (lexdbg) { */
/*         debug(); */
/*     } else { */
/*         printf("Result code: %d\n", res); */
/*     } */

/*     eval_delete_buffer(buf); */
/*     return 0; */
/* } */
