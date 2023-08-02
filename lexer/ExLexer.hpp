#pragma once

#include <ExLexem.hpp>

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
    StringView currLine;
    StringView program;
    // Additional storage required for line continuations
    char* contBuf;
    int contBufLen;
};
