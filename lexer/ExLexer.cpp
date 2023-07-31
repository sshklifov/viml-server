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
    progRemaining = ProgramView(buffer, buffer + len);
    currLine = progRemaining.popLine();
    lineCounter = 0;

    return true;
}

bool ExLexer::unload() {
    if (isLoaded()) {
        munmap(mptr, len);
        mptr = nullptr;
        close(fd);
        fd = -1;
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

    while (!currLine.empty()) {
        ++lineCounter;
        // Sanity check
        LineView futureLine = progRemaining.popLine();
        if (!futureLine.empty()) {
            LineView workLine = futureLine;
            workLine.popSpaces();
            if (*workLine.begin == '\\') {
                throw std::runtime_error("Do not support continuations yet :/");
            }
        }

        LineView workingLine = currLine;
        currLine = futureLine;
        workingLine.popSpaces();
        if (*workingLine.begin != '"' && *workingLine.begin != '\n') {
            const char* lineBegin = workingLine.begin;
            res->name = workingLine.popWord();
            if (!res->name.empty() && res->name.front() == ':') {
                ++res->name.begin;
            }
            if (!res->name.empty() && res->name.back() == '!') {
                --res->name.end;
                res->bang = true;
            }

            workingLine.popSpaces();
            res->qargs = workingLine;
            if (!res->qargs.empty()) {
                // Remove newline
                --res->qargs.end;
            }
            res->line = lineCounter;
            res->nameCol = (res->name.begin - lineBegin);
            res->qargsCol = (res->qargs.begin - lineBegin);
            return true;
        }
    }
    return false;
}
