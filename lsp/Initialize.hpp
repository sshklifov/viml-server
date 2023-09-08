#pragma once

#include "BufferWriter.hpp"

struct ServerCapabilities {
	/**
	 * The position encoding the server picked from the encodings offered
	 * by the client via the client capability `general.positionEncodings`.
	 *
	 * If the client didn't provide any position encodings the only valid
	 * value that a server can return is 'utf-16'.
	 *
	 * If omitted it defaults to 'utf-16'.
	 *
	 * @since 3.17.0
	 */
	const char* positionEncoding = "utf-8";

	struct TextDocumentSyncOptions {
		/**
		 * Open and close notifications are sent to the server. If omitted open
		 * close notifications should not be sent.
		 */
		bool openClose = true;

		/**
		 * Defines how the host (editor) should sync document changes to the language
		 * server.
		 */
		enum TextDocumentSyncKind {
			/**
			 * Documents should not be synced at all.
			 */
			None,

			/**
			 * Documents are synced by always sending the full content
			 * of the document.
			 */
			Full,

			/**
			 * Documents are synced by sending the full content on open.
			 * After that only incremental updates to the document are
			 * sent.
			 */
			Incremental

		} change = Full;

	} textDocumentSync;
	
	/**
	 * Completion options.
	 */
	struct CompletionOptions {
		bool workDoneProgress = false;
	} completionProvider;

	/**
	 * The server provides hover support.
	 */
	bool hoverProvider = false;

	/**
	 * The server provides signature help support.
	 */
	struct SignatureHelpOptions {
		bool workDoneProgress = false;
	} signatureHelpProvider;

	/**
	 * The server provides go to declaration support.
	 *
	 * @since 3.14.0
	 */
	bool declarationProvider = false;

	/**
	 * The server provides goto definition support.
	 */
	bool definitionProvider = false;

	/**
	 * The server provides goto type definition support.
	 *
	 * @since 3.6.0
	 */
	bool typeDefinitionProvider = false;

	/**
	 * The server provides goto implementation support.
	 *
	 * @since 3.6.0
	 */
	bool implementationProvider = false;

	/**
	 * The server provides find references support.
	 */
	bool referencesProvider = false;

	/**
	 * The server provides document highlight support.
	 */
	bool documentHighlightProvider = false;

	/**
	 * The server provides document symbol support.
	 */
	bool documentSymbolProvider = false;

	/**
	 * The server provides code actions. The `CodeActionOptions` return type is
	 * only valid if the client signals code action literal support via the
	 * property `textDocument.codeAction.codeActionLiteralSupport`.
	 */
	bool codeActionProvider = false;

	/**
	 * The server provides code lens.
	 */
	bool codeLensProvider = false;

	/**
	 * The server provides document link support.
	 */
	bool documentLinkProvider = false;

	/**
	 * The server provides color provider support.
	 *
	 * @since 3.6.0
	 */
	bool colorProvider = false;

	/**
	 * The server provides document formatting.
	 */
	bool documentFormattingProvider = false;

	/**
	 * The server provides document range formatting.
	 */
	bool documentRangeFormattingProvider = false;

	/**
	 * The server provides document formatting on typing.
	 */
    bool documentOnTypeFormattingProvider = false;

	/**
	 * The server provides rename support. RenameOptions may only be
	 * specified if the client states that it supports
	 * `prepareSupport` in its initial `initialize` request.
	 */
    bool renameProvider = false;

	/**
	 * The server provides folding provider support.
	 *
	 * @since 3.10.0
	 */
	bool foldingRangeProvider = false;

	/**
	 * The server provides execute command support.
	 */
	bool executeCommandProvider = false;

	/**
	 * The server provides selection range support.
	 *
	 * @since 3.15.0
	 */
	bool selectionRangeProvider = false;

	/**
	 * The server provides linked editing range support.
	 *
	 * @since 3.16.0
	 */
	bool linkedEditingRangeProvider = false;

	/**
	 * The server provides call hierarchy support.
	 *
	 * @since 3.16.0
	 */
	bool callHierarchyProvider = false;

	/**
	 * The server provides semantic tokens support.
	 *
	 * @since 3.16.0
	 */
	bool semanticTokensProvider = false;

	/**
	 * Whether server provides moniker support.
	 *
	 * @since 3.16.0
	 */
	bool monikerProvider = false;

	/**
	 * The server provides type hierarchy support.
	 *
	 * @since 3.17.0
	 */
	bool typeHierarchyProvider = false;

	/**
	 * The server provides inline values.
	 *
	 * @since 3.17.0
	 */
	bool inlineValueProvider = false;

	/**
	 * The server provides inlay hints.
	 *
	 * @since 3.17.0
	 */
	bool inlayHintProvider = false;

