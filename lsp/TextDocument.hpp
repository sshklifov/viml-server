#pragma once

#include "ValueReader.hpp"
#include "Range.hpp"

struct DidOpenTextDocumentParams {

    void read(ValueReader& rd) {
        rd.readMember("textDocument", textDocument);
    }

    /**
     * The document that was opened.
     */
    struct TextDocumentItem {

        void read(ValueReader& rd) {
            rd.readMember("uri", uri);
            rd.readMember("languageId", languageId);
            rd.readMember("version", version);
            rd.readMember("text", text);
        }

        /**
         * The text document's URI.
         */
        FStr uri;

        /**
         * The text document's language identifier.
         */
        FStr languageId;

        /**
         * The version number of this document (it will increase after each
         * change, including undo/redo).
         */
        int version;

        /**
         * The content of the opened text document.
         */
        FStr text;

    } textDocument;
};

struct DidChangeTextDocumentParams {

    void read(ValueReader& rd) {
        rd.readMember("textDocument", textDocument);
        rd.readMember("contentChanges", contentChanges);
    }

    /**
     * The document that did change. The version number points
     * to the version after all provided content changes have
     * been applied.
     */
    struct VersionedTextDocumentIdentifier {

        void read(ValueReader& rd) {
            rd.readMember("uri", uri);
            rd.readMember("version", version);
        }

        /**
         * The text document's URI.
         */
        FStr uri;

        /**
         * The version number of this document.
         *
         * The version number of a document will increase after each change,
         * including undo/redo. The number doesn't need to be consecutive.
         */
        int version;

    } textDocument;

    /**
     * The actual content changes. The content changes describe single state
     * changes to the document. So if there are two content changes c1 (at
     * array index 0) and c2 (at array index 1) for a document in state S then
     * c1 moves the document from S to S' and c2 from S' to S''. So c1 is
     * computed on the state S and c2 is computed on the state S'.
     *
     * To mirror the content of a document using change events use the following
     * approach:
     * - start with the same initial content
     * - apply the 'textDocument/didChange' notifications in the order you
     *   receive them.
     * - apply the `TextDocumentContentChangeEvent`s in a single notification
     *   in the order you receive them.
     */
    struct TextDocumentContentChangeEvent {

        void read(ValueReader& rd) {
            rd.readMember("range", range);
            rd.readMember("text", text);
        }

        /**
         * The range of the document that changed.
         */
        std::optional<Range> range;

        /**
         * The new text for the provided range.
         */
        FStr text;
    };
    Vector<TextDocumentContentChangeEvent> contentChanges;
};

struct DidCloseParams {

    void read(ValueReader& rd) {
        rd.readMember("textDocument", textDocument);
    }

    /**
     * The document that was closed.
     */
    FStr textDocument;
};
