cmake_minimum_required(VERSION 3.10)

project(InterpreteMarco CXX)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

BISON_TARGET(parser ${CMAKE_SOURCE_DIR}/gramaticas/parser.yy ${CMAKE_BINARY_DIR}/parser.tab.cc COMPILE_FLAGS "-Wcounterexamples --verbose")
FLEX_TARGET(lexer ${CMAKE_SOURCE_DIR}/gramaticas/lexer.l ${CMAKE_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable ( marco_language
    ${CMAKE_SOURCE_DIR}/src/main.cc
    ${CMAKE_SOURCE_DIR}/src/interprete.cc
    ${CMAKE_SOURCE_DIR}/src/utils.cc

    ${FLEX_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)
target_include_directories ( marco_language PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${FLEX_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
)
set_property(TARGET marco_language PROPERTY CXX_STANDARD 14)

#enable_testing()
#add_test(NAME basic COMMAND ${CMAKE_SOURCE_DIR}/test/fuente.marco WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
