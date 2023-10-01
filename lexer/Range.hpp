#pragma once

/// Maximal (invalid) line number
enum { MAXLNUM = 0x7fffffff, };
/// Maximal column number
/// MAXCOL used to be INT_MAX, but with 64 bit ints that results in running
/// out of memory when trying to allocate a very long line.
enum { MAXCOL = 0x7fffffff, };

struct Position {
    Position(int line = MAXLNUM, int col = MAXCOL) : line(line), character(col) {}

    bool operator<(const Position& other) const {
        if (line < other.line) {
            return true;
        }
        return line == other.line && character < other.character;
    }

    /**
     * Line position in a document (zero-based).
     */
    int line;

    /**
     * Character offset on a line in a document (zero-based). The meaning of this
     * offset is determined by the negotiated `PositionEncodingKind`.
     *
     * If the character value is greater than the line length it defaults back
     * to the line length.
     */
    int character;
};

struct Range {
    Range() = default;
    Range(const Position& pos) : start(pos), end(pos) {}
    Range(const Position& start, const Position& end) : start(start), end(end) {}

    void join(const Range& other) {
        if (other.start < start) {
            start = other.start;
        }
        if (end < other.end) {
            end = other.end;
        }
    }

    bool has(const Position& pos) const {
        return !(pos < start) && pos < end;
    }

    int locate(const Position& pos) const {
        if (pos < start) {
            return -1;
        } else if (!(pos < end)) {
            return 1;
        } else {
            return 0;
        }
    }

    /**
     * The range's start position.
     */
    Position start;

    /**
     * The range's end position.
     */
    Position end;
};
