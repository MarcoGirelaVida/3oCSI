cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(CompiladorMarcoToCpp)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Cosas de bison y lex
# Ubicación de los archivos fuente Lex y Yacc
set(LEX_FILE ${CMAKE_SOURCE_DIR}/gramaticas/tokenizador.l)
set(YACC_FILE ${CMAKE_SOURCE_DIR}/gramaticas/parser.y)

# Archivos generados por Flex y Bison
set(LEX_OUTPUT ${CMAKE_BINARY_DIR}/lex.yy.cc)
set(YACC_OUTPUT ${CMAKE_BINARY_DIR}/y.tab.cc)
set(YACC_HEADER ${CMAKE_BINARY_DIR}/y.tab.h)

# Generación de los ficheros
add_custom_command(
    OUTPUT ${YACC_OUTPUT} ${YACC_HEADER}
    COMMAND bison -d -o ${YACC_OUTPUT} ${YACC_FILE}
    MAIN_DEPENDENCY ${YACC_FILE}
    COMMENT "Generando el analizador sintáctico (Bison)"
)

add_custom_command(
    OUTPUT ${LEX_OUTPUT}
    COMMAND flex -o ${LEX_OUTPUT} ${LEX_FILE}
    MAIN_DEPENDENCY ${LEX_FILE}
    COMMENT "Generando el analizador léxico (Flex)"
)

# Incluir los archivos generados como parte del proyecto
add_library(lexer_parser ${LEX_OUTPUT} ${YACC_OUTPUT})

# Fuentes
file(GLOB SOURCES
"${CMAKE_SOURCE_DIR}/src/*.cc"
$(LEX_OUTPUT)
$(YACC_OUTPUT)
)


# Ejecutable
add_executable(marco_compiler ${SOURCES})
target_link_libraries(marco_compiler lexer_parser)

# Include
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}
)

# Archivos específicos a eliminar con make clean
add_custom_target(clean_test COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_SOURCE_DIR}/test/foo.cc)
add_custom_target(clean_all DEPENDS clean clean_test)