#pragma once

#include "Locator.hpp"

#include <ExCmdsEnum.hpp>

struct ExLexem {
    ExLexem() {
        cmdline = nullptr;
        name = nullptr;
        qargs = nullptr;
        namelen = 0;
        bang = 0;
        range = 0;
        cmdidx = CMD_SIZE;
    }

    const char* cmdline;
    const char* name;
    const char* qargs;

    Locator locator;

    int namelen;
    int bang;
    int range;
    int cmdidx;
};
