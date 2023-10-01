include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_SOURCE_DIR}/container)
include_directories(${CMAKE_SOURCE_DIR}/lexer)
include_directories(${CMAKE_SOURCE_DIR}/nodes)

link_libraries(container)
add_library(neovim DoCmd.cpp Options.cpp Eval.cpp Charset.cpp Mbyte.cpp ExCmdsDefs.cpp)
