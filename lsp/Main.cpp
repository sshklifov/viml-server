#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <string_view>

#include <cstdio>

#include "Initialize.hpp"
#include "Diagnostics.hpp"
#include "TextDocument.hpp"

template <typename T>
void writeResponse(int id, const T& resp) {
    rapidjson::StringBuffer output;
    rapidjson::Writer<rapidjson::StringBuffer> handle(output);

    Writer w(handle);

    ObjectScope root = w.newObject();
    w.add("jsonrpc", "2.0");
    w.add("id", id);
    w.add("result", resp);
    root.close();

    int bufferSize = output.GetSize();
    printf("Content-Length: %d\r\n\r\n", bufferSize);
    fputs(output.GetString(), stdout);
    fflush(stdout);
}

template <typename T>
void writeNotification(const char* method, const T& resp) {
    rapidjson::StringBuffer output;
    rapidjson::Writer<rapidjson::StringBuffer> handle(output);

    Writer w(handle);

    ObjectScope root = w.newObject();
    w.add("jsonrpc", "2.0");
    w.add("method", method);
    w.add("params", resp);
    root.close();

    int bufferSize = output.GetSize();
    printf("Content-Length: %d\r\n\r\n", bufferSize);
    fputs(output.GetString(), stdout);
    fflush(stdout);
}

void stdinFunction() {
	std::string str;
	const std::string_view kContentLength("Content-Length: ");
	bool received_exit = false;
	while (true) {
		int len = 0;
		str.clear();
		while (true) {
			int c = getchar();
			if (c == EOF) {
                goto quit;
            } else if (c == '\n') {
				if (str.empty()) {
                    break;
                }
				if (!str.compare(0, kContentLength.size(), kContentLength)) {
					len = atoi(str.c_str() + kContentLength.size());
                }
				str.clear();
			} else if (c != '\r') {
				str += c;
			}
		}

        if (len == 0) {
            goto quit;
        }

		str.resize(len);
		for (int i = 0; i < len; ++i) {
			int c = getchar();
			if (c == EOF) goto quit;
			str[i] = c;
		}

		auto message = std::make_unique< char[] >(len);
		std::copy(str.begin(), str.end(), message.get());

		/* FILE* fp = fopen("/home/stef/viml-server/stdin.txt", "a"); */
		/* assert(fp); */
		/* fprintf(fp, "Content-Length: %d\n%s\n", len, message.get()); */
		/* fclose(fp); */

        rapidjson::Document document;
		document.Parse(message.get(), len);
		assert(!document.HasParseError());

        const char* method = document["method"].GetString();
        if (strcmp(method, "initialize") == 0) {
            int id = document["id"].GetInt();
            writeResponse(id, InitializeResult());
        } else if (strcmp(method, "initialize") == 0) {
            // Do Nothing
        } else if (strcmp(method, "textDocument/didOpen") == 0) {
            ValueReader reader(document.GetObject());
            DidOpenTextDocumentParams params;
            reader.read(params);
        }

        /* PublishDiagnosticsParams params; */
        /* Diagnostic dig; */
        /* dig.message = "Underfined reference to server"; */
        /* dig.severity = Diagnostic::Error; */
        /* dig.range.start.line = 1; */
        /* dig.range.start.character = 1; */
        /* dig.range.end.line = 1; */
        /* dig.range.end.character = 2; */
        /* params.diagnostics.push_back(dig); */
        /* params.uri = "file:///home/stef/.vimrc"; */
        /* writeNotification("textDocument/publishDiagnostics", params); */
        /* return; */

#if 0

		JsonReader reader{document.get()};
		if (!reader.m->HasMember("jsonrpc") || std::string((*reader.m)["jsonrpc"].GetString()) != "2.0") break;
		RequestId id;
		std::string method;
		/* reflectMember(reader, "id", id); */
		/* reflectMember(reader, "method", method); */
		/* if (id.valid()) */
		/*	   LOG_V(2) << "receive RequestMessage: " << id.value << " " << method; */
		/* else */
		/*	   LOG_V(2) << "receive NotificationMessage " << method; */
		if (method.empty()) continue;
		received_exit = (method == "exit");
		// g_config is not available before "initialize". Use 0 in that case.
		/* auto tuple = std::make_tuple(id, method, message, document); */

		if (received_exit) break;
#endif
	}

quit:
	if (!received_exit) {
#if 0
		const std::string_view str("{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");
		auto message = std::make_unique< char[] >(str.size());
		std::copy(str.begin(), str.end(), message.get());
		auto document = std::make_unique< rapidjson::Document >();
		document->Parse(message.get(), str.size());

		FILE* fp = fopen("/home/shs1sf/viml-server/lsp.txt", "a");
		assert(fp);
		fprintf(fp, "FOUND QUIT: %s\n", message.get());
		fclose(fp);
#endif
		return;
	}
}

int main() {
    /* FILE* fp = fopen("/home/stef/viml-server/stdin.txt", "w"); */
    /* fclose(fp); */

    /* int c = fgetc(stdin); */
    /* while (c != EOF) { */
    /*     fp = fopen("/home/stef/viml-server/stdin.txt", "a"); */
    /*     fputc(c, fp); */
    /*     fclose(fp); */
    /*     c = fgetc(stdin); */
    /* } */
	stdinFunction();
	return 0;
}
