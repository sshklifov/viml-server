#pragma once

#include "LocationMap.hpp"
#include "ExLexem.hpp"
#include "DiagnosticReporter.hpp"

/// Internal storage class for ExLexer
struct ContinuationStorage {
    ContinuationStorage() {
        maxlen = 0;
        buf = nullptr;
        len = 0;
    }

    ~ContinuationStorage() {
        delete[] buf;
    }

    void realloc(int capacity) {
        capacity += 1; //< Additional line feed in the end if missing.
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

    bool isInit() const { return buf; }

    char* buf;
    int len;
    int maxlen;
};

/// Only one instance at a time! Otherwise, instances will output to the same storage location.
struct Continuation {
    Continuation(ContinuationStorage& contStorage, LocationMap& locMap) : contStorage(contStorage), locMap(locMap) {
        beginPtr = contStorage.buf + contStorage.len;
        writePtr = beginPtr;
        availableStorage = contStorage.maxlen - contStorage.len;
        fragmentsBegin = locMap.fragments.count();
    }

    void add(StringView lineNoBackslash, int line, int col) {
        // Remove trailing newline
        if (lineNoBackslash.endsWith('\n')) {
            --lineNoBackslash.end;
        }
        int n = lineNoBackslash.length();
        assert(n <= availableStorage);
        if (n <= availableStorage) {
            memcpy(writePtr, lineNoBackslash.begin, n);

            int fragLen = lineNoBackslash.length();
            locMap.addFragment(line, col, fragLen);

            writePtr += n;
            availableStorage -= n;
        }
    }

    StringView finish(LocationMap::Key& key) {
        assert(availableStorage >= 1);

        *writePtr = '\n';
        ++writePtr;

        int numWritten = writePtr - beginPtr;
        contStorage.len += numWritten;
        assert(contStorage.len <= contStorage.maxlen);
        StringView res(beginPtr, writePtr);

        key.fragBegin = this->fragmentsBegin;
        key.fragEnd = locMap.fragments.count();

        writePtr = nullptr;
        beginPtr = nullptr;
        availableStorage = 0;
        return res;
    }

private:
    ContinuationStorage& contStorage;
    LocationMap& locMap;

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
        assert(!empty()); // TODO ok?
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

    const char* splitPos;
    StringView program;
    int lineCounter;
};

struct ExLexer {
    bool reload(const char* str, DiagnosticReporter* reporter);
    bool lex(ExLexem& res);
    const LocationMap& getLocationMap() const;

private:
    bool buildExLexem(StringView line, LocationMap::Key locationKey, ExLexem& lex);

    // Diagnostics are written here
    DiagnosticReporter* reporter;
    // Input program allowing to access current and next lines.
    Program program;
    // Additional storage required for line continuations
    ContinuationStorage contStorage;
    // Mapper correcting line/column numbers from continuation lines
    LocationMap locationMap;
};
