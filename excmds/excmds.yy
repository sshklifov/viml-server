%token COMMAND ENUM FLAGS_FUN ADDR_TYPE FUNC PREVIEW_FUNC
%token BEGIN_BLOCK BIT_OR
%token ID STR

%{
#include <string>
#include <vector>
#include <map>

#include <cstdio>

#define YYDEBUG 1
#define YYFPRINTF fprintf
#define YYPRINT

void yyerror(const char*);
int yylex();

enum CommandFlags {
    RANGE      =     0x001,
    BANG       =     0x002,
    EXTRA      =     0x004,
    XFILE      =     0x008,
    NOSPC      =     0x010,
    DFLALL     =     0x020,
    WHOLEFOLD  =     0x040,
    NEEDARG    =     0x080,
    TRLBAR     =     0x100,
    REGSTR     =     0x200,
    COUNT      =     0x400,
    NOTRLCOM   =     0x800,
    ZEROR      =    0x1000,
    CTRLV      =    0x2000,
    CMDARG     =    0x4000,
    BUFNAME    =    0x8000,
    BUFUNL     =   0x10000,
    ARGOPT     =   0x20000,
    SBOXOK     =   0x40000,
    CMDWIN     =   0x80000,
    MODIFY     =  0x100000,
    FLAGS      =  0x200000,
    LOCK_OK    = 0x1000000,
    PREVIEW    = 0x8000000,
    FILES      = XFILE | EXTRA,
    WORD1      = EXTRA | NOSPC,
    FILE1      = FILES | NOSPC
};

std::map<std::string, int> toCommandFlag = {
    {"RANGE", 0x001},
    {"BANG", 0x002},
    {"EXTRA", 0x004},
    {"XFILE", 0x008},
    {"NOSPC", 0x010},
    {"DFLALL", 0x020},
    {"WHOLEFOLD", 0x040},
    {"NEEDARG", 0x080},
    {"TRLBAR", 0x100},
    {"REGSTR", 0x200},
    {"COUNT", 0x400},
    {"NOTRLCOM", 0x800},
    {"ZEROR", 0x1000},
    {"CTRLV", 0x2000},
    {"CMDARG", 0x4000},
    {"BUFNAME", 0x8000},
    {"BUFUNL", 0x10000},
    {"ARGOPT", 0x20000},
    {"SBOXOK", 0x40000},
    {"CMDWIN", 0x80000},
    {"MODIFY", 0x100000},
    {"FLAGS", 0x200000},
    {"LOCK_OK", 0x1000000},
    {"PREVIEW", 0x8000000},
    {"FILES", FILES},
    {"WORD1", WORD1},
    {"FILE1", FILE1},
};

struct Command {
    void Clear() {
        command = "";
        flags.clear();
        addr_type = "";
        func = "";
        preview_func = "";
    }

    std::string command;
    std::vector<int> flags;
    std::string addr_type;
    std::string func;
    std::string preview_func;
};

Command currcmd;
std::vector<Command> commands;

extern std::string match_capture;
%}

%%
input: BEGIN_BLOCK '=' '{' entries '}'
;

entries: entry maybe_comma | entry ',' entries
;

entry: '{' command ',' enum maybe_comma flags ',' addr_type ',' func ',' preview_func maybe_comma '}' { commands.push_back(currcmd); currcmd.Clear(); }

command: COMMAND '=' STR { currcmd.command = match_capture; }
;

enum: %empty | ENUM '=' STR

flags: FLAGS_FUN '=' BIT_OR '(' fargs ')'
     | FLAGS_FUN '=' ID { currcmd.flags.push_back(toCommandFlag[match_capture]); }
;

fargs: %empty
     | ID            { currcmd.flags.push_back(toCommandFlag[match_capture]); }
     | fargs ',' ID  { currcmd.flags.push_back(toCommandFlag[match_capture]); }
;

addr_type: ADDR_TYPE '=' STR { currcmd.addr_type = match_capture; }
;

preview_func: %empty | PREVIEW_FUNC '=' STR { currcmd.preview_func = match_capture; }

func: FUNC '=' STR

maybe_comma: %empty | ','
;
%%

void yyerror (const char* s) {
    printf ("%s\n", s);
}

int main() {
    const int flex_debug = 0;
    if (!flex_debug) {
        yydebug = 0;
        yyparse();

        for (const Command& cmd : commands) {
            printf(":%s\n", cmd.command.c_str());
        }
        //std::string s = root->getString();
        //printf("%s", s.c_str());
    } else {
        int yychar = 0;
        do {
            yychar = yylex();
            if (yychar >= 32 && yychar < 126) {
                printf("Pass-through: %c\n", yychar);
            } else {
                int tr = YYTRANSLATE(yychar);
                printf("Lex=%s\n", yytname[tr]);
            }
        }
        while (yychar != 0);
    }


    return 0;
}

