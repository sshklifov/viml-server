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
	BufferWriter w(handle);

	BufferWriter::ObjectScope root = w.beginObject();
	w.add("jsonrpc", "2.0");
	w.add("id", id);
	w.add("result", resp);

	int bufferSize = output.GetSize();
	printf("Content-Length: %d\r\n\r\n", bufferSize);
	fputs(output.GetString(), stdout);
	fflush(stdout);
}

template <typename T>
void writeNotification(const char* method, const T& resp) {
	rapidjson::StringBuffer output;
	rapidjson::Writer<rapidjson::StringBuffer> handle(output);
	BufferWriter w(handle);

	BufferWriter::ObjectScope root = w.beginObject();
	w.add("jsonrpc", "2.0");
	w.add("method", method);
	w.add("params", resp);

	int bufferSize = output.GetSize();
	printf("Content-Length: %d\r\n\r\n", bufferSize);
	fputs(output.GetString(), stdout);
	fflush(stdout);
}

void processJson(rapidjson::Document& document) {
	if (!document.HasMember("jsonrpc") || strcmp(document["jsonrpc"].GetString(), "2.0") != 0) {
		return;
	}
	if (!document.HasMember("method")) {
		return;
	}

	const char* method = document["method"].GetString();
	if (strcmp(method, "initialize") == 0) {
		// TODO check id
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
					processJson(document);
					delete[] message;
				}
			}
		}
	}

#if 0
	if (!received_exit) {
		const std::string_view str("{\"jsonrpc\":\"2.0\",\"method\":\"exit\"}");
		auto message = std::make_unique< char[] >(str.size());
		std::copy(str.begin(), str.end(), message.get());
		auto document = std::make_unique< rapidjson::Document >();
		document->Parse(message.get(), str.size());

		FILE* fp = fopen("/home/shs1sf/viml-server/lsp.txt", "a");
		assert(fp);
		fprintf(fp, "FOUND QUIT: %s\n", message.get());
		fclose(fp);
		return;
	}
#endif
}

int main() {
	/* FILE* fp = fopen("/home/stef/viml-server/stdin.txt", "w"); */
	/* fclose(fp); */

	/* int c = fgetc(stdin); */
	/* while (c != EOF) { */
	/*	   fp = fopen("/home/stef/viml-server/stdin.txt", "a"); */
	/*	   fputc(c, fp); */
	/*	   fclose(fp); */
	/*	   c = fgetc(stdin); */
	/* } */
	stdinFunction();
	return 0;
}
