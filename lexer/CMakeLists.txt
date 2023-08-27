run_flex_generator(Command.l Command.cpp)
include_directories(${CMAKE_SOURCE_DIR}/lsp)
include_directories(${CMAKE_CURRENT_LIST_DIR})
add_library(lexer ExLexer.cpp ExDictionary.cpp Command.cpp)
