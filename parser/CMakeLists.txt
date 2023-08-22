include_directories(${CMAKE_SOURCE_DIR}/lsp)
include_directories(${CMAKE_SOURCE_DIR}/lexer) # TODO Remove. Needed for StringView.
include_directories(${CMAKE_CURRENT_LIST_DIR})
link_libraries(lexer)
add_library(parser Parser.cpp)
