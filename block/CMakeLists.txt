include_directories(../cont/)
link_libraries(continuation)
build_parser(block block.lex block.yy BlockParser.cpp)
