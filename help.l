%option noyywrap
%s CMD

space [ \t]*

range "[range]"|"{range}"
count "[count]"|"{count}"
line "[line]"|"{line}"
bufnum "[N]"|"{N}"

pre_num {range}|{bufnum}|{count}|{line}

cmd_name [a-zA-Z_][a-zA-Z0-9_]*
bang "[!]"
optional "["[^]]+"]"
argument "{"[^}]+"}"
arg_or_opt {argument}|{optional}

%{
#include <string>
#include <algorithm>

void removeRange(std::string& s) {
    if (s.size() > 1 && s[1] == '[') {
        size_t i = s.find(']');
        if (i > 1 && i < std::string::npos) {
            s = s.erase(1, i);
        }
    }
}

void removeString(std::string& s, const char* what) {
    size_t i = s.find(what);
    if (i < std::string::npos) {
        s.erase(i, strlen(what));
    }
}

void removeBrackets(std::string& s) {
    size_t start = s.find('[');
    if (start != std::string::npos) {
        s = s.erase(start, 1);
    }
    size_t finish = s.find(']');
    if (finish != std::string::npos) {
        s = s.erase(finish, 1);
    }
}
%}

%%
^{space}?[:]{pre_num}?{cmd_name}{optional}?{bang}? {
    std::string name = yytext;
    removeString(name, "[line]");
    removeString(name, "{line}");
    removeString(name, "[count]");
    removeString(name, "{count}");
    removeString(name, "[range]");
    removeString(name, "[N]");
    removeString(name, "[!]");
    removeString(name, "{range}");
    removeString(name, "{N}");
    removeBrackets(name);
    printf("%s\n", name.c_str());

    //BEGIN(CMD);
    //REJECT;
}


.|"\n" {
    // IGNORE
}
%%

int main() {
    yylex();
}
//<CMD>^[:]{pre_num}?{cmd_name}{bang}?{bang}?([ \t]+{arg_or_opt})* {
//}

// [++opt]
