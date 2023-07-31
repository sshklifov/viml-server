#pragma once

#include <StringView.hpp>

struct ExLexem {
    bool bang;
    StringView name;
    StringView qargs;
    int line;
    int nameCol;
    int qargsCol;
};
