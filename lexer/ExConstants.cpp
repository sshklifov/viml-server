#include "ExConstants.hpp"
#include "ExDictionary.hpp"

bool checkExConstants(const ExDictionary& dict) {
    std::string lowercase;
    std::vector<int> values;

    int okay = true;

#define COMPUTE_COMMAND_VALUE(name, val)              \
    lowercase = #name;                                \
    for (int i = 0; i < lowercase.length(); ++i) {    \
        lowercase[i] = tolower(lowercase[i]);         \
    }                                                 \
    values.push_back(dict.search(lowercase.c_str())); \
    okay &= (values.back() == val);                   \

    FOR_EACH_COMMAND(COMPUTE_COMMAND_VALUE);

#undef COMPUTE_COMMAND_VALUE

    if (okay) {
        return true;
    }

    int i = 0;

#define PRINT_COMMAND_VALUE(name, val) printf("    code(%s, %d) \\\n", #name, values[i++]);

    FOR_EACH_COMMAND(PRINT_COMMAND_VALUE)

#undef PRINT_COMMAND_VALUE

    return okay;
}
