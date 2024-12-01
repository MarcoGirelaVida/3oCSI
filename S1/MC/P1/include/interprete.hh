
#ifndef INTERPRETE_H
#define INTERPRETE_H
#include <istream>
#include <filesystem>
#include <iostream>

#include "utils.hh"
#include "tokenizador.hh"
#include "parser.tab.hh"
using namespace std;

namespace Interprete
{
class Interprete_Driver
{
private:
    Interprete::Interprete_Parser *parser = nullptr;
    Interprete::Interprete_Tokenizador *tokenizador = nullptr;

    // Aquí debo declarar el AST

    bool MODO_VERBOSO = false;

public:
    /*************************************************************************/
    // Funciones principales
    /*************************************************************************/
    Interprete_Driver() = default;
    virtual ~Interprete_Driver();

    void set_modo_verboso(bool modo) { MODO_VERBOSO = modo; }

    void ejecutar(const string &fichero_fuente, const bool linea_a_linea);
    void ejecutar(istream &input_puente, const bool linea_a_linea);

private:
    /*************************************************************************/
    // De lectura
    /*************************************************************************/
    void leer_fuente(const string &fichero_fuente);
    void leer_input(istream &in_fuente);


    /*************************************************************************/
    // De compilacion
    /*************************************************************************/
    //void compilar_to_cpp(const string &FICHERO_CPP = NOMBRE_CPP_BASE, const string &ejecutable, const string &variables_compilacion, const bool conservar_cpp);

};
} // namespace Interprete
#endif