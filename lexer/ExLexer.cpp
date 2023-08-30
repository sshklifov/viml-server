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
#include "ExDictionary.hpp"

static int buildExLexem(StringView line, LocationMap::Key locationKey, ExLexem& lex) {
    YY_BUFFER_STATE buf = cmd_scan_bytes(line.begin, line.length());

    lex.exDictIdx = -1;
    lex.name.begin = lex.name.end;
    lex.qargs.begin = lex.qargs.end;
    lex.locationKey = locationKey;
    lex.nameOffset = 0;
    lex.qargsOffset = 0;
    lex.bang = 0;
    lex.range = 0;

    const ExDictionary& dict = ExDictionary::getSingleton();

    enum {ERROR = 256, RANGE_ARG, RANGE_DELIM, COMMAND_COLON, NAME, QARGS};
    int lineOffset = 0;
    int done = false;
    do {
        int tok = cmdlex();
        switch (tok) {
            case '\0':
                done = true;
                break;

            case ERROR:
                // TODO error
                break;

            case RANGE_ARG:
            case RANGE_DELIM:
                lex.range = 1;
                break;

            case NAME:
                int cmdNameLen = 0;
                int dictIdx = dict.partialSearch(cmdget_text(), cmdNameLen);
                if (dictIdx < 0) {
                    // TODO error
                    break;
                } else {
                    // TODO special commands (check with bang)
                    StringView namePart(line.begin + lineOffset, cmdNameLen);
                    StringView restPart(namePart.end, namePart.begin + cmdget_leng());
                    if (!restPart.empty()) {
                        if (isalpha(restPart.left())) {
                            // TODO error
                            break;
                        } else if (restPart.left() == '!') {
                            ++restPart.begin;
                            lex.bang = true;
                        }
                    }
                    lex.exDictIdx = dictIdx;
                    lex.name = namePart;
                    lex.qargs = restPart.trimLeftSpace();
                    lex.nameOffset = namePart.begin - line.begin;
                    lex.qargsOffset = restPart.begin - line.begin;
                }
                break;
        }
        lineOffset += cmdget_leng();
    }
    while (!done);

    cmd_delete_buffer(buf);
    return 0; // TODO
}

ExLexer::ExLexer() {
    mptr = nullptr;
    len = 0;
    fd = -1;
}

ExLexer::~ExLexer() {
    unloadFile();
}

bool ExLexer::loadFile(const char* filename) {
    if (isLoaded()) {
        return false;
    }

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

bool ExLexer::unloadFile() {
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

int ExLexer::lex(ExLexem& res) {
    assert(isLoaded());

    if (program.empty()) {
        return EOF;
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

    return buildExLexem(programLine, locationKey, res);
}

bool ExLexer::resolveLoc(const ExLexem& lexem, int off, int& line, int& col) const {
    bool ok = locationMap.resolve(lexem.locationKey, off, line, col);
    // Convert to 1-based indexing
    if (ok) {
        ++line;
        ++col;
    }
    return ok;
}

bool ExLexer::resolveNameLoc(const ExLexem& lexem, int& line, int& col) const {
    return resolveLoc(lexem, lexem.nameOffset, line, col);
}

bool ExLexer::resolveNameEndLoc(const ExLexem& lexem, int& line, int& col) const {
    int offset = lexem.nameOffset;
    if (lexem.name.length() > 1) {
        offset += lexem.name.length() - 1;
    }
    return resolveLoc(lexem, offset, line, col);
}

bool ExLexer::resolveQargsLoc(const ExLexem& lexem, int& line, int& col) const {
    return resolveLoc(lexem, lexem.qargsOffset, line, col);
}

bool ExLexer::resolveQargsEndLoc(const ExLexem& lexem, int& line, int& col) const {
    int offset = lexem.qargsOffset;
    if (lexem.qargs.length() > 1) {
        offset += lexem.qargs.length() - 1;
    }
    return resolveLoc(lexem, offset, line, col);
}

// TODO carriage returns
