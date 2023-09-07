#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <argp.h>

#include "Initialize.hpp"
#include "Diagnostics.hpp"
#include "TextDocument.hpp"
#include "Window.hpp"
#include <SyntaxTree.hpp>
#include <Eval.hpp>

struct RequestId {
    enum Type {INT, STR};

    RequestId(const rapidjson::Value& v) { read(v); }
    RequestId(int id) : type(INT), intValue(id) {}
    RequestId(std::string s) : type(STR), strValue(std::move(s)) {}

    void read(const rapidjson::Value& v) {
        if (v.IsString()) {
            type = STR;
            strValue = v.GetString();
        } else {
            type = INT;
            intValue = v.GetInt();
        }
    }

    Type type;
    int intValue;
    std::string strValue;
};

template<>
inline void BufferWriter::setKey(const RequestId& reqId) {
    if (reqId.type == RequestId::INT) {
        w.Int(reqId.intValue);
    } else {
        w.String(reqId.strValue.c_str());
    }
}

enum ErrorCode {
    // Defined by common sense.
    NoError = 0,

    // Defined by JSON RPC
    ParseError = -32700,
    InvalidRequest = -32600,
    MethodNotFound = -32601,
    InvalidParams = -32602,
    InternalError = -32603,
    serverErrorStart = -32099,
    serverErrorEnd = -32000,
    ServerNotInitialized = -32002,
    UnknownErrorCode = -32001,
    // Defined by the protocol.
    RequestCancelled = -32800,
};

struct ResponseError {
    ResponseError() : code(NoError) {}

    ResponseError(ErrorCode code, const char* suffix = nullptr) : code(code) {
        switch (code) {
        case ParseError:
            message = "Parse error";
            break;
        case InvalidRequest:
            message = "Invalid request";
            break;
        case MethodNotFound:
            message = "Method not found";
            break;
        case InvalidParams:
            message = "Invalid parameters";
            break;
        case InternalError:
            message = "Internal error";
            break;
        case ServerNotInitialized:
            message = "Server not initialized";
            break;
        case UnknownErrorCode:
            message = "Unknown error code";
            break;
        case RequestCancelled:
            message = "Request cancelled";
            break;
        default:
            // No message
            return;
        }
        if (suffix && *suffix) {
            message += ": ";
            message += suffix;
        }
    }

    operator bool() {
        return code != NoError;
    }

    ErrorCode code;
    std::string message;
};

template<>
inline void BufferWriter::setKey(const ResponseError& error) {
    ObjectScope scope = beginObject();
    add("code", (int)error.code);
    add("message", error.message);
}

struct InitOptions {
    std::string dupInput;
    std::string dupOutput;
};

struct EchoingServer {
    EchoingServer(const InitOptions& opt) {
        dup[0] = fopen(opt.dupInput.c_str(), "w");
        dup[1] = fopen(opt.dupOutput.c_str(), "w");
    }

    ~EchoingServer() {
        if (dup[0]) {
            fclose(dup[0]);
        }
        if (dup[1]) {
            fclose(dup[1]);
        }
    }

protected:
    FILE* dup[2];
};

struct RespondingServer : public EchoingServer {
    RespondingServer(const InitOptions& opt) : EchoingServer(opt) {}

    void respond(const rapidjson::StringBuffer& output) {
        int bufferSize = output.GetSize();
        printf("Content-Length: %d\r\n\r\n", bufferSize);
        fputs(output.GetString(), stdout);
        fflush(stdout);

        if (dup[1]) {
            fprintf(dup[1], "Writing response: %s\n", output.GetString());
            fflush(dup[1]);
        }
    }

    template <typename T>
    void respondResult(const RequestId& id, const T& res) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        {
            BufferWriter::ObjectScope root = w.beginObject();
            w.add("jsonrpc", "2.0");
            w.add("id", id);
            w.add("result", res);
        }

