#pragma once

#include "StringView.hpp"
#include "LocationMap.hpp"

struct ExLexem {
    int exDictIdx;
    StringView name;
    StringView qargs;

    LocationMap::Key locationKey;
    int nameOffset;
    int qargsOffset;

    bool bang;
    int range;
};
