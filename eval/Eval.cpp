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

void eval::parser::error(const eval::parser::location_type& l, const std::string& msg) {
    assert(false);
}

class ErrorPushParser : public eval::parser {
public:
    ErrorPushParser(EvalLexState& lexState, EvalFactory& f, EvalCommand*& result) :
        eval::parser(lexState, f, result), locMap(nullptr), digs(nullptr) {}

    void setErrorPush(const LocationMap* locMap, LocationMap::Key key, std::vector<Diagnostic>* digs) {
        this->locMap = locMap;
        this->locKey = key;
        this->digs = digs;
    }

    virtual void error(const eval::parser::location_type& l, const std::string& msg) override {
        assert(digs);

        digs->resize(digs->size() + 1);
        Diagnostic& error = digs->back();
        error.severity = Diagnostic::Error;
        error.message = msg;
        locMap->resolve(locKey, l.begin, error.range.start.line, error.range.start.character);
        locMap->resolve(locKey, l.end - 1, error.range.end.line, error.range.end.character);
    }

private:
    LocationMap::Key locKey;
    const LocationMap* locMap;
    std::vector<Diagnostic>* digs;
};

bool blockSupported(const Block& block) {
    eval::parser::token::token_kind_type tok = exDictToKindType(block.lexem.exDictIdx);
    return tok != eval::parser::token::token_kind_type::EVALerror;
}

bool evalParseBlock(Block& block, SyntaxTree& root, std::vector<Diagnostic>& digs) {
    // Only a few ex commands have support for eval parsing. Ignore the rest (issue no warnings).
    if (!blockSupported(block)) {
        return false;
    }

    EvalLexState lexState(block.lexem);
    EvalCommand* result = nullptr;
    ErrorPushParser parser(lexState, root.evalFac, result);
    parser.setErrorPush(&root.lexer.getLocationMap(), block.lexem.locationKey, &digs);

    bool notOk = parser.parse();
    if (notOk) {
        return false;
    }
    block.evalCmd = result;
    return true;
}
