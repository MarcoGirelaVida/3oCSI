cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(BuscadorPlagiosMusicales)
find_package(FLEX REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Directorio de encabezados
include_directories(include)

# Archivos fuente
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cc")
set(LEX_FILE "${CMAKE_SOURCE_DIR}/src/buscador_plagios.l")
FLEX_TARGET(Lexico ${LEX_FILE} ${CMAKE_BINARY_DIR}/lex.yy.cc COMPILE_FLAGS -+)
#add_flex_files(${FLEX_Lexico_OUTPUTS})
set_source_files_properties(${FLEX_Lexico_OUTPUTS} PROPERTIES GENERATED TRUE)
list(APPEND SOURCES ${FLEX_Lexico_OUTPUTS})

# Ejecutable
add_executable(buscador_plagios ${SOURCES})
#target_link_libraries(buscador_plagios ${FLEX_LIBRARIES})

# Archivos específicos a eliminar con make clean
set(CANCIONES_TOKENIZADAS ${CMAKE_SOURCE_DIR}/canciones/*_tokens.txt)
add_custom_target(clean_extra COMMAND ${CMAKE_COMMAND} -E rm -f ${CANCIONES_TOKENIZADAS})
add_custom_target(clean_all DEPENDS clean clean_extra)