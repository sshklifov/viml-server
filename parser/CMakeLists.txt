include_directories(${CMAKE_SOURCE_DIR}/lexer)
include_directories(${CMAKE_SOURCE_DIR}/container)
include_directories(${CMAKE_SOURCE_DIR}/neovim)
include_directories(${CMAKE_SOURCE_DIR}/nodes)
include_directories(${CMAKE_CURRENT_LIST_DIR})

link_libraries(lexer)
add_library(parser SyntaxTree.cpp)
