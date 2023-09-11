#include "EvalLexer.hpp"
#include "EvalParser.hpp"
#include "EvalFactory.hpp"

#include <ExConstants.hpp>
#include <ExLexer.hpp>

#include <SyntaxTree.hpp>

struct EvalLexState {
    EvalLexState(const ExLexem& lexem) : lexem(lexem), yycol(0) {
        yybuffer = eval_scan_bytes(lexem.qargs.begin, lexem.qargs.length());
    }
    
    ~EvalLexState() {
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

int evallex(eval::parser::value_type* v, eval::parser::location_type* l, EvalLexState& lexState) {
    using kind_type = eval::parser::token::token_kind_type;

    // 1 token for the command name
    if (lexState.yycol == 0) {
        kind_type sym = exDictToKindType(lexState.lexem.exDictIdx);
        if (sym != kind_type::EVALerror) {
            l->begin = lexState.lexem.nameOffset;
            l->end = lexState.lexem.qargsOffset;
            lexState.yycol = l->end;
        }
        v->build<int>(sym);
        return sym;
    }
    // Remaining tokens for the qargs
    while (true) {
        int t = evallex();
        l->begin = lexState.yycol;
        l->end = l->begin + evalget_leng();
        lexState.yycol = l->end;

        switch (t) {
        case kind_type::NUMBER:
        case kind_type::FLOAT:
        case kind_type::BLOB:
        case kind_type::STR:
        case kind_type::SID_ID:
        case kind_type::AUTOLOAD_ID:
        case kind_type::OPTION_ID:
        case kind_type::REGISTER_ID:
        case kind_type::ENV_ID:
        case kind_type::ID:
            v->build<FStr>(evalget_text());
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

void eval::parser::error(const eval::parser::location_type& l, const std::string& msg) {
    assert(false);
}

class ReportingParser : public eval::parser {
public:
    ReportingParser(EvalLexState& lexState, EvalFactory& f, EvalCommand*& result) :
        eval::parser(lexState, f, result) {}

    void setReporter(DiagnosticReporter* reporter, LocationMap::Key locKey) {
        this->reporter = reporter;
        this->locKey = locKey;
    }

    virtual void error(const eval::parser::location_type& l, const std::string& msg) override {
        reporter->error(msg.c_str(), locKey, l.begin, l.end - 1);
    }

private:
    DiagnosticReporter* reporter;
    LocationMap::Key locKey;
};

bool supported(const ExLexem& lexem) {
    eval::parser::token::token_kind_type tok = exDictToKindType(lexem.exDictIdx);
    return tok != eval::parser::token::token_kind_type::EVALerror;
}

EvalCommand* evalEx(const ExLexem& lexem, EvalFactory& factory, DiagnosticReporter& reporter) {
    // Only a few ex commands have support for eval parsing. Ignore the rest (issue no warnings).
    if (!supported(lexem)) {
        return nullptr;
    }

    EvalLexState lexState(lexem);
    EvalCommand* result = nullptr;
    ReportingParser parser(lexState, factory, result);
    parser.setReporter(&reporter, lexem.locationKey);

    bool notOk = parser.parse();
    if (notOk) {
        return nullptr;
    }
    return result;
}
