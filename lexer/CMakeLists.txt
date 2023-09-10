include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_SOURCE_DIR}/container)

run_flex_generator(Command.l Command.cpp)
find_file(EXCMDS_TXT NAMES excmds.txt PATHS ${CMAKE_CURRENT_LIST_DIR} REQUIRED NO_DEFAULT_PATH)
add_compile_definitions(DICTIONARY_FILE="${EXCMDS_TXT}")
add_library(lexer ExLexer.cpp ExDictionary.cpp ExConstants.cpp Command.cpp)
