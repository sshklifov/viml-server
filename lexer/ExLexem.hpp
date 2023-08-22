#pragma once

#include <StringView.hpp>
#include <Diagnostics.hpp>

struct ExLexem {
    Range getNameRange() {
        Position start{line, nameColumn};
        Position end{line, nameColumn + name.length()};
        return Range{start, end};
    }

    Range getQargsRange() {
        Position start{line, qargsColumn};
        Position end{line, qargsColumn + qargs.length()};
        return Range{start, end};
    }

    StringView name;
    StringView qargs;
    int line;
    int nameColumn;
    int qargsColumn;
};
