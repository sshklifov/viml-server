#pragma once

#include <StringView.hpp>
#include <Diagnostics.hpp>

struct ExLexem {
    // TODO RENAME
    Range getNameRange() {
        int len = name.length();
        Position start{line, nameColumn};
        Position end{line, nameColumn + len};
        return Range{start, end};
    }

    Range getQargsRange() {
        int len = qargs.length();
        Position start{line, qargsColumn};
        Position end{line, qargsColumn + len};
        return Range{start, end};
    }

    std::string name;
    std::string qargs;

    int line;
    int nameColumn;
    int qargsColumn;

    bool bang;
    int range;
};
