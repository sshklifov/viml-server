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

static bool splitCmdAndArgs(StringView line, StringView& name, StringView& args) {
    line = line.trimLeftSpace();
    const char* it = line.begin;
    while (it < line.end) {
        if (*it != ' ' && *it != '\t' && *it != '\n' && *it != '\0') {
            ++it;
        } else {
            name = StringView(line.begin, it);
            args = StringView(it, line.end).popLineFeed().trimSpace();
            return true;
        }
    }
    return false;
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
    conts.init(len);

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

    int lineNumber = program.topNumber();

    StringView resultLine;
    int ignore = false;
    do {
        StringView line = program.next().trimLeftSpace();
        if (line.beginsWith('\\')) {
            conts.startNew();
            conts.append(program.top());
            program.pop();

            int continuation = true;
            do {
                int codeCont = line.beginsWith('\\');
                int commentCont = line.beginsWith("\"\\ ");
                if (codeCont || commentCont) {
                    if (codeCont) {
                        conts.append(line.popLeft());
                    }
                    program.pop();
                    line = program.top().trimLeftSpace();
                } else {
                    continuation = false;
                }
            }
            while (continuation);
            resultLine = conts.finish();
        } else {
            resultLine = program.top();
            program.pop();
        }

        line = resultLine.trimLeftSpace();
        if (line.empty()) {
           return false;
        }
        ignore = (line.left() == '"' || line.left() == '\n');
    } while (ignore);

    const char* columnBegin = resultLine.begin;

    splitCmdAndArgs(resultLine, res->name, res->qargs);
    res->line = lineNumber;
    res->nameColumn = (res->name.begin - columnBegin + 1);
    res->qargsColumn = (res->qargs.begin - columnBegin + 1);
    return true;
}
