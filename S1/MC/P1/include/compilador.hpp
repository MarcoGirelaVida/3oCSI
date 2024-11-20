
#ifndef COMPILADOR_H
#define COMPILADOR_H
#include "utils.hpp"
#include <string>
#include <fstream>
using namespace std;

class Compilador
{
private:
    static const string NOMBRE_CPP_BASE;
    const string nombre_fuente;
    bool MODO_VERBOSO = false;


    ifstream fuente;
    string destino;

public:
    /*************************************************************************/
    // Funciones principales
    /*************************************************************************/
    Compilador(const string &fichero_marco);
    void modo_verboso(bool modo) { MODO_VERBOSO = modo; }

    // Compila directamente a ejecutable
    void compilar(const string &ejecutable, const string &variables_compilacion, const bool conservar_cpp);
private:

    /*************************************************************************/
    // De compilacion
    /*************************************************************************/
    void compilar_to_cpp(const string &FICHERO_CPP = NOMBRE_CPP_BASE);

    /*************************************************************************/
    // De escribir cosas
    /*************************************************************************/
    void abrir_main();
    void cerrar_main();
};

#endif