
#ifndef INTERPRETE_H
#define INTERPRETE_H
#include "utils.hh"
#include <string>
#include <fstream>
using namespace std;

class Interprete
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
    Interprete(const string &fichero_marco);
    void modo_verboso(bool modo) { MODO_VERBOSO = modo; }

    void ejecutar(const string &ejecutable, const string &variables_compilacion, const bool conservar_cpp);
private:

    /*************************************************************************/
    // De compilacion
    /*************************************************************************/
    void compilar_to_cpp(const string &FICHERO_CPP = NOMBRE_CPP_BASE);

    /*************************************************************************/
    // 
    /*************************************************************************/
};

#endif