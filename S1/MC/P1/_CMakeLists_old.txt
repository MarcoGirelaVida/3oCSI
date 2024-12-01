cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(InterpreteMarcoToCpp)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Archivos de utilidades
set(UTILS_SOURCES ${CMAKE_SOURCE_DIR}/src/utils.cc)
set(UTILS_HEADERS ${CMAKE_SOURCE_DIR}/include/utils.hh)
add_library(utils ${UTILS_SOURCES})
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)


# Cosas de bison y lex
# Ubicación de los archivos fuente Lex y Yacc
set(LEX_FILE ${CMAKE_SOURCE_DIR}/gramaticas/tokenizador.l)
set(YACC_FILE ${CMAKE_SOURCE_DIR}/gramaticas/parser.y)

# Archivos generados por Flex y Bison
set(LEX_OUTPUT ${CMAKE_BINARY_DIR}/tokenizador.cc)
set(YACC_OUTPUT ${CMAKE_BINARY_DIR}/parser.cc)
set(YACC_HEADER ${CMAKE_BINARY_DIR}/parser.hh)



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
    MAIN_DEPENDENCY ${LEX_FILE} ${YACC_HEADER}
    COMMENT "Generando el analizador léxico (Flex)"
)

# Incluir los archivos generados como parte del proyecto
add_library(lexer_parser ${LEX_OUTPUT} ${YACC_OUTPUT})
target_include_directories(lexer_parser PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})
target_link_libraries(lexer_parser utils)

# Fuentes
file(GLOB SOURCES
"${CMAKE_SOURCE_DIR}/src/*.cc"
)


# Ejecutable
add_executable(marco_language ${SOURCES})
add_dependencies(marco_language lexer_parser)
target_link_libraries(marco_language lexer_parser utils)
target_include_directories(marco_language PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

# Archivos específicos a eliminar con make clean
add_custom_target(clean_test COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_SOURCE_DIR}/test/foo.cc)
add_custom_target(clean_all DEPENDS clean clean_test)