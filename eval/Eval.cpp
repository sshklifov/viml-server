#include "EvalLexer.hpp"
#include "EvalParser.hpp"
#include "EvalNode.hpp"

#include <ExConstants.hpp>
#include <ExLexer.hpp>

struct EvalState {
    EvalState(const ExLexem& lexem) : lexem(lexem), yycol(0) {
        yybuffer = eval_scan_bytes(lexem.qargs.begin, lexem.qargs.length());
    }
    
    ~EvalState() {
        eval_delete_buffer(yybuffer);
    }

    const ExLexem& lexem;
    int yycol;
    YY_BUFFER_STATE yybuffer;
};

eval::parser::token::token_kind_type exDictToKindType(int exDict) {
    using kind_type = eval::parser::token::token_kind_type;
    switch (exDict) {
        case LET:
            return kind_type::LET;

        case UNLET:
            return kind_type::UNLET;

        case CONST:
            return kind_type::CONST;

        case LOCKVAR:
            return kind_type::LOCKVAR;

        case UNLOCKVAR:
            return kind_type::UNLOCKVAR;

        case FUNCTION:
            return kind_type::FUNCTION;

        default:
            return kind_type::EVALerror;
    }
}

int evallex(eval::parser::value_type* v, eval::parser::location_type* l, EvalState* state) {
    using kind_type = eval::parser::token::token_kind_type;

    if (state->yycol == 0) {
        kind_type sym = exDictToKindType(state->lexem.exDictIdx);
        if (sym != kind_type::EVALerror) {
            l->begin = state->lexem.nameOffset;
            l->end = state->lexem.qargsOffset;
            state->yycol = l->end;
        }
        v->build<int>(sym);
        return sym;
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

void eval::parser::error(const EvalLocation& l, const std::string& msg) {
    fprintf(stderr, "Erorr (%d-%d): %s\n", l.begin, l.end, msg.c_str());
    exit(5);
}

EvalCommand* parse(const ExLexem& lexem, EvalFactory& factory) {
    EvalState state(lexem);
    EvalCommand* result = nullptr;
    eval::parser parser(&state, factory, result);
    if (!parser.parse()) {
        return nullptr;
    }
    return result;
}
