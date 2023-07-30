cmake_minimum_required(VERSION 3.27)
project(viml_server LANGUAGES CXX)

function(build_parser exe flex_file bison_file)
    set(source_path ${CMAKE_CURRENT_LIST_DIR})
    set(generated_path ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${generated_path})

    get_filename_component(bison_name ${bison_file} NAME_WE)
    set(bison_cpp ${generated_path}/${bison_name}Parser.cpp)
    set(bison_hpp ${bison_name}Parser.hpp)
    add_custom_command(OUTPUT "${bison_cpp}"
        MAIN_DEPENDENCY ${bison_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND bison -Werror -Wcounterexamples -l -o ${bison_cpp} ${bison_file})

    get_filename_component(flex_name ${flex_file} NAME_WE)
    set(flex_cpp ${generated_path}/${flex_name}Lexer.cpp)
    add_custom_command(OUTPUT ${flex_cpp}
        MAIN_DEPENDENCY ${flex_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND flex -L -o ${flex_cpp} ${flex_file})

    add_compile_definitions(GENERATED_PARSER_HEADER=<${bison_hpp}>)
    add_compile_options(-std=c++20)
    include_directories(${source_path} ${generated_path})
    add_executable(${exe} ${bison_cpp} ${flex_cpp} ${ARGN})
endfunction()

add_subdirectory(cont)
add_subdirectory(block)
add_subdirectory(ex_cmds)
