#pragma once

#include <ExLexem.hpp>

struct ContinuationStorage {
    friend struct Continuation;

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

    void getRealPosition(int& lineId, int& colId) {
        assert(colId >= 0);
        if (lineId < 0 || lineId > colBreaksOffset.size()) {
            assert(false);
            return;
        }

        int lo = colBreaksOffset[lineId];
        int hi;
        if (lineId == colBreaksOffset.size() - 1) {
            hi = colBreaksTable.size() - 1;
        } else {
            hi = colBreaksTable[lineId + 1] - 1;
        }

        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            if (colId < colBreaksTable[mid]) {
                hi = mid - 1;
            } else {
                lo = mid;
            }
        }
        if (colId >= colBreaksTable[lo]) {
            int breaksOffset = lo - colBreaksOffset[lineId];
            lineId = lineMap[lineId] + breaksOffset;
            colId = colId - colBreaksTable[lo];
        } else if (colId >= colBreaksTable[hi]) {
            int breaksOffset = hi - colBreaksOffset[lineId];
            lineId = lineMap[lineId] + breaksOffset;
            colId = colId - colBreaksTable[hi];
        }
        assert(false);
    }

private:
    std::vector<int> colBreaksTable;
    std::vector<int> colBreaksOffset;

    std::vector<int> lineMap;

    char* buf;
    int len;
    int maxlen;
};

/// Only one instance at a time! Otherwise, instances will output to the same storage location.
struct Continuation {
    Continuation(ContinuationStorage& contStorage) : contStorage(contStorage) {
        beginPtr = contStorage.buf + contStorage.len;
        writePtr = beginPtr;
        availableStorage = contStorage.maxlen - contStorage.len;
        lineContinuations = 0;
    }

    void add(StringView lineNoBackslash) {
        // Remove trailing newline
        if (lineNoBackslash.endsWith('\n')) {
            --lineNoBackslash.end;
        }
        int n = lineNoBackslash.length();
        assert(n <= availableStorage);
        if (n <= availableStorage) {
            memcpy(writePtr, lineNoBackslash.begin, n);

            contStorage.colBreaksTable.push_back(writePtr-beginPtr);
            ++lineContinuations;

            writePtr += n;
            availableStorage -= n;
        }
    }

    StringView flush(int lineNumber) {
        assert(lineContinuations >= 1);
        assert(availableStorage >= 1);

        *writePtr = '\n';
        ++writePtr;

        int numWritten = writePtr - beginPtr;
        contStorage.len += numWritten;
        assert(contStorage.len <= contStorage.maxlen);
        StringView res(beginPtr, writePtr);

        contStorage.lineMap.push_back(lineNumber);
        int columnsOffset = contStorage.colBreaksTable.size() - lineContinuations;
        contStorage.colBreaksOffset.push_back(columnsOffset);

        writePtr = nullptr;
        beginPtr = nullptr;
        availableStorage = 0;
        return res;
    }

private:
    ContinuationStorage& contStorage;
    char* writePtr;
    char* beginPtr;
    int availableStorage;
    int lineContinuations;
};

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
    bool unload();

    bool lex(ExLexem* res);

private:
    // File handles
    void* mptr;
    int len;
    int fd;

    // Input program allowing to access current and next lines.
    Program program;
    // Additional storage required for line continuations
    ContinuationStorage contStorage;
};
