include_directories(${CMAKE_SOURCE_DIR}/parser)
include_directories(${CMAKE_SOURCE_DIR}/lexer)
include_directories(${CMAKE_SOURCE_DIR}/eval) # TODO needed?
include_directories(${CMAKE_SOURCE_DIR}/fstr)
include_directories(${CMAKE_CURRENT_LIST_DIR})

find_file(TEST_TXT NAMES test.txt PATHS ${CMAKE_CURRENT_LIST_DIR} REQUIRED NO_DEFAULT_PATH)
add_compile_definitions(TEST_FILE="${TEST_TXT}")

link_libraries(parser)
link_libraries(eval)
link_libraries(fstr)
add_executable(lsp Main.cpp)
