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
    program = ProgramView(buffer, buffer + len);
    currLine = program.popLine();

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
    int currLineNumber = program.poppedLines;
    LineView nextLine = program.popLine();
    LineView origNextLine = nextLine;
    const char* contOrigBegin = contBuf + contBufLen;

    nextLine.popSpaces();
    if (!nextLine.empty() && nextLine.front() == '\\') {
        if (!contBuf) {
            contBuf = new char[len + 1];
            contBufLen = 0;
        }
        strncpy(contBuf + contBufLen, currLine.begin, currLine.length());
        contBufLen += currLine.length();
    }
    while (!nextLine.empty() && nextLine.front() == '\\') {
        nextLine.begin += 1;
        // Might be a comment, LOL syntax.
        if (nextLine[0] != '"' || nextLine[1] != ' ') {
            strncpy(contBuf + contBufLen, nextLine.begin, nextLine.length());
            contBufLen += nextLine.length();
        }
        nextLine = program.popLine();
        origNextLine = nextLine;
        nextLine.popSpaces();
    }
    currLine = origNextLine;

    LineView contLine(contOrigBegin, contBuf + contBufLen);
    LineView resultLine = contLine.empty() ? origNextLine : contLine;
    resultLine.dropSpaces();
    if (resultLine.empty()) {
        return false;
    } else if (resultLine.front() == '"' || resultLine.front() != '\n') {
        res->name = LineView();
        res->qargs = LineView();
        res->line = currLineNumber;
        res->nameColumn = 1;
        res->qargsColumn = 1;
        return true;
    } else {
        const char* columnBegin = resultLine.begin;
        // TODO more formatting needed
        res->name = resultLine.popWord();
        resultLine.dropSpaces();
        res->qargs = resultLine;
        // Remove newline
        if (!res->qargs.empty()) {
            --res->qargs.end;
        }
        res->line = currLineNumber;
        res->nameColumn = (res->name.begin - columnBegin + 1);
        res->qargsColumn = (res->qargs.begin - columnBegin + 1);
        return true;
    }
}
