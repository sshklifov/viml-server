#pragma once

#include "Diagnostic.hpp"
#include "LocationMap.hpp"
#include "ExLexem.hpp"
#include <Vector.hpp>

struct DiagnosticReporter {
    void bindLocationResolver(const Locator& resolver) {
        locMap = &resolver;
    }

    void clear() { diagnostics.clear(); }

    const Vector<Diagnostic>& get() const { return diagnostics; }

    bool empty() const { return diagnostics.empty(); }

    void error(FStr msg, const ExLexem& lex, int first, int last) {
        Range r = lex.locator.resolve(first, last);
        diagnostics.emplace(msg, r);
    }

    void error(FStr msg, const ExLexem& lex, int pos) {
        error(std::move(msg), lex, pos, pos);
    }

    void error(FStr msg, const ExLexem& lex) {
        Range r = lex.locator.resolve();
        diagnostics.emplace(msg, r);
    }

    void error(FStr msg, const ExLexem& lex, const char* pos) {
        error(std::move(msg), lex, pos - lex.cmdline.begin);
    }

private:
    const Locator* locMap;
    Vector<Diagnostic> diagnostics;
};
