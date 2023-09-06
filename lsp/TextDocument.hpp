#pragma once

#include "ValueReader.hpp"
#include "Range.hpp"

struct DidOpenTextDocumentParams {
	/**
	 * The document that was opened.
	 */
	struct TextDocumentItem {
		/**
		 * The text document's URI.
		 */
		const char* uri;

		/**
		 * The text document's language identifier.
		 */
		const char* languageId;

		/**
		 * The version number of this document (it will increase after each
		 * change, including undo/redo).
		 */
		int version;

		/**
		 * The content of the opened text document.
		 */
		const char* text;

	} textDocument;
};

struct DidChangeTextDocumentParams {
	/**
	 * The document that did change. The version number points
	 * to the version after all provided content changes have
	 * been applied.
	 */
	struct VersionedTextDocumentIdentifier {
		/**
		 * The text document's URI.
		 */
		const char* uri;

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
	 *	 receive them.
	 * - apply the `TextDocumentContentChangeEvent`s in a single notification
	 *	 in the order you receive them.
	 */
	struct TextDocumentContentChangeEvent {
		/**
		 * The range of the document that changed.
		 */
		std::optional<Range> range;

		/**
		 * The new text for the provided range.
		 */
		const char* text;
	};
	std::vector<TextDocumentContentChangeEvent> contentChanges;
};

template<>
inline void ValueReader::read(DidOpenTextDocumentParams& params) {
	MemberScope paramsScope = beginMember("params");
	MemberScope documentScope = beginMember("textDocument");
	readMember("uri", params.textDocument.uri);
	readMember("languageId", params.textDocument.languageId);
	readMember("version", params.textDocument.version);
	readMember("text", params.textDocument.text);
}

template<>
inline void ValueReader::read(DidChangeTextDocumentParams::VersionedTextDocumentIdentifier& textDocument) {
	readMember("uri", textDocument.uri);
	readMember("version", textDocument.version);
}

template<>
inline void ValueReader::read(DidChangeTextDocumentParams::TextDocumentContentChangeEvent& contentChanges) {
	readMember("range", contentChanges.range);
	readMember("text", contentChanges.text);
}

template<>
inline void ValueReader::read(DidChangeTextDocumentParams& params) {
	MemberScope paramsScope = beginMember("params");
	readMember("textDocument", params.textDocument);
	readMember("contentChanges", params.contentChanges);
}
