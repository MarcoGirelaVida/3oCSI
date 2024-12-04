#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
using namespace std;

// Colores para la terminal
constexpr const char* TEXTO_MORADO = "\033[1;35m";  // Para warnings / información
constexpr const char* TEXTO_VERDE = "\033[1;32m";   // Para mensajes de éxito
constexpr const char* TEXTO_ROJO = "\033[1;31m";    // Para mensajes de error
constexpr const char* TEXTO_AZUL = "\033[1;34m";    // Para configuraciones y anotaciones
constexpr const char* TEXTO_AMARILLO = "\033[1;33m";// Para errores sintácticos
constexpr const char* TEXTO_NORMAL = "\033[0m";
#endif 