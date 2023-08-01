#pragma once

#include <ExDictionary.hpp>

#define FOR_EACH_COMMAND(code) \
    code(IF, 186) \
    code(ELSEIF, 141) \
    code(ELSE, 140) \
    code(ENDIF, 143) \
    code(WHILE, 511) \
    code(ENDWHILE, 147) \
    code(FOR, 167) \
    code(ENDFOR, 145) \
    code(FUNCTION, 168) \
    code(ENDFUNCTION, 144) \
    code(TRY, 475) \
    code(ENDTRY, 146) \
    code(FINALLY, 159) \
    code(CATCH, 55)

#define DEFINE_COMMAND(name, val) const int name = val;

FOR_EACH_COMMAND(DEFINE_COMMAND)

#undef DEFINE_COMMAND

bool checkBlockConstants(const ExDictionary& dict);

const int ROOT = -1;
