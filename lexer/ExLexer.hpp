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
    Program() = default;

    Program(const StringView& p) {
        set(p);
    }

    void set(const StringView& p) {
        program = p;
        line.end = program.begin; // Kinda hacky whoops
        lineCounter = 0;
        pop();
    }
    
    void pop() {
        line.begin = line.end;
        line.end = program.find(line.begin, '\n');
        if (line.end < program.end) {
            ++line.end; // Include the new line
        }

        // Handle weird case where end of file would be returned as a separate line.
        if (line.beginsWith('\0')) {
            line.begin = line.end;
        }
        // Increment line counter if not past end of program.
        if (!line.empty()) {
            ++lineCounter;
        }
    }

    const StringView& top() { return line; }

    int lineNumber() const { return lineCounter; }

private:
    StringView line;
    StringView program;
    int lineCounter;
};

struct TwoLineProgram {
    TwoLineProgram() = default;

    void set(const StringView& p) {
        oneLineProg.set(p);
        savedLine = oneLineProg.top();
        oneLineProg.pop();
    }
    
    void pop() {
        savedLine = oneLineProg.top();
        oneLineProg.pop();
    }

    const StringView& top() {
        return savedLine;
    }

    const StringView& next() {
        return oneLineProg.top();
    }

    int topNumber() const { return nextNumber() - 1; }

    int nextNumber() const { return oneLineProg.lineNumber(); }

private:
    StringView savedLine;
    Program oneLineProg;
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

    // Input program allowing to access current and next lines.
    TwoLineProgram program;
    // Additional storage required for line continuations
    LineConts conts;
};
