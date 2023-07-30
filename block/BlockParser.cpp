#include <ContParser.hpp>

#include GENERATED_PARSER_HEADER
GroupBlock* root;

extern struct yy_buffer_state* block_scan_bytes(const char* yybytes, int yybytes_len);
extern void block_delete_buffer (yy_buffer_state* b);
extern int blocklex();
extern char* blockget_text();
extern int blockget_leng();

void yy::parser::error(const std::string& msg) {
    fprintf(stderr, "Erorr: %s\n", msg.c_str());
    exit(5);
}

int yyparse() {
    root = new GroupBlock;
    yy::parser parser;

    int res =  parser();
    std::string program = root->toString();
    fputs(program.c_str(), stdout);
    return res;
}


int yylex(yy::parser::value_type* p) {
    using kind_type = yy::parser::token::token_kind_type;

    int t = blocklex();
    switch (t) {
    case kind_type::EX:
    case kind_type::QARGS:
        p->build<std::string>(blockget_text());
        return t;

    case kind_type::YYEOF:
    case kind_type::YYerror:
    case kind_type::YYUNDEF:
        return t;

    default:
        p->build<int>(t);
        return t;
    }
}

void yylex_debug() {
    using kind_type = yy::parser::token::token_kind_type;
    int kt;

    do {
        kt = blocklex();
        printf("(%d): %s", kt, blockget_text());
        if (kt != '\n') {
            printf("\n");
        }
    }
    while (kt != kind_type::YYEOF && kt != kind_type::YYerror);
}

int main () {
    const char* file = "test.txt";
    if (!ContParser::Get().tryLoad(file)) {
        fprintf(stderr, "Failed to load with file %s\n", file);
        return 1;
    }
    int len = 0;
    std::unique_ptr<char[]> input = ContParser::Get().lex(len);
    ContParser::Get().unload();

    yy_buffer_state* yybuffer = block_scan_bytes(input.get(), len);
    yyparse();

    block_delete_buffer(yybuffer);
    input.release();
}
