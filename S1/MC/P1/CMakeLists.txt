cmake_minimum_required(VERSION 3.10)

project(InterpreteMarco CXX)
# 17 para los variant de la tabla de tipos
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(GTest CONFIG REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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


# COSAS DE TEST
add_subdirectory(test/googletest)
enable_testing()
# Crear ejecutable de pruebas
add_executable(my_test test/test.cc)
target_include_directories(my_test PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${FLEX_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(my_test gtest gtest_main pthread)
include(GoogleTest)
gtest_discover_tests(my_test)
add_test(NAME basic COMMAND my_test)