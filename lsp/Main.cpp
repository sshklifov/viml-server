#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>

#include <functional>
#include <memory>
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
#include <FStr.hpp>

#include "DocumentMap.hpp"

struct RequestId {
    enum Type {UNDEF, INT, STR};

    RequestId() : type(UNDEF) {}
    RequestId(int id) : type(INT), intValue(id) {}
    RequestId(FStr s) : type(STR), strValue(std::move(s)) {}

    void write(BufferWriter& wr) const {
        assert(type != UNDEF);
        if (type == RequestId::INT) {
            wr.writeMember("type", intValue);
        } else if (type == RequestId::STR) {
            wr.writeMember("type", strValue);
        }
    }

    void read(const ValueReader& rd) {
        read(rd.getValue());
    }

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
    FStr strValue;
};

enum class ErrorCode {
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
    ResponseError() : code(ErrorCode::NoError) {}

    ResponseError(ErrorCode code, const char* suffix = nullptr) : code(code) {
        const char* strCode = "";
        switch (code) {
        case ErrorCode::ParseError:
            strCode = "Parse error";
            break;
        case ErrorCode::InvalidRequest:
            strCode = "Invalid request";
            break;
        case ErrorCode::MethodNotFound:
            strCode = "Method not found";
            break;
        case ErrorCode::InvalidParams:
            strCode = "Invalid parameters";
            break;
        case ErrorCode::InternalError:
            strCode = "Internal error";
            break;
        case ErrorCode::ServerNotInitialized:
            strCode = "Server not initialized";
            break;
        case ErrorCode::UnknownErrorCode:
            strCode = "Unknown error code";
            break;
        case ErrorCode::RequestCancelled:
            strCode = "Request cancelled";
            break;
        default:
            // No message
            return;
        }
        if (suffix) {
            message.appendf("{}: {}", strCode, suffix);
        } else {
            message.append(strCode);
        }
    }

    
    void write(BufferWriter& wr) const {
        BufferWriter::ScopedObject scope = wr.beginScopedObject();
        wr.writeMember("code", code);
        wr.writeMember("message", message);
    }

    operator bool() {
        return code != ErrorCode::NoError;
    }

    ErrorCode code;
    FStr message;
};

struct InitOptions {
    FStr dupInput;
    FStr dupOutput;
};

struct EchoingServer {
    EchoingServer(const InitOptions& opt) {
        dup[0] = fopen(opt.dupInput.str(), "w");
        dup[1] = fopen(opt.dupOutput.str(), "w");
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

    void respond(const RequestId& id) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        {
            BufferWriter::ScopedObject root = w.beginScopedObject();
            w.writeMember("jsonrpc", "2.0");
            w.writeMember("id", id);
        }

        respondStr(output);
    }

    template <typename T>
    void respondResult(const RequestId& id, const T& res) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        {
            BufferWriter::ScopedObject root = w.beginScopedObject();
            w.writeMember("jsonrpc", "2.0");
            w.writeMember("id", id);
            w.writeMember("result", res);
        }

        respondStr(output);
    }

    template <typename T>
    void pushNotification(const char* method, const T& resp) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        {
            BufferWriter::ScopedObject root = w.beginScopedObject();
            w.writeMember("jsonrpc", "2.0");
            w.writeMember("method", method);
            w.writeMember("params", resp);
        }

        respondStr(output);
    }

    void pushShowMessage(FStr msg, ShowMessageParams::MessageType type = ShowMessageParams::Error) {
        ShowMessageParams params;
        params.type = type;
        params.message = std::move(msg);
        pushNotification("window/showMessage", params);
    }

    void pushLogMessage(FStr msg, LogMessageParams::MessageType type = LogMessageParams::Error) {
        LogMessageParams params;
        params.type = type;
        params.message = msg.str();
        pushNotification("window/logMessage", params);
    }

    void respondError(const RequestId& id, const ResponseError& error) {
        rapidjson::StringBuffer output;
        rapidjson::Writer<rapidjson::StringBuffer> handle(output);
        BufferWriter w(handle);

        BufferWriter::Object root = w.beginObject();
        w.writeMember("jsonrpc", "2.0");
        w.writeMember("id", id);
        w.writeMember("error", error);
        root.close();

        respondStr(output);
    }

private:
    void respondStr(const rapidjson::StringBuffer& output) {
        int bufferSize = output.GetSize();
        printf("Content-Length: %d\r\n\r\n", bufferSize);
        fputs(output.GetString(), stdout);
        fflush(stdout);

        if (dup[1]) {
            fprintf(dup[1], "Writing response: %s\n", output.GetString());
            fflush(dup[1]);
        }
    }
};

struct ReceivingServer : public RespondingServer {
    using MethodHandler = std::function<void(RespondingServer*, const rapidjson::Document&)>;
    using MethodMap = std::unordered_map<const char*, MethodHandler>;

    ReceivingServer(const InitOptions& init) : RespondingServer(init) {}

    virtual bool shouldExit() = 0;

