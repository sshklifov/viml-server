#pragma once

#include <FStr.hpp>

#include "BufferWriter.hpp"
#include "Range.hpp"

struct Diagnostic {

    void write(BufferWriter& wr) const {
        BufferWriter::ObjectScope scoped = wr.beginObject();
        wr.writeMember("range", range);
        wr.writeMember("severity", severity);
        wr.writeMember("message", message);
    }

    /**
     * The range at which the message applies.
     */
    Range range;

    /**
     * The diagnostic's severity. Can be omitted. If omitted it is up to the
     * client to interpret diagnostics as error, warning, info or hint.
     */
    enum DiagnosticSeverity {
        /**
         * Reports an error.
         */
        Error = 1,

        /**
         * Reports a warning.
         */
        Warning = 2,

        /**
         * Reports an information.
         */
        Information = 3,

        /**
         * Reports a hint.
         */
        Hint = 4

    } severity;

    FStr message;
};

struct PublishDiagnosticsParams {

    void write(BufferWriter& wr) const {
        BufferWriter::ObjectScope scoped = wr.beginObject();
        wr.writeMember("uri", uri);
        wr.writeMember("diagnostics", diagnostics);
    }

    /**
     * The URI for which diagnostic information is reported.
     */
    FStr uri;

    /**
     * An array of diagnostic information items.
     */
    std::vector<Diagnostic> diagnostics;
};