	/**
	 * The server has support for pull model diagnostics.
	 *
	 * @since 3.17.0
	 */
	struct DiagnosticOptions {
		bool workDoneProgress = false;
	} diagnosticProvider;

	/**
	 * The server provides workspace symbol support.
	 */
	bool workspaceSymbolProvider = false;

	/**
	 * Workspace specific server capabilities
	 */
	struct WorkspaceAnon {
		/**
		 * The server supports workspace folder.
		 *
		 * @since 3.6.0
		 */
		struct WorkspaceFoldersServerCapabilities {
			bool supported = false;
		} workspaceFolders;

	} workspace;
};

struct InitializeResult {
	/**
	 * The capabilities the language server provides.
	 */
    ServerCapabilities capabilities; 

	/**
	 * Information about the server.
	 *
	 * @since 3.15.0
	 */
    struct ServerInfoAnon {
		/**
		 * The name of the server as defined by the server.
		 */
        const char* name = "viml-server";

		/**
		 * The server's version as defined by the server.
		 */
        const char* version = "0.1";

    } serverInfo;
};

template <>
inline void BufferWriter::setKey(const ServerCapabilities::TextDocumentSyncOptions::TextDocumentSyncKind& kind) {
    w.Int(kind);
}

template <>
inline void BufferWriter::setKey(const ServerCapabilities::TextDocumentSyncOptions& opts) {
    ObjectScope scope = beginObject();
    add("openClose", opts.openClose);
    add("change", opts.change);
}

template <>
inline void BufferWriter::setKey(const ServerCapabilities::CompletionOptions& opts) {
    ObjectScope scope = beginObject();
    add("workDoneProgress", opts.workDoneProgress);
}

template <>
inline void BufferWriter::setKey(const ServerCapabilities::SignatureHelpOptions& opts) {
    ObjectScope scope = beginObject();
    add("workDoneProgress", opts.workDoneProgress);
}

template <>
inline void BufferWriter::setKey(const ServerCapabilities::DiagnosticOptions& opts) {
    ObjectScope scope = beginObject();
    add("workDoneProgress", opts.workDoneProgress);
}

template <>
inline void BufferWriter::setKey(const ServerCapabilities::WorkspaceAnon& ws) {
    ObjectScope scope = beginObject();

    {
        ObjectScope scope = beginObject("workspaceFolders");
        add("supported", ws.workspaceFolders.supported);
    }
}

template <>
inline void BufferWriter::setKey(const ServerCapabilities& cap) {
    ObjectScope scope = beginObject();
    add("positionEncoding", cap.positionEncoding);
    add("textDocumentSync", cap.textDocumentSync);
    add("completionProvider", cap.completionProvider);
    add("hoverProvider", cap.hoverProvider);
    add("signatureHelpProvider", cap.signatureHelpProvider);
    add("declarationProvider", cap.declarationProvider);
    add("definitionProvider", cap.definitionProvider);
    add("typeDefinitionProvider", cap.typeDefinitionProvider);
    add("implementationProvider", cap.implementationProvider);
    add("referencesProvider", cap.referencesProvider);
    add("documentHighlightProvider", cap.documentHighlightProvider);
    add("documentSymbolProvider", cap.documentSymbolProvider);
    add("codeActionProvider", cap.codeActionProvider);
    add("codeLensProvider", cap.codeLensProvider);
    add("documentLinkProvider", cap.documentLinkProvider);
    add("colorProvider", cap.colorProvider);
    add("documentFormattingProvider", cap.documentFormattingProvider);
    add("documentRangeFormattingProvider", cap.documentRangeFormattingProvider);
    add("documentOnTypeFormattingProvider", cap.documentOnTypeFormattingProvider);
    add("renameProvider", cap.renameProvider);
    add("foldingRangeProvider", cap.foldingRangeProvider);
    add("executeCommandProvider", cap.executeCommandProvider);
    add("selectionRangeProvider", cap.selectionRangeProvider);
    add("linkedEditingRangeProvider", cap.linkedEditingRangeProvider);
    add("callHierarchyProvider", cap.callHierarchyProvider);
    add("semanticTokensProvider", cap.semanticTokensProvider);
    add("monikerProvider", cap.monikerProvider);
    add("typeHierarchyProvider", cap.typeHierarchyProvider);
    add("inlineValueProvider", cap.inlineValueProvider);
    add("inlayHintProvider", cap.inlayHintProvider);
    add("diagnosticProvider", cap.diagnosticProvider);
    add("workspaceSymbolProvider", cap.workspaceSymbolProvider);
    add("workspace", cap.workspace);
}

template<>
inline void BufferWriter::setKey(const InitializeResult& res) {
    ObjectScope scope = beginObject();
    add("capabilities", res.capabilities);
    
    {
        ObjectScope scope = beginObject("serverInfo");
        add("name", res.serverInfo.name);
        add("version", res.serverInfo.version);

    }
}
