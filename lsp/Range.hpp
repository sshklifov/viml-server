#pragma once

#include "ValueReader.hpp"
#include "BufferWriter.hpp"

struct Position {

    void read(ValueReader& rd) {
        rd.readMember("line", line);
        rd.readMember("character", character);
    }

    void write(BufferWriter& wr) const {
        BufferWriter::Object o = wr.beginObject();
        o.writeMember("line", line);
        o.writeMember("character", character);
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

    void read(ValueReader& rd) {
        rd.readMember("start", start);
        rd.readMember("end", end);
    }

    void write(BufferWriter& wr) const {
        BufferWriter::Object o = wr.beginObject();
        o.writeMember("start", start);
        o.writeMember("end", end);
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
