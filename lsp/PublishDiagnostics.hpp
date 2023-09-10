#pragma once

#include <Vector.hpp>
#include "BufferWriter.hpp"

struct PublishDiagnosticsParams {

    void write(BufferWriter& wr) const {
        BufferWriter::Object o = wr.beginObject();
        o.writeMember("uri", uri);
        o.writeMember("diagnostics", diagnostics);
    }

    /**
     * The URI for which diagnostic information is reported.
     */
    FStr uri;

    /**
     * An array of diagnostic information items.
     */
    Vector<Diagnostic> diagnostics;
};