    void stdinFunction() {
        const char* expectedMsg = "Content-Length: ";
        int charsMatched = 0;
        while (!shouldExit()) {
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

    void receiveDocument(rapidjson::Document& document) {
        ResponseError error = receiveDocumentWithError(document);
        if (error) {
            if (document.HasMember("id")) {
                RequestId id;
                id.read(document["id"]);
                respondError(id, error);
            } else {
                pushLogMessage(error.message);
            }
        }
    }

    ResponseError receiveDocumentWithError(rapidjson::Document& document) {
        if (!document.HasMember("jsonrpc")) {
            return ErrorCode::InvalidParams;
        }
        const char* jsonrpc = document["jsonrpc"].GetString();
        if (strcmp(jsonrpc, "2.0") != 0) {
            return ErrorCode::InvalidParams;
        }

        if (!document.HasMember("method")) {
            return ErrorCode::InvalidParams;
        }
        const char* methodStr = document["method"].GetString();
        ResponseError error = preInvokeMethod(methodStr);
        if (error) {
            return error;
        }

        MethodMap::iterator it = methods.find(methodStr);
        if (it == methods.end()) {
            return ErrorCode::MethodNotFound;
        }
        MethodHandler handler = it->second;
        handler(this, document);
        return ErrorCode::NoError;
    }

    virtual ResponseError preInvokeMethod(const char* methodStr) { return ErrorCode::NoError; }

protected:
    MethodMap methods;
};

struct Server : public ReceivingServer {
    using MethodNoParams = void (Server::*)(const RequestId&);

    template <typename T>
    using MethodWithParams = void (Server::*)(const RequestId&, const T&);

    using NotifNoParams = void (Server::*)();

    template <typename T>
    using NotifWithParams = void (Server::*)(const T&);

    Server(const InitOptions& opt) : ReceivingServer(opt) {
        state = NOT_INITIALIZED;

        registerMethod("initialize", &Server::initialize);
        registerMethod("shuwdown", &Server::shutdown);

        registerNotification("initialized", &Server::initialized);
        registerNotification("textDocument/didOpen", &Server::didOpen);
        registerNotification("textDocument/didChange", &Server::didChange);
        registerNotification("textDocument/didClose", &Server::didClose);
        registerNotification("exit", &Server::exit);
    }

    template <typename T>
    void registerMethod(const char* method, MethodWithParams<T> handler) {
        struct InvokeWrapper {
            InvokeWrapper(MethodWithParams<T> handler) : handler(handler) {}

            void operator()(RespondingServer* self, const rapidjson::Document& doc) {
                RequestId id;
                T params;

                ValueReader reader(doc);
                reader.readMember("params", params);
                reader.readMember("id", id);
                (static_cast<Server*>(self)->*handler)(id, params);
            }

            MethodWithParams<T> handler;
        };
        methods[method] = InvokeWrapper(handler);
    }

    void registerMethod(const char* method, MethodNoParams handler) {
        struct InvokeWrapper {
            InvokeWrapper(MethodNoParams handler) : handler(handler) {}

            void operator()(RespondingServer* self, const rapidjson::Document& doc) {
                RequestId id;

                ValueReader reader(doc);
                reader.readMember("id", id);

                (static_cast<Server*>(self)->*handler)(id);
            }

            MethodNoParams handler;
        };
        methods[method] = InvokeWrapper(handler);
    }

    template <typename T>
    void registerNotification(const char* method, NotifWithParams<T> handler) {
        struct InvokeWrapper {
            InvokeWrapper(NotifWithParams<T> handler) : handler(handler) {}

            void operator()(RespondingServer* self, const rapidjson::Document& doc) {
                ValueReader reader(doc);
                T params;
                reader.readMember("params", params);

                (static_cast<Server*>(self)->*handler)(params);
            }

            NotifWithParams<T> handler;
        };
        methods[method] = InvokeWrapper(handler);
    }

    void registerNotification(const char* method, NotifNoParams handler) {
        struct InvokeWrapper {
            InvokeWrapper(NotifNoParams handler) : handler(handler) {}

            void operator()(RespondingServer* self, const rapidjson::Document& doc) {
                (static_cast<Server*>(self)->*handler)();
            }

            NotifNoParams handler;
        };
        methods[method] = InvokeWrapper(handler);
    }

    bool shouldExit() override { return state == EXIT; }

    ResponseError preInvokeMethod(const char* methodStr) override {
        switch (state) {
        case NOT_INITIALIZED:
            if (strcmp(methodStr, "initialize") == 0) {
                return ErrorCode::NoError;
            } else {
                return ErrorCode::ServerNotInitialized;
            }

        case INITIALIZED:
            return ErrorCode::NoError;

        case SHUTDOWN:
            if (strcmp(methodStr, "exit") == 0) {
                return ErrorCode::NoError;
            } else {
                return ErrorCode::InvalidRequest;;
            }

        default:
            assert(false);
            return ErrorCode::InternalError;
        }
    }

    void initialize(const RequestId& id) {
        state = INITIALIZED;
        respondResult(id, InitializeResult());
    }

    void initialized() {}

    void shutdown(const RequestId& id) {
        state = SHUTDOWN;
        respond(id);
    }

    void exit() {
        state = EXIT;
    }

    void didOpen(const DidOpenTextDocumentParams& didOpenParams) {
        WorkingDocument* doc = docs.open(didOpenParams);
    }

    void didChange(const DidChangeTextDocumentParams& didChangeParams) {
        WorkingDocument* doc = docs.change(didChangeParams);
        if (!doc) {
            pushLogMessage("Document not opened");
        }
    }

    void didClose(const DidCloseParams& didCloseParams) {
        if (!docs.close(didCloseParams)) {
            pushLogMessage("Document not opened");
        }
    }

private:
    enum State {NOT_INITIALIZED, INITIALIZED, SHUTDOWN, EXIT} state;

    SyntaxTree ast;
    DocumentMap docs;
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
    otherMain(argc, argv);
    return 0;
}

int test() {
    /* SyntaxTree ast; */
    /* std::vector<Diagnostic> digs; */
    /* ast.reloadFromFile(TEST_FILE, digs); */
    /* Block* letBlock = root->body[0]; */
    /* EvalFactory factory; */
    /* EvalCommand* result = parse(letBlock->lexem, factory); */
    return 0;
}
