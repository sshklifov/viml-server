include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_SOURCE_DIR}/container)
include_directories(${CMAKE_SOURCE_DIR}/neovim)

link_libraries(neovim)
add_library(lexer ExLexer.cpp)
