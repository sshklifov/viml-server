#pragma once

#include "Diagnostic.hpp"
#include "ExLexem.hpp"

#include <Message.hpp>
#include <Vector.hpp>

struct DiagnosticReporter {
    void clear() { diagnostics.clear(); }

    const Vector<Diagnostic>& get() const { return diagnostics; }

    bool empty() const { return diagnostics.empty(); }

    void error(FStr msg, const Range& range) {
        diagnostics.emplace(msg, range);
    }

private:
    Vector<Diagnostic> diagnostics;
};

struct BoundReporter {
    BoundReporter(DiagnosticReporter& rep, const ExLexem& lex) : rep(rep), lex(lex) {}

    void error(FStr msg, const char* arg) {
        int pos = arg - lex.cmdline;
        Range range = lex.locator.resolve(pos);
        rep.error(std::move(msg), range);
    }

    void error(FStr msg) {
        Range range = lex.locator.resolve();
        rep.error(std::move(msg), range);
    }

    void error(msg& m) {
        int pos = m.ppos - lex.cmdline;
        Range range = lex.locator.resolve(pos);
        rep.error(std::move(m.message), range);
    }

    void errorName(FStr msg) {
        int start = lex.name - lex.cmdline;
        Range range = lex.locator.resolve(start, start + lex.namelen);
        rep.error(std::move(msg), range);
    }

private:
    DiagnosticReporter& rep;
    const ExLexem& lex;
};
