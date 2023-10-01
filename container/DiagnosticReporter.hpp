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
