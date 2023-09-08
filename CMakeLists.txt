cmake_minimum_required(VERSION 3.16)
project(viml_server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

function(run_flex_generator flex_file flex_cpp)
    get_filename_component(gen_name ${flex_cpp} NAME_WE)
    set(flex_cpp ${CMAKE_CURRENT_BINARY_DIR}/${gen_name}.cpp)
    set(flex_hpp ${CMAKE_CURRENT_BINARY_DIR}/${gen_name}.hpp)
    add_custom_command(OUTPUT ${flex_cpp} ${flex_hpp}
        MAIN_DEPENDENCY ${flex_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${FLEX_EXECUTABLE} -L --header-file=${flex_hpp} -o ${flex_cpp} ${flex_file})
    include_directories(${FLEX_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

function(run_bison_generator bison_file bison_cpp)
    get_filename_component(gen_name ${bison_cpp} NAME_WE)
    set(bison_cpp ${CMAKE_CURRENT_BINARY_DIR}/${gen_name}.cpp)
    set(bison_hpp ${CMAKE_CURRENT_BINARY_DIR}/${gen_name}.hpp)
    add_custom_command(OUTPUT ${bison_cpp} ${bison_hpp}
        MAIN_DEPENDENCY ${bison_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${BISON_EXECUTABLE} -l -Werror -Wcounterexamples --header=${bison_hpp} -o ${bison_cpp} ${bison_file})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endfunction()

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(eval)
add_subdirectory(fstr)
add_subdirectory(lsp)
