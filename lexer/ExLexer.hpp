#pragma once

#include "LocationMap.hpp"
#include "ExLexem.hpp"
#include "DiagnosticReporter.hpp"

/// Internal storage class for ExLexer
struct CmdlineStorage {
    friend struct CmdlineCreator;

    CmdlineStorage(const CmdlineStorage&) = delete;
    CmdlineStorage(CmdlineStorage&&) = delete;

    CmdlineStorage() {
        maxlen = 0;
        buf = nullptr;
        len = 0;
    }

    ~CmdlineStorage() {
        delete[] buf;
    }

    void realloc(int capacity) {
        capacity += 1; //< Additional line feed at the end (if missing).
        if (maxlen < capacity) {
            maxlen = capacity;
            delete[] buf;
            buf = new char[capacity];
        }
        len = 0;
    }

    void deinit() {
        delete[] buf;
        buf = nullptr;
        maxlen = 0;
        len = 0;
    }

private:
    char* buf;
    int len;
    int maxlen;
};

/// Only one instance at a time! Otherwise, instances will output to the same storage location.
struct CmdlineCreator {
    CmdlineCreator(const CmdlineCreator&) = delete;
    CmdlineCreator(CmdlineCreator&&) = delete;

    CmdlineCreator(CmdlineStorage& storage, Locator& loc) : storage(storage), loc(loc) {
        beginPtr = storage.buf + storage.len;
        writePtr = beginPtr;
        availableStorage = storage.maxlen - storage.len;
    }

    void concat(StringView lineNoBackslash, int line, int col) {
        // Remove trailing newline
        if (lineNoBackslash.endsWith('\n')) {
            --lineNoBackslash.end;
        }
        int n = lineNoBackslash.length();
        assert(n <= availableStorage);
        if (n <= availableStorage) {
            memcpy(writePtr, lineNoBackslash.begin, n);

            int fragLen = lineNoBackslash.length();
            loc.addFragment(line, col, fragLen);

            writePtr += n;
            availableStorage -= n;
        }
    }

    StringView finish() {
        assert(availableStorage >= 1);

        *writePtr = '\n';
        ++writePtr;

        int numWritten = writePtr - beginPtr;
        storage.len += numWritten;
        assert(storage.len <= storage.maxlen);
        StringView res(beginPtr, writePtr);

        writePtr = nullptr;
        beginPtr = nullptr;
        availableStorage = 0;
        return res;
    }

private:
    CmdlineStorage& storage;
    Locator& loc;

    char* beginPtr;
    char* writePtr;
    int availableStorage;
    int fragmentsBegin;
};

/// Internal helper for parsing program line by line
struct Program {
    void set(const char* s, int n) {
        program.begin = s;
        program.end = s + n;

        splitPos = findSplitPos();
        lineCounter = 0;
    }

    bool empty() const { return program.empty(); }

    StringView top() const { return StringView(program.begin, splitPos); }

    int lineNumber() const { return lineCounter; }

    void pop() {
        assert(!empty());
        program.begin = splitPos;
        splitPos = findSplitPos();
        ++lineCounter;
    }

private:
    const char* findSplitPos() {
        const char* res = program.find('\n');
        if (res < program.end) {
            ++res; // Include the new line
        }
        return res;
    }

    StringView program;
    const char* splitPos;
    int lineCounter;
};

struct ExLexer {
    ExLexer() = default;
    ExLexer(const ExLexer&) = delete;
    ExLexer(ExLexer&&) = delete;

    bool reload(const char* str);
    bool lex(DiagnosticReporter& rep, ExLexem& res);

private:
    // Input program allowing to access current and next lines.
    Program program;
    // Additional storage required for line continuations
    CmdlineStorage contStorage;
};
