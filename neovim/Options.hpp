#pragma once

#include <StringMap.hpp>

struct OptionsMap {
    OptionsMap();

    int findVarType(const char* name, int len) const;

    static const OptionsMap& getSingleton();

private:
    StringMap<int> opts;
    int quickTab[26];
};
