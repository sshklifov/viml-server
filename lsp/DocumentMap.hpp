#pragma once

#include <TextDocument.hpp>
#include <SyntaxTree.hpp>
#include <StringMap.hpp>

struct WorkingDocument {
    WorkingDocument(const DidOpenTextDocumentParams::TextDocumentItem& textDoc) {
        version = textDoc.version;
        uri = textDoc.uri;
        content = textDoc.text;
        ast.reload(content.str(), diagnostics);
    }

    void change(const DidChangeTextDocumentParams& params) {
        using Event = DidChangeTextDocumentParams::TextDocumentContentChangeEvent;
        for (const Event& event : params.contentChanges) {
            if (event.range.has_value()) {
                assert(false && "TODO");
            } else {
                content = event.text;
            }
        }
        diagnostics.clear();
        ast.reload(content.str(), diagnostics);
        version = params.textDocument.version;
        assert(uri == params.textDocument.uri);
    }

    SyntaxTree ast;
    std::vector<Diagnostic> diagnostics;

    int version;
    FStr uri;
    FStr content;
};

struct DocumentMap {
    using Iterator = std::unordered_map<const char*, WorkingDocument>::iterator;

    WorkingDocument& operator[](int i) { return docs[i]; }

    int open(const DidOpenTextDocumentParams& params) {
        const char* key = params.textDocument.uri.str();
        int pos = docs.find(key);
        if (pos < 0) {
            return docs.emplace(key, params.textDocument);
        } else {
            return -1;
        }
    }

    int change(const DidChangeTextDocumentParams& params) {
        int pos = docs.find(params.textDocument.uri);
        if (pos >= 0) {
            docs[pos].change(params);
            return pos;
        } else {
            return -1;
        }
    }

    bool close(const DidCloseParams& params) {
        int present = docs.erase(params.textDocument.str());
        return present;
    }

private:
    StringMap<WorkingDocument> docs;
};
