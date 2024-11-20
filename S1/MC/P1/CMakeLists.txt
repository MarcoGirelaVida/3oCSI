cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(CompiladorMarcoToCpp)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Cosas de bison y lex
FLEX_TARGET(Tokenizador
    ${CMAKE_SOURCE_DIR}/gramaticas/tokenizador.l
    ${CMAKE_BINARY_DIR}/lex.yy.c
    COMPILE_FLAGS -+
)

BISON_TARGET(Parser
    ${CMAKE_SOURCE_DIR}/gramaticas/parser.y     # Archivo .y
    ${CMAKE_BINARY_DIR}/y.tab.c                 # Salida .c
    DEFINES_FILE ${CMAKE_BINARY_DIR}/y.tab.h    # Salida .h
    )

# Include
    include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}
    )

# Fuentes
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_BINARY_DIR}/lex.yy.c"
    "${CMAKE_BINARY_DIR}/y.tab.c"
)


# Ejecutable
add_executable(marco_compiler ${SOURCES})

# Dependencias para que se ejecuten antes lex y yacc
#add_dependencies(marco_compiler Tokenizador Parser)

# Archivos específicos a eliminar con make clean
add_custom_target(clean_test COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_SOURCE_DIR}/test/foo.cc)
add_custom_target(clean_all DEPENDS clean clean_test)