#pragma once

struct Position {
    Position() = default;
    Position(int line, int col) : line(line), character(col) {}

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
    /**
     * The range's start position.
     */
    Position start;

    /**
     * The range's end position.
     */
    Position end;
};