#pragma once

#include "BufferWriter.hpp"

struct ShowMessageParams {
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
    std::string message;
};

template <>
void BufferWriter::setKey(const ShowMessageParams& showMessage) {
    ObjectScope s = beginObject();
    add("message", showMessage.message);
    add("type", (int)showMessage.type);
}
