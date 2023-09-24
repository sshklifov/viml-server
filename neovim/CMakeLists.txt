include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_SOURCE_DIR}/eval)
include_directories(${CMAKE_SOURCE_DIR}/container)
include_directories(${CMAKE_SOURCE_DIR}/lexer)

link_libraries(container)
add_executable(nvim Charset.cpp Eval.cpp Options.cpp Test.cpp)
