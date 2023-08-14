#pragma once

#include <ExLexem.hpp>

struct LineConts {
    LineConts() {
        maxLen = 0;
        buf = nullptr;
        len = 0;
        curr = nullptr;
    }

    ~LineConts() {
        delete[] buf;
    }

    void init(int capacity) {
        maxLen = capacity;
        // Lazily create buffer, very likely to be never used
    }

    void startNew() {
        assert(maxLen > 0 && "Missing prior call to init");
        // Force allocation of buffer
        if (!buf) {
            buf = new char[maxLen + 1];
        }
        curr = buf;
    }

    StringView finish() {
        assert(curr && "Missing prior call to startNew");

        buf[len] = '\n';
        ++len;
        assert(len <= maxLen);

        StringView res(curr, buf + len);
        curr = nullptr;
        return res;
    }

    void append(StringView line) {
        assert(curr && "Missing prior call to startNew");

        // Remove trailing newline
        if (line.endsWith('\n')) {
            --line.end;
        }
        int n = line.length();
        memcpy(buf + len, line.begin, n);
        len += n;
    }

private:
    char* buf;
    int len;
    int maxLen;

    char* curr;
};

struct Program {
    void init(const StringView& p) {
        program = p;
        currLine = popHelper();
        nextLine = popHelper();
        lineCounter = 1;
    }
    
    StringView pop() {
        currLine = nextLine;
        nextLine = popHelper();
        if (!currLine.empty()) {
            ++lineCounter;
        }
        return currLine;
    }

    const StringView& top() {
        return currLine;
    }

    const StringView& next() {
        return nextLine;
    }

    int lineNumber() const { return lineCounter; }

private:
    // Actually removes a line from program. Unlike pop method, which is more complicated because
    // of the lookahead.
    StringView popHelper() {
        StringView line(program.begin, program.find('\n'));
        if (line.end < program.end) {
            ++line.end; // Include the new line
        }
        program.begin = line.end;
        // Handle weird case where end of file would be returned as a separate line.
        if (line.beginsWith('\0')) {
            return StringView();
        }
        return line;
    }

    StringView currLine;
    StringView nextLine;
    StringView program;
    int lineCounter;
};

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

    // Program (StringView of file buffer)
    Program program;
    // Additional storage required for line continuations
    LineConts conts;
};
