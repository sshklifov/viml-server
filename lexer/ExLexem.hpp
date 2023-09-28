#pragma once

#include "StringView.hpp"
#include "LocationMap.hpp"

struct ExLexem {
    StringView cmdline;
    StringView name;
    StringView qargs;

    Locator locator;

    bool bang;
    int range;
    int cmdidx;
};
