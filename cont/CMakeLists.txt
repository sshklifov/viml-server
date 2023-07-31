run_flex_generator(cont.lex)
add_library(continuation ${CMAKE_CURRENT_BINARY_DIR}/contLexer.cpp ContParser.cpp)
