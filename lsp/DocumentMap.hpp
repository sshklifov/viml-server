#pragma once

#include <TextDocument.hpp>
#include <unordered_map>

struct WorkingDocument {
    WorkingDocument(const DidOpenTextDocumentParams::TextDocumentItem& textDoc) {
        version = textDoc.version;
        uri = textDoc.uri;
        content = textDoc.text;
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
        version = params.textDocument.version;
        assert(uri == params.textDocument.uri);
    }

private:
    int version;
    FStr uri;
    FStr content;
};

struct DocumentMap {

    bool open(const DidOpenTextDocumentParams& params) {
        using Iterator = std::unordered_map<const char*, WorkingDocument>::iterator;
        std::pair<Iterator, bool> res = docs.emplace(params.textDocument.uri.str(), params.textDocument);
        return res.second;
    }

    bool change(const DidChangeTextDocumentParams& params) {
        using Iterator = std::unordered_map<const char*, WorkingDocument>::iterator;
        Iterator it = docs.find(params.textDocument.uri.str());
        if (it != docs.end()) {
            it->second.change(params);
            return true;
        } else {
            return false;
        }
    }

    bool close(const DidCloseParams& params) {
        int present = docs.erase(params.textDocument.str());
        return present;
    }

private:
    std::unordered_map<const char*, WorkingDocument> docs;
};
