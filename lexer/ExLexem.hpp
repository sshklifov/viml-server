#pragma once

#include "StringView.hpp"
#include "Locator.hpp"

#include <ExCmdsEnum.hpp>

struct ExLexem {
    ExLexem() : bang(false), range(false), cmdidx(CMD_SIZE), nextcmd(NULL) {}

    StringView cmdline;
    StringView name;
    StringView qargs;

    Locator locator;

    bool bang;
    int range;
    int cmdidx;
    const char* nextcmd;
};
