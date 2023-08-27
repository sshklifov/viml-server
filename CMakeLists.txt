cmake_minimum_required(VERSION 3.16)
project(viml_server LANGUAGES CXX)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

function(run_flex_generator flex_file flex_cpp)
    get_filename_component(gen_name ${flex_cpp} NAME_WE)
    set(flex_cpp ${CMAKE_CURRENT_LIST_DIR}/${gen_name}.cpp)
    set(flex_hpp ${CMAKE_CURRENT_LIST_DIR}/${gen_name}.hpp)
    add_custom_command(OUTPUT ${flex_cpp} ${flex_hpp}
        MAIN_DEPENDENCY ${flex_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${FLEX_EXECUTABLE} -L --header-file=${flex_hpp} -o ${flex_cpp} ${flex_file})
    include_directories(${FLEX_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR})
endfunction()

function(run_bison_generator bison_file bison_we)
    message(FATAL_ERROR "Needs update, see run_flex_generator")
    set(bison_cpp ${CMAKE_CURRENT_BINARY_DIR}/${bison_we}.cpp)
    add_custom_command(OUTPUT ${bison_cpp}
        MAIN_DEPENDENCY ${bison_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${BISON_EXECUTABLE} -Werror -Wcounterexamples -o ${bison_cpp} -l ${bison_file})
    # Include the generated header
    set(bison_hpp ${bison_we}.hpp)
    add_compile_definitions(GENERATED_PARSER_HEADER=<${bison_hpp}>)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    # Pass the name of the cpp file. Optional.
    if(ARGV1)
        set(${ARGV1} ${bison_cpp} PARENT_SCOPE)
    endif()
endfunction()

# function(build_parser target flex_file bison_file)
#     run_bison_generator(${bison_file} bison_cpp)
#     run_flex_generator(${flex_file} flex_cpp)

#     add_compile_options(-std=c++14)
#     include_directories(${CMAKE_CURRENT_LIST_DIR})
#     add_executable(${target} ${bison_cpp} ${flex_cpp} ${ARGN})
# endfunction()

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(lsp)
