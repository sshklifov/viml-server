#pragma once

#include "BufferWriter.hpp"

struct ShowMessageParams {

    void write(BufferWriter& wr) const {
        BufferWriter::Object o = wr.beginObject();
        o.writeMember("message", message);
        o.writeMember("type", type);
    }

    /**
     * The message type. See {@link MessageType}.
     */
    enum MessageType {
        /**
         * An error message.
         */
        Error = 1,
        /**
         * A warning message.
         */
        Warning = 2,
        /**
         * An information message.
         */
        Info = 3,
        /**
         * A log message.
         */
        Log = 4

    } type;

    /**
     * The actual message.
     */
    FStr message;
};

struct LogMessageParams {

    void write(BufferWriter& wr) const {
        BufferWriter::Object o = wr.beginObject();
        o.writeMember("message", message);
        o.writeMember("type", type);
    }

    /**
     * The message type. See {@link MessageType}.
     */
    enum MessageType {
        /**
         * An error message.
         */
        Error = 1,
        /**
         * A warning message.
         */
        Warning = 2,
        /**
         * An information message.
         */
        Info = 3,
        /**
         * A log message.
         */
        Log = 4

    } type;

    /**
     * The actual message.
     */
    FStr message;
};
