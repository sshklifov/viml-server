#pragma once

#include <StringView.hpp>

struct ExLexem {
    StringView name;
    StringView qargs;
    int line;
    int nameColumn;
    int qargsColumn;
};