        respond(output);
    }

    template <typename T>
    void pushNotification(const char* method, const T& resp) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        {
            BufferWriter::ObjectScope root = w.beginObject();
            w.add("jsonrpc", "2.0");
            w.add("method", method);
            w.add("params", resp);
        }

        respond(output);
    }

    void pushShowMessage(std::string msg, ShowMessageParams::MessageType type = ShowMessageParams::Error) {
        ShowMessageParams params;
        params.type = type;
        params.message = std::move(msg);
        pushNotification("window/showMessage", params);
    }

    void respondError(const RequestId& id, const ResponseError& error) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        {
            BufferWriter::ObjectScope root = w.beginObject();
            w.add("jsonrpc", "2.0");
            w.add("id", id);
            w.add("error", error);
        }

        respond(output);
    }
};

struct ReceivingServer : public RespondingServer {
    using MethodHandler = void(ReceivingServer::*)(const RequestId&, const rapidjson::Value&);
    using NotifHandler = void(ReceivingServer::*)(const rapidjson::Value&);
    using MethodMap = std::unordered_map<std::string, MethodHandler>;
    using NotifMap = std::unordered_map<std::string, NotifHandler>;

    ReceivingServer(const InitOptions& init) : RespondingServer(init) {}

    void stdinFunction() {
        const char* expectedMsg = "Content-Length: ";
        int charsMatched = 0;
        while (true) {
            int c = getchar();
            if (c == EOF) {
                break;
            } else if (c == '\r' || c == '\n') {
                charsMatched = 0;
            } else if (c == expectedMsg[charsMatched]) {
                ++charsMatched;
            } else {
                while (c != '\n' && c != EOF) {
                    c = getchar();
                }
                charsMatched = 0;
            }

            if (expectedMsg[charsMatched] == '\0') {
                charsMatched = 0;
                // Read message length
                int len = 0;
                int s = scanf("%d", &len);
                if (s == 1) {
                    char* message = new char[len + 1];
                    // Skip to message
                    do {
                        c = getchar();
                    } while (c == '\n' || c == '\r');
                    message[0] = c;
                    // Read message
                    for (int i = 1; i < len; ++i) {
                        if (c == EOF) {
                            break;
                        }
                        c = getchar();
                        message[i] = c;
                    }
                    if (c != EOF) {
                        message[len] = '\0';
                        // Parse as json
                        rapidjson::Document document;
                        document.Parse(message, len);
                        assert(!document.HasParseError());
                        // Log message
                        if (dup[0]) {
                            fprintf(dup[0], "Content-Length: %d\n\n", len + 1);
                            fputs(message, dup[0]);
                            fputc('\n', dup[0]);
                            fflush(dup[0]);
                        }
                        // Process
                        receiveDocument(document);
                        delete[] message;
                    }
                }
            }
        }
    }

    // TODO string format library... WTF c++

    void receiveDocument(rapidjson::Document& document) {
        ResponseError error = receiveDocumentWithError(document);
        if (error) {
            if (document.HasMember("id")) {
                RequestId requestId(document["id"]);
                respondError(requestId, error);
            } else {
                std::string message = std::to_string(error.code);
                message += ": ";
                message += error.message;
                pushShowMessage(std::move(message));
            }
        }
    }

    ResponseError receiveDocumentWithError(rapidjson::Document& document) {
        if (!document.HasMember("jsonrpc")) {
            return ResponseError(InvalidParams, "Missing jsonrpc");
        }
        const char* jsonrpc = document["jsonrpc"].GetString();
        if (strcmp(jsonrpc, "2.0") != 0) {
            return ResponseError(InvalidParams, "Bad jsonrpc version");
        }

        if (!document.HasMember("method")) {
            return ResponseError(InvalidParams, "Missing method");
        }
        const char* methodStr = document["method"].GetString();
        ResponseError error = receiveMethod(methodStr);
        if (error) {
            return error;
        }

        bool notification = !document.HasMember("id");
        if (notification) {
            NotifMap::iterator it = notifs.find(methodStr);
            if (it == notifs.end()) {
                return ResponseError(MethodNotFound, methodStr);
            }
            NotifHandler handler = it->second;
            if (document.HasMember("params")) {
                (this->*handler)(document["params"]);
            } else {
                (this->*handler)(rapidjson::Value());
            }
            return NoError;
        } else {
            RequestId requestId(document["id"]);
            MethodMap::iterator it = methods.find(methodStr);
            if (it == methods.end()) {
                return ResponseError(MethodNotFound, methodStr);
            }
            MethodHandler handler = it->second;
            if (document.HasMember("params")) {
                (this->*handler)(requestId, document["params"]);
            } else {
                (this->*handler)(requestId, rapidjson::Value());
            }
            return NoError;
        }
    }

