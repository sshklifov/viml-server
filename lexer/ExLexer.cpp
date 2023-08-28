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

#include "Command.hpp"

static int buildExLexem(StringView line, LocationMap::Key locationKey, ExLexem& lex) {
    YY_BUFFER_STATE buf = cmd_scan_bytes(line.begin, line.length());

    lex.name.clear();
    lex.qargs.clear();

    lex.locationKey = locationKey;
    lex.nameOffset = 0;
    lex.qargsOffset = 0;
    lex.bang = 0;
    lex.range = 0;

    enum {ERROR = 256, RANGE_ARG, RANGE_DELIM, COMMAND_COLON, NAME, QARGS};
    int numMatched = 0;
    int done = false;
    int unexpectedSymbol = 0;
    do {
        int tok = cmdlex();
        switch (tok) {
            case '\0':
                done = true;
                break;

            case ERROR:
                // Set first time only
                if (!unexpectedSymbol) {
                    unexpectedSymbol = tok;
                }
                break;

            case RANGE_ARG:
            case RANGE_DELIM:
                lex.range = 1;
                break;

            case NAME:
                lex.name = cmdget_text();
                if (!lex.name.empty() && lex.name.back() == '!') {
                    lex.bang = 1;
                    lex.name.resize(lex.name.size() - 1);
                }
                lex.nameOffset = numMatched;
                break;

            case QARGS:
                lex.qargs = cmdget_text();
                lex.qargsOffset = numMatched;
                break;
        }
        numMatched += cmdget_leng();
    }
    while (!done);

    cmd_delete_buffer(buf);
    return unexpectedSymbol;
}

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
    
    program.set(StringView(buffer, buffer + len));
    contStorage.init(len);

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
    assert(isLoaded());

    if (program.empty()) {
        return false;
    }

    LocationMap::Key locationKey;
    StringView programLine;

    int ignore = false;
    do {
        Continuation cont(contStorage, locationMap);
        cont.add(program.top(), program.lineNumber(), 0);
        program.pop();

        while (true) {
            const char* colBegin = program.top().begin;
            StringView workLine = program.top().trimLeftSpace();
            int codeCont = workLine.beginsWith('\\');
            int commentCont = workLine.beginsWith("\"\\ ");
            if (!codeCont && !commentCont) {
                break;
            }
            if (codeCont) {
                workLine = workLine.popLeft(); // Remove continuation character
                int colOffset = workLine.begin - colBegin;
                cont.add(workLine, program.lineNumber(), colOffset);
            }
            program.pop();
        }

        programLine = cont.finish(locationKey);
        // Check if line is a comment or empty
        StringView workLine = programLine.trimLeftSpace();
        // Must include a line feed character (at least)
        assert(workLine.length() >= 1);
        ignore = (workLine.left() == '"' || workLine.left() == '\n');
    } while (ignore);

    int errors = buildExLexem(programLine, locationKey, *res);
    if (res->name.empty()) {
        return false;
    }
    return true;
}

// TODO carriage returns
