#pragma once

#include "BufferWriter.hpp"
#include "Range.hpp"

struct Diagnostic {
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

    std::string message;
};

struct PublishDiagnosticsParams {
	/**
	 * The URI for which diagnostic information is reported.
	 */
    const char* uri;

	/**
	 * An array of diagnostic information items.
	 */
    std::vector<Diagnostic> diagnostics;
};

template <>
inline void BufferWriter::setKey(const Diagnostic::DiagnosticSeverity& severity) {
    w.Int(severity);
}

template <>
inline void BufferWriter::setKey(const Diagnostic& dig) {
    ObjectScope scoped = beginObject();
    add("range", dig.range);
    add("range", dig.range);
    add("severity", dig.severity);
    add("message", dig.message.c_str());
}

template <>
inline void BufferWriter::setKey(const PublishDiagnosticsParams& params) {
    ObjectScope scoped = beginObject();
    add("uri", params.uri);
    add("diagnostics", params.diagnostics);
}