    virtual ResponseError receiveMethod(const char* methodStr) { return NoError; }

protected:
    MethodMap methods;
    NotifMap notifs;
};

struct Server : public ReceivingServer {
    Server(const InitOptions& opt) : ReceivingServer(opt) {
        didInitialize = false;

        methods["initialize"] = static_cast<MethodHandler>(&Server::initialize);
        notifs["initialized"] = static_cast<NotifHandler>(&Server::initialized);
        notifs["textDocument/didOpen"] = static_cast<NotifHandler>(&Server::didOpen);
        notifs["textDocument/didChange"] = static_cast<NotifHandler>(&Server::didChange);
    }

    ResponseError receiveMethod(const char* methodStr) override {
        if (didInitialize || strcmp(methodStr, "initialize") == 0) {
            return NoError;
        }
        return ServerNotInitialized;
    }

    void initialized(const rapidjson::Value& obj) {}

    void initialize(const RequestId& id, const rapidjson::Value& obj) {
        didInitialize = true;
        respondResult(id, InitializeResult());
    }

    void didOpen(const rapidjson::Value& obj) {
#if 0
        ValueReader reader(std::move(obj));
        DidOpenTextDocumentParams didOpenParams;
        reader.read(didOpenParams);

        const char* source = didOpenParams.textDocument.text;
        std::vector<Diagnostic> errors;
        ast.reloadFromString(source, errors);

        PublishDiagnosticsParams diagnosticsParams;
        diagnosticsParams.uri = didOpenParams.textDocument.uri;
        diagnosticsParams.diagnostics = std::move(errors);
        pushNotification("textDocument/publishDiagnostics", diagnosticsParams);
#endif
    }

    void didChange(const rapidjson::Value& obj) {
#if 0
        ValueReader reader(std::move(obj));
        DidChangeTextDocumentParams didChangeParams;
        reader.read(didChangeParams);

        const char* source = didChangeParams.contentChanges[0].text;
        std::vector<Diagnostic> errors;
        ast.reloadFromString(source, errors);

        PublishDiagnosticsParams diagnosticsParams;
        diagnosticsParams.uri = didChangeParams.textDocument.uri;
        diagnosticsParams.diagnostics = std::move(errors);
        pushNotification("textDocument/publishDiagnostics", diagnosticsParams);
#endif
    }

private:
    bool didInitialize;
    SyntaxTree ast;
};

error_t argsParser(int key, char *arg, argp_state *state) {
    InitOptions& init = *reinterpret_cast<InitOptions*>(state->input);
    int ret = 0;
    switch (key) {
    case 'i':
        init.dupInput = arg;
        break;

    case 'o':
        init.dupOutput = arg;
        break;

    default:
        ret = ARGP_ERR_UNKNOWN;
    }
    return ret;
}

int otherMain(int argc, char** argv) {
    const char* argsDoc = ""; // < No args
    const char* doc = "Vim LSP";

    struct argp_option options[] = {
        {"stdin",  'i', "FILE", 0, "Redirect input LSP messages"},
        {"stdout", 'o', "FILE", 0, "Redirect output LSP messages"},
        {0}
    };

    struct argp argp = {options, argsParser, argsDoc, doc};

    InitOptions init;
    int errnum = argp_parse(&argp, argc, argv, 0, NULL, &init);
    if (errnum) {
        return errnum;
    }

    Server server(init);
    server.stdinFunction();

    return 0;
}

int main(int argc, char** argv) {
    /* abort(); */
    otherMain(argc, argv);
    return 0;
}

int test() {
    SyntaxTree ast;
    std::vector<Diagnostic> digs;
    ast.reloadFromFile(TEST_FILE, digs);
    /* Block* letBlock = root->body[0]; */
    /* EvalFactory factory; */
    /* EvalCommand* result = parse(letBlock->lexem, factory); */
    return 0;
}
