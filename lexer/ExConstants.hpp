#pragma once

#define FOR_EACH_COMMAND(code) \
    code(IF, 197) \
    code(ELSEIF, 152) \
    code(ELSE, 151) \
    code(ENDIF, 154) \
    code(WHILE, 522) \
    code(ENDWHILE, 158) \
    code(FOR, 178) \
    code(ENDFOR, 156) \
    code(FUNCTION, 179) \
    code(ENDFUNCTION, 155) \
    code(TRY, 486) \
    code(ENDTRY, 157) \
    code(FINALLY, 170) \
    code(CATCH, 66) \
    code(LET, 240) \
    code(UNLET, 495) \
    code(CONST, 110) \
    code(LOCKVAR, 265) \
    code(UNLOCKVAR, 496) \


#define DEFINE_COMMAND(name, val) const int name = val;

FOR_EACH_COMMAND(DEFINE_COMMAND)

#undef DEFINE_COMMAND

bool debugCheckExConstants();
