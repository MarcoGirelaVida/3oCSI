#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
using namespace std;

// Colores para la terminal
constexpr const char* TEXTO_MORADO = "\033[1;35m";
constexpr const char* TEXTO_VERDE = "\033[1;32m";
constexpr const char* TEXTO_ROJO = "\033[1;31m";
constexpr const char* TEXTO_AZUL = "\033[1;34m";
constexpr const char* TEXTO_AMARILLO = "\033[1;33m";
constexpr const char* TEXTO_NORMAL = "\033[0m";

struct nodo_arbol_sintactico
{
    string tipo;    // Tipo del nodo
    string valor;   // Valor del nodo (si es que tiene)
    size_t nivel;   // Nivel del nodo
};

// Estructura del árbol sintáctico
struct ArbolSintactico
{
    vector<nodo_arbol_sintactico> nodes; // Lista de nodos
};

#endif