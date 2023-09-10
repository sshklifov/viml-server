#pragma once

#include <Range.hpp>
#include <FStr.hpp>

struct Diagnostic {
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

    };

    Diagnostic() = default;
    Diagnostic(FStr msg, const Range& range, DiagnosticSeverity type = Error) :
        range(range), severity(type), message(std::move(msg)) {}

    /**
     * The range at which the message applies.
     */
    Range range;

    /**
     * The diagnostic's severity. Can be omitted. If omitted it is up to the
     * client to interpret diagnostics as error, warning, info or hint.
     */
    DiagnosticSeverity severity;


    /**
     * The diagnostic's message
     */
    FStr message;
};
