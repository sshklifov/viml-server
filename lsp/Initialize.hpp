#pragma once

#include "BufferWriter.hpp"

struct ServerCapabilities {

    void write(BufferWriter& wr) const {
        BufferWriter::ScopedObject scope = wr.beginScopedObject();
        wr.writeMember("positionEncoding", positionEncoding);
        wr.writeMember("textDocumentSync", textDocumentSync);
        wr.writeMember("completionProvider", completionProvider);
        wr.writeMember("hoverProvider", hoverProvider);
        wr.writeMember("signatureHelpProvider", signatureHelpProvider);
        wr.writeMember("declarationProvider", declarationProvider);
        wr.writeMember("definitionProvider", definitionProvider);
        wr.writeMember("typeDefinitionProvider", typeDefinitionProvider);
        wr.writeMember("implementationProvider", implementationProvider);
        wr.writeMember("referencesProvider", referencesProvider);
        wr.writeMember("documentHighlightProvider", documentHighlightProvider);
        wr.writeMember("documentSymbolProvider", documentSymbolProvider);
        wr.writeMember("codeActionProvider", codeActionProvider);
        wr.writeMember("codeLensProvider", codeLensProvider);
        wr.writeMember("documentLinkProvider", documentLinkProvider);
        wr.writeMember("colorProvider", colorProvider);
        wr.writeMember("documentFormattingProvider", documentFormattingProvider);
        wr.writeMember("documentRangeFormattingProvider", documentRangeFormattingProvider);
        wr.writeMember("documentOnTypeFormattingProvider", documentOnTypeFormattingProvider);
        wr.writeMember("renameProvider", renameProvider);
        wr.writeMember("foldingRangeProvider", foldingRangeProvider);
        wr.writeMember("executeCommandProvider", executeCommandProvider);
        wr.writeMember("selectionRangeProvider", selectionRangeProvider);
        wr.writeMember("linkedEditingRangeProvider", linkedEditingRangeProvider);
        wr.writeMember("callHierarchyProvider", callHierarchyProvider);
        wr.writeMember("semanticTokensProvider", semanticTokensProvider);
        wr.writeMember("monikerProvider", monikerProvider);
        wr.writeMember("typeHierarchyProvider", typeHierarchyProvider);
        wr.writeMember("inlineValueProvider", inlineValueProvider);
        wr.writeMember("inlayHintProvider", inlayHintProvider);
        wr.writeMember("diagnosticProvider", diagnosticProvider);
        wr.writeMember("workspaceSymbolProvider", workspaceSymbolProvider);
        wr.writeMember("workspace", workspace);
    }

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
        
        void write(BufferWriter& wr) const {
            BufferWriter::ScopedObject scope = wr.beginScopedObject();
            wr.writeMember("openClose", openClose);
            wr.writeMember("change", change);
        }

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
        void write(BufferWriter& wr) const {
            BufferWriter::ScopedObject scope = wr.beginScopedObject();
            wr.writeMember("workDoneProgress", workDoneProgress);
        }
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
        void write(BufferWriter& wr) const {
            BufferWriter::ScopedObject scope = wr.beginScopedObject();
            wr.writeMember("workDoneProgress", workDoneProgress);
        }
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
        void write(BufferWriter& wr) const {
            BufferWriter::ScopedObject scope = wr.beginScopedObject();
            wr.writeMember("workDoneProgress", workDoneProgress);
        }
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

        void write(BufferWriter& wr) const {
            BufferWriter::ScopedObject scope = wr.beginScopedObject();
            wr.writeMember("workspaceFolders", workspaceFolders);
        }

        /**
         * The server supports workspace folder.
         *
         * @since 3.6.0
         */
        struct WorkspaceFoldersServerCapabilities {
            void write(BufferWriter& wr) const {
                BufferWriter::ScopedObject scope = wr.beginScopedObject();
                wr.writeMember("supported", supported);
            }
            bool supported = false;
        } workspaceFolders;

    } workspace;
};

struct InitializeResult {

    void write(BufferWriter& wr) const {
        BufferWriter::ScopedObject scope = wr.beginScopedObject();
        wr.writeMember("capabilities", capabilities);
        wr.writeMember("serverInfo", serverInfo);
    }

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

        void write(BufferWriter& wr) const {
            BufferWriter::ScopedObject scope = wr.beginScopedObject();
            wr.writeMember("name", name);
            wr.writeMember("version", version);
        }

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
