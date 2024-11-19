
#ifndef COMPILADOR_H
#define COMPILADOR_H
#include "utils.hpp"
#include <string>
#include <fstream>
using namespace std;

class Compilador
{
private:
    static constexpr const char* FICHERO_CPP = "foo.cc";
    const string nombre_fuente;
    bool MODO_DEBUG = false;


    ifstream fuente;
    string destino;

public:
    /*************************************************************************/
    // Funciones principales
    /*************************************************************************/
    Compilador(const string &fichero_marco);
    void modo_debug(bool modo) { MODO_DEBUG = modo; }

    // Compila directamente a ejecutable
    void compilar(const string &ejecutable, const string variables_compilacion, const bool conservar_cpp);
private:

    /*************************************************************************/
    // De compilacion
    /*************************************************************************/
    void compilar_to_cpp();
    void compilar_to_ejecutable(const string &ejecutable, const string variables_compilacion, bool conservar_cpp);

    /*************************************************************************/
    // De escribir cosas
    /*************************************************************************/
    void abrir_main();
    void cerrar_main();
};

#endif