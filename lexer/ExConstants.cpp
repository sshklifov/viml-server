#include <FStr.hpp>
#include <Vector.hpp>

#include "ExConstants.hpp"
#include "ExDictionary.hpp"

#include <cstdio>
#include <cctype>

bool debugCheckExConstants() {
    const ExDictionary& dict = ExDictionary::getSingleton();
    FStr lowercase;
    Vector<int> values;

    int okay = true;

#define COMPUTE_COMMAND_VALUE(name, val)              \
    lowercase = #name;                                \
    for (int i = 0; i < lowercase.length(); ++i) {    \
        lowercase[i] = tolower(lowercase[i]);         \
    }                                                 \
    values.emplace(dict.search(lowercase));   \
    assert(values.last() != -1);                      \
    okay &= (values.last() == val);                   \

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
