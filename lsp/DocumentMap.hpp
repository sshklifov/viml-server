#pragma once

#include <TextDocument.hpp>
#include <SyntaxTree.hpp>

#include <unordered_map>

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

    WorkingDocument* open(const DidOpenTextDocumentParams& params) {
        std::pair<Iterator, bool> res = docs.emplace(params.textDocument.uri.str(), params.textDocument);
        if (res.second) {
            docs.erase(res.first);
            // Try again after erasing conficting entry
            res = docs.emplace(params.textDocument.uri.str(), params.textDocument);
        }
        return &res.first->second;
    }

    WorkingDocument* change(const DidChangeTextDocumentParams& params) {
        Iterator it = docs.find(params.textDocument.uri.str());
        if (it != docs.end()) {
            it->second.change(params);
            return &it->second;
        } else {
            return nullptr;
        }
    }

    bool close(const DidCloseParams& params) {
        int present = docs.erase(params.textDocument.str());
        return present;
    }

private:
    std::unordered_map<const char*, WorkingDocument> docs;
};
