#pragma once

#include "Diagnostic.hpp"
#include "LocationMap.hpp"
#include "ExLexem.hpp"
#include <Vector.hpp>

struct DiagnosticReporter {
    void bindLocationResolver(const LocationMap& resolver) {
        locMap = &resolver;
    }

    void clear() { diagnostics.clear(); }

    const Vector<Diagnostic>& get() const { return diagnostics; }

    bool empty() const { return diagnostics.empty(); }

    void error(FStr msg, const LocationMap::Key& locKey, int first, int last) {
        Range range = locMap->resolve(locKey, first, last);
        diagnostics.emplace(msg, range);
    }

    void error(FStr msg, const LocationMap::Key& locKey, int pos) {
        error(std::move(msg), locKey, pos, pos);
    }

    void error(FStr msg, const LocationMap::Key& locKey) {
        Range range = locMap->resolve(locKey);
        diagnostics.emplace(msg, range);
    }

    void error(FStr msg, const ExLexem& lex, int first, int last) {
        error(std::move(msg), lex.locationKey, first, last);
    }

    void error(FStr msg, const ExLexem& lex, int pos) {
        error(std::move(msg), lex.locationKey, pos, pos);
    }

    void error(FStr msg, const ExLexem& lex) {
        error(std::move(msg), lex.locationKey);
    }

private:
    const LocationMap* locMap;
    Vector<Diagnostic> diagnostics;
};
