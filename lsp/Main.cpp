#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <string_view>

#include <cstdio>

#include <argp.h>

#include "Initialize.hpp"
#include "Diagnostics.hpp"
#include "TextDocument.hpp"
#include <Parser.hpp>

struct Arguments {
	FILE* redirOut;
	FILE* redirIn;
} args;

template <typename T>
void writeResponse(int id, const T& resp) {
	rapidjson::StringBuffer output;
	rapidjson::Writer<rapidjson::StringBuffer> handle(output);
	BufferWriter w(handle);

	{
		BufferWriter::ObjectScope root = w.beginObject();
		w.add("jsonrpc", "2.0");
		w.add("id", id);
		w.add("result", resp);
	}

	int bufferSize = output.GetSize();
	printf("Content-Length: %d\r\n\r\n", bufferSize);
	fputs(output.GetString(), stdout);
	fflush(stdout);

	if (args.redirOut) {
		fprintf(args.redirOut, "Writing response: %s\n", output.GetString());
		fflush(args.redirOut);
	}
}

template <typename T>
void writeNotification(const char* method, const T& resp) {
	rapidjson::StringBuffer output;
	rapidjson::Writer<rapidjson::StringBuffer> handle(output);
	BufferWriter w(handle);

	{
		BufferWriter::ObjectScope root = w.beginObject();
		w.add("jsonrpc", "2.0");
		w.add("method", method);
		w.add("params", resp);
	}

	int bufferSize = output.GetSize();
	printf("Content-Length: %d\r\n\r\n", bufferSize);
	fputs(output.GetString(), stdout);
	fflush(stdout);

	if (args.redirOut) {
		fprintf(args.redirOut, "Writing notification: %s\n", output.GetString());
		fflush(args.redirOut);
	}
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

		const char* uri = params.textDocument.uri;
		char scheme[] = "file://";
		if (strncmp(uri, scheme, sizeof(scheme)) != 0) {
			// Report error idk
		}
		const char* filename = uri + sizeof(scheme);
		const bool hasQuery = strchr(filename, '?') != nullptr;
		const bool hasFragment = strchr(filename, '#') != nullptr;
		if (hasQuery || hasFragment) {
			// Report error idk
		}
		
		SyntaxTree ast;
		PublishDiagnosticsParams diagnosticsParams;
		RootBlock* root = ast.build(filename, diagnosticsParams.diagnostics);
		if (!root) {
			// Report error idk
		}

        // Report errors
        if (!diagnosticsParams.diagnostics.empty()) {
            diagnosticsParams.uri = uri;
            writeNotification("textDocument/publishDiagnostics", params);
        }
	}

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
					if (args.redirIn) {
                        fprintf(args.redirIn, "Content-Length: %d\n\n", len + 1);
						fputs(message, args.redirIn);
                        fputc('\n', args.redirIn);
						fflush(args.redirIn);
					}
					// Process
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

error_t argsParser(int key, char *arg, argp_state *state) {
	Arguments* user = (Arguments*)(state->input);
	switch (key) {
	case 'i':
		user->redirIn = fopen(arg, "w");
		if (!user->redirIn) {
			int err = errno;
			fputs("Failed to open --stdin file\n", state->err_stream);
			return err;
		} else {
			return 0;
		}
	case 'o':
		user->redirOut = fopen(arg, "w");
		if (!user->redirOut) {
			int err = errno;
			fputs("Failed to open --stdout file", state->err_stream);
			return err;
		} else {
			return 0;
		}

	default:
		return ARGP_ERR_UNKNOWN;
	}
}

void alternateMain() {
    SyntaxTree ast;
    std::vector<Diagnostic> digs;
    RootBlock* root = ast.build("/home/stef/viml-server/test.txt", digs);
    if (!root) {
        // Report error idk
    }
    for (const Diagnostic& dig : digs) {
        printf("%s\n", dig.message);
    }

    exit(0);
}

int main(int argc, char** argv) {
    alternateMain();

	const char* argsDoc = ""; // < No args
	const char* doc = "Vim LSP";

	struct argp_option options[] = {
		{"stdin",  'i', "FILE", 0, "Redirect input LSP messages"},
		{"stdout", 'o', "FILE", 0, "Redirect output LSP messages"},
		{0}
	};

	struct argp argp = {options, argsParser, argsDoc, doc};

	args.redirIn = NULL;
	args.redirOut = NULL;
	int errnum = argp_parse(&argp, argc, argv, 0, NULL, &args);
	if (errnum != 0) {
		return 1;
	}

	stdinFunction();

	// Fuck it, stderr is more comfortable to use
	if (args.redirIn) {
		fclose(args.redirIn);
		/* args.redirIn = stderr; */
	}
	if (args.redirOut) {
		fclose(args.redirOut);
		/* args.redirOut = stderr; */
	}

	return 0;
}
