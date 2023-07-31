cmake_minimum_required(VERSION 3.16)
project(viml_server LANGUAGES CXX)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

function(run_flex_generator flex_file)
    get_filename_component(flex_name ${flex_file} NAME_WE)
    set(flex_cpp ${CMAKE_CURRENT_BINARY_DIR}/${flex_name}Lexer.cpp)
    set(flex_name_hpp ${flex_name}Lexer.hpp)
    set(flex_hpp ${CMAKE_CURRENT_BINARY_DIR}/${flex_name_hpp})
    add_custom_command(OUTPUT ${flex_cpp}
        MAIN_DEPENDENCY ${flex_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${FLEX_EXECUTABLE} -L --header-file=${flex_hpp} -o ${flex_cpp} ${flex_file})
    add_compile_definitions(GENERATED_LEXER_HEADER=<${flex_name_hpp}>)
    # Pass the name of the cpp file. Optional.
    if(ARGV1)
        set(${ARGV1} ${flex_cpp} PARENT_SCOPE)
    endif()
endfunction()

function(run_bison_generator bison_file)
    get_filename_component(bison_name ${bison_file} NAME_WE)
    set(bison_cpp ${CMAKE_CURRENT_BINARY_DIR}/${bison_name}Parser.cpp)
    add_custom_command(OUTPUT ${bison_cpp}
        MAIN_DEPENDENCY ${bison_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${BISON_EXECUTABLE} -Werror -Wcounterexamples -l -o ${bison_cpp} ${bison_file})
    # Include the generated header
    set(bison_name_hpp ${bison_name}Parser.hpp)
    add_compile_definitions(GENERATED_PARSER_HEADER=<${bison_name_hpp}>)
    # Pass the name of the cpp file. Optional.
    if(ARGV1)
        set(${ARGV1} ${bison_cpp} PARENT_SCOPE)
    endif()
endfunction()

function(build_parser target flex_file bison_file)
    set(source_path ${CMAKE_CURRENT_LIST_DIR})
    set(generated_path ${CMAKE_CURRENT_BINARY_DIR})
    run_bison_generator(${bison_file} bison_cpp)
    run_flex_generator(${flex_file} flex_cpp)

    add_compile_options(-std=c++14)
    include_directories(${FLEX_INCLUDE_DIRS} ${source_path} ${generated_path})
    link_libraries(${FLEX_LIBRARIES})
    add_executable(${target} ${bison_cpp} ${flex_cpp} ${ARGN})
endfunction()

add_subdirectory(cont)
add_subdirectory(block)
add_subdirectory(excmds)
