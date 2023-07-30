set(flex_file cont.lex)
set(flex_cpp ${CMAKE_CURRENT_BINARY_DIR}/contLexer.cpp)
add_custom_command(OUTPUT ${flex_cpp}
    MAIN_DEPENDENCY ${flex_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND flex -L -o ${flex_cpp} ${flex_file})

add_definitions(-fvisibility=hidden)
add_library(continuation ${flex_cpp} ContParser.cpp)
