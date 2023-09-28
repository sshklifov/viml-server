#pragma once

#include "Diagnostic.hpp"
#include "ExLexem.hpp"

#include <Vector.hpp>

struct DiagnosticReporter {
    void clear() { diagnostics.clear(); }

    const Vector<Diagnostic>& get() const { return diagnostics; }

    bool empty() const { return diagnostics.empty(); }

    void error(FStr msg, const Range& range) {
        diagnostics.emplace(msg, range);
    }

    void error(FStr msg, const ExLexem& lex, int first, int last) {
        Range range = lex.locator.resolve(first, last);
        error(std::move(msg), range);
    }

    void error(FStr msg, const ExLexem& lex, int pos) {
        error(std::move(msg), lex, pos, pos);
    }

    void error(FStr msg, const ExLexem& lex) {
        Range range = lex.locator.resolve();
        error(std::move(msg), range);
    }

    void error(FStr msg, const ExLexem& lex, const char* pos) {
        error(std::move(msg), lex, pos - lex.cmdline.begin);
    }

private:
    const Locator* locMap;
    Vector<Diagnostic> diagnostics;
};
