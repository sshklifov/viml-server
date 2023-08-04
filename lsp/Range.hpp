#pragma once

#include "Writer.hpp"

struct Position {
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

template<>
void Writer::setKey(const Position& pos) {
    ObjectScope s = newObject();
    add("line", pos.line);
    add("character", pos.character);
}

template<>
void Writer::setKey(const Range& range) {
    ObjectScope s = newObject();
    add("start", range.start);
    add("end", range.end);
}
