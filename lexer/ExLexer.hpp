#pragma once

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

    CmdlineCreator(CmdlineStorage& storage) : storage(storage) {
        beginPtr = storage.buf + storage.len;
        writePtr = beginPtr;
    }

    void concat(const char* lineNoBackslash, int line, int col) {
        int len = 0;
        const char* p = lineNoBackslash;
        while (*p && *p != '\n') {
            *writePtr++ = *p++;
            ++len;
        }
        assert(len > 0);
        fragments.emplace(line, col, len);
    }

    const char* finish(Vector<CmdlineResolver::Fragment>& res) {
        *writePtr = '\n';
        ++writePtr;

        int numWritten = writePtr - beginPtr;
        storage.len += numWritten;
        assert(storage.len <= storage.maxlen);
        const char* cmdline = beginPtr;

        res = std::move(fragments);
        writePtr = nullptr;
        beginPtr = nullptr;
        return cmdline;
    }

private:
    CmdlineStorage& storage;
    char* beginPtr;
    char* writePtr;

    Vector<CmdlineResolver::Fragment> fragments;
};

/// Internal helper for parsing program line by line
struct Program {
    Program() {
        set(nullptr);
    }

    void set(const char* s) {
        program = s;
        lineCounter = 0;
    }

    bool empty() const { return *program == '\0'; }

    const char* top() const { return program; }

    int lineNumber() const { return lineCounter; }

    void pop() {
        assert(!empty());
        do {
            while (*program != 0 && *program != '\n') {
                ++program;
            }
            ++lineCounter;
            ++program;
        } while (*program == '\n');
    }

private:
    const char* program;
    int lineCounter;
};

struct ExLexer {
    ExLexer() = default;
    ExLexer(const ExLexer&) = delete;
    ExLexer(ExLexer&&) = delete;

    bool reload(const char* str);
    bool lexNext(ExLexem& res, DiagnosticReporter& rep);

private:
    // Input program allowing to access current and next lines.
    Program program;
    // Additional storage required for line continuations
    CmdlineStorage contStorage;
};
