#pragma once

#include <ExLexem.hpp>
#include <ProgramView.hpp>

struct ExLexer {
    ExLexer();
    ~ExLexer();

    bool loadFile(const char* filename);
    bool isLoaded() const;
    bool unload();

    bool lex(ExLexem* res);

private:
    // File handles
    void* mptr;
    int len;
    int fd;
    // State variables for lexing
    LineView currLine;
    ProgramView progRemaining;
    int lineCounter;
};
