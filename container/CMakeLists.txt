include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lexer)
add_library(container BitTwiddle.cpp FStr.cpp)
