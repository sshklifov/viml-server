run_flex_generator(cont.lex flex_cpp)
add_library(continuation ${flex_cpp} ContParser.cpp)
