#pragma once

#include "ValueReader.hpp"
#include <string>

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

template<>
inline void ValueReader::read(DidOpenTextDocumentParams& params) {
    MemberScope paramsScope = beginMember("params");
    MemberScope documentScope = beginMember("textDocument");
    readMember("uri", params.textDocument.uri);
    readMember("languageId", params.textDocument.languageId);
    readMember("version", params.textDocument.version);
    readMember("text", params.textDocument.text);
}
