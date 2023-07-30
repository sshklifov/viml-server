cmake_minimum_required(VERSION 3.27)
project(viml_server LANGUAGES CXX)

function(build_parser name)
    set(source_path ${CMAKE_CURRENT_LIST_DIR})
    set(generated_path ${CMAKE_CURRENT_BINARY_DIR}/gen)
    file(MAKE_DIRECTORY ${generated_path})

    set(bison_file ${source_path}/${name}.yy)
    set(bison_cpp ${generated_path}/${name}_parser.cpp)
    add_custom_command(OUTPUT "${bison_cpp}"
        MAIN_DEPENDENCY ${bison_file}
        COMMAND bison -Werror -Wcounterexamples -l -o ${bison_cpp} ${bison_file})

    set(flex_file ${source_path}/${name}.lex)
    set(flex_cpp ${generated_path}/${name}_lexer.cpp)
    add_custom_command(OUTPUT ${flex_cpp}
        MAIN_DEPENDENCY ${flex_file}
        COMMAND flex -L -o ${flex_cpp} ${flex_file})

    set(main_cpp ${source_path}/${name}.cpp)
    include_directories()
    add_definitions(-std=c++20)
    include_directories(${source_path} ${generated_path})
    add_executable(${name} ${main_cpp} ${bison_cpp} ${flex_cpp})
endfunction()

build_parser(tree)
