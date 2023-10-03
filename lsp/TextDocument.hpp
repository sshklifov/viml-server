#pragma once

#include "ValueReader.hpp"
#include "BufferWriter.hpp"

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


struct TextDocumentIdentifier {
    void read(ValueReader& rd) {
        rd.readMember("uri", uri);
    }

    /**
     * The text document's URI.
     */
    FStr uri;

};

struct DidCloseParams {
    void read(ValueReader& rd) {
        rd.readMember("textDocument", textDocument);
    }

    /**
     * The document that was closed.
     */
    TextDocumentIdentifier textDocument;
};

struct ReferenceParams {
    void read(ValueReader& rd) {
        rd.readMember("textDocument", textDocument);
        rd.readMember("position", position);
    }

    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The position inside the text document.
     */
    Position position;
};

struct DocumentSymbolParams {
    void read(ValueReader& rd) {
        rd.readMember("textDocument", textDocument);
    }

    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;
};

enum SymbolKind {
    File = 1,
    Module = 2,
    Namespace = 3,
    Package = 4,
    Class = 5,
    Method = 6,
    Property = 7,
    Field = 8,
    Constructor = 9,
    Enum = 10,
    Interface = 11,
    Function = 12,
    Variable = 13,
    Constant = 14,
    String = 15,
    Number = 16,
    Boolean = 17,
    Array = 18,
    Object = 19,
    Key = 20,
    Null = 21,
    EnumMember = 22,
    Struct = 23,
    Event = 24,
    Operator = 25,
    TypeParameter = 26
};

/**
 * Represents programming constructs like variables, classes, interfaces etc.
 * that appear in a document. Document symbols can be hierarchical and they
 * have two ranges: one that encloses its definition and one that points to its
 * most interesting range, e.g. the range of an identifier.
 */
struct DocumentSymbol {
    void write(BufferWriter& wr) const {
        BufferWriter::Object o = wr.beginObject();
        o.writeMember("name", name);
        o.writeMember("detail", detail);
        o.writeMember("kind", kind);
        o.writeMember("range", range);
        o.writeMember("selectionRange", range);
    }

    /**
     * The name of this symbol. Will be displayed in the user interface and
     * therefore must not be an empty string or a string only consisting of
     * white spaces.
     */
    FStr name;

    /**
     * More detail for this symbol, e.g the signature of a function.
     */
    std::optional<FStr> detail;

    /**
     * The kind of this symbol.
     */
    SymbolKind kind;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else like comments. This information is typically used to
     * determine if the clients cursor is inside the symbol to reveal in the
     * symbol in the UI.
     */
    Range range;

    /**
     * Children of this symbol, e.g. properties of a class.
     */
    Vector<DocumentSymbol> children;
};
