include_directories(${CMAKE_SOURCE_DIR}/lexer)
include_directories(${CMAKE_CURRENT_LIST_DIR})
link_libraries(lexer)
add_executable(parser Parser.cpp Constants.cpp)
