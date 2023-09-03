run_flex_generator(Eval.l EvalLexer.cpp)
run_bison_generator(Eval.y EvalParser.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_executable(eval EvalLexer.cpp EvalParser.cpp Main.cpp)
