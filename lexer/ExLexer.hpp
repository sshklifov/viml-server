#pragma once

#include "LocationMap.hpp"
#include "ExLexem.hpp"

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

    void init(int capacity) {
        maxlen = capacity + 1; // Additional line feed in the end if missing.
        delete[] buf;
        buf = new char[capacity];
        len = 0;
    }

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
        locationBegin = locMap.locations.size();
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

            int entryLen = lineNoBackslash.length();
            locMap.locations.push_back(LocationMap::Location(line, col, entryLen));

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

        key.entryBegin = this->locationBegin;
        key.entryEnd = locMap.locations.size();

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
    int locationBegin;
};

/// Internal helper for parsing program line by line
struct Program {
    Program() = default;

    Program(const StringView& p) {
        set(p);
    }

    void set(const StringView& p) {
        program = p;

        // Kinda hachy whoops
        line.begin = NULL;
        line.end = program.begin;
        pop();

        lineCounter = 0;
    }

    bool empty() const { return line.empty(); }
    
    void pop() {
        if (empty()) {
            return;
        }

        line.begin = line.end;
        line.end = program.find(line.begin, '\n');
        if (line.end < program.end) {
            ++line.end; // Include the new line
        }

        // Discard EOF
        if (line.beginsWith('\0')) {
            line.begin = line.end;
            return;
        }
        ++lineCounter;
    }

    const StringView& top() const { return line; }

    int lineNumber() const { return lineCounter; }

private:
    StringView line;
    StringView program;
    int lineCounter;
};

struct ExLexer {
    ExLexer();
    ~ExLexer();

    bool loadFile(const char* filename);
    bool isLoaded() const;
    bool unloadFile();

    int lex(ExLexem& res);

    bool getLoc(const ExLexem& lexem, int off, int& line, int& col) const;
    bool getNameLoc(const ExLexem& lexem, int& line, int& col) const;
    bool getNameEndLoc(const ExLexem& lexem, int& line, int& col) const;
    bool getQargsLoc(const ExLexem& lexem, int& line, int& col) const;
    bool getQargsEndLoc(const ExLexem& lexem, int& line, int& col) const;

    const LocationMap& getLocationMap() const;

private:
    // File handles
    void* mptr;
    int len;
    int fd;

    // Input program allowing to access current and next lines.
    Program program;
    // Additional storage required for line continuations
    ContinuationStorage contStorage;
    // Mapper correcting line/column numbers from continuation lines
    LocationMap locationMap;
};
