#include <ExLexer.hpp>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cstddef>
#include <cstdio>
#include <cassert>
#include <cctype>

#include <string>
#include <stdexcept>

// TODO remove these when there is a proper lexer...
static void popSpaces(StringView& line) {
    line = line.trim(" \t");
}

static StringView popLine(StringView& program) {
    const char* oldBegin = program.begin;
    program = program.trimUntil("\n");
    program.begin++; // Remove the new line
    return StringView(oldBegin, program.begin);
}

static StringView popWord(StringView& line) {
    popSpaces(line);
    const char* oldBegin = line.begin;
    line = line.trimUntil(" \t");
    return StringView(oldBegin, line.begin);
}

ExLexer::ExLexer() {
    mptr = nullptr;
    len = 0;
    fd = -1;
    contBuf = nullptr;
    contBufLen = 0;
}

ExLexer::~ExLexer() {
    unload();
}

bool ExLexer::loadFile(const char* filename) {
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        return false;
    }

    struct stat statbuf;
    int s = fstat(fd, &statbuf);
    if (s != 0) {
        close(fd);
        return false;
    }

    size_t len =  statbuf.st_size;
    void* mptr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (!mptr) {
        close(fd);
        return false;
    }

    const char* buffer = reinterpret_cast<const char*>(mptr);

    // Success
    this->mptr = mptr;
    this->len = len;
    this->fd = fd;
    program = StringView(buffer, buffer + len);
    currLine = popLine(program);

    return true;
}

bool ExLexer::unload() {
    if (isLoaded()) {
        munmap(mptr, len);
        mptr = nullptr;
        close(fd);
        fd = -1;
        delete[] contBuf;

        return true;
    } else {
        return false;
    }
}

bool ExLexer::isLoaded() const {
    return mptr != nullptr;
}

bool ExLexer::lex(ExLexem* res) {
    if (!isLoaded()) {
        assert(false);
        return false;
    }
    int currLineNumber = 0; // TODO program.poppedLines;
    StringView nextLine = popLine(program);
    StringView origNextLine = nextLine;
    const int origContBufLen = contBufLen;

    popSpaces(nextLine);
    if (!nextLine.empty() && nextLine.front() == '\\') {
        if (!contBuf) {
            contBuf = new char[len + 1];
            contBufLen = 0;
        }
        strncpy(contBuf + contBufLen, currLine.begin, currLine.length() - 1); // Remove leading '\n'
        contBufLen += currLine.length() - 1;
    }
    while (!nextLine.empty() && nextLine.front() == '\\') {
        nextLine.begin += 1;
        // Might be a comment, LOL syntax.
        if (nextLine[0] != '"' || nextLine[1] != ' ') {
            strncpy(contBuf + contBufLen, nextLine.begin, nextLine.length() - 1); // Remove leading '\n'
            contBufLen += nextLine.length() - 1;
        }
        nextLine = popLine(program);
        origNextLine = nextLine;
        popSpaces(nextLine);
    }
    currLine = origNextLine;

    StringView contBufLine(contBuf + origContBufLen, contBuf + contBufLen);
    StringView resultLine = contBufLine.empty() ? origNextLine : contBufLine;
    popSpaces(resultLine);
    if (resultLine.empty() || resultLine.front() == '\0') {
        return false;
    } else if (resultLine.front() == '"' || resultLine.front() == '\n') {
        res->name = StringView();
        res->qargs = StringView();
        res->line = currLineNumber;
        res->nameColumn = 1;
        res->qargsColumn = 1;
        return true;
    } else {
        const char* columnBegin = resultLine.begin;
        // TODO more formatting needed
        res->name = popWord(resultLine);
        res->qargs = resultLine;
        // Remove newline
        if (!res->qargs.empty() && res->qargs.back() == '\n') {
            --res->qargs.end;
        }
        res->line = currLineNumber;
        res->nameColumn = (res->name.begin - columnBegin + 1);
        res->qargsColumn = (res->qargs.begin - columnBegin + 1);
        return true;
    }
}
