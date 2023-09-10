run_flex_generator(EvalLexer.l EvalLexer.cpp)
run_bison_generator(EvalParser.y EvalParser.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lexer)
include_directories(${CMAKE_SOURCE_DIR}/parser)
include_directories(${CMAKE_SOURCE_DIR}/lsp)
include_directories(${CMAKE_SOURCE_DIR}/container)
add_library(eval EvalLexer.cpp EvalParser.cpp Eval.cpp)
