
#ifndef INTERPRETE_H
#define INTERPRETE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <variant>
#include <vector>
#include <stack>
#include <exception>
#include <filesystem>
#include <array>


#include "utils.hh"
#include "tokenizador.hh"
#include "parser.tab.hh"
using namespace std;
using tipo_variable = variant<int, double, string>;
struct tipo_dato
{
    const char prefijo;
    // string view lo que hace es que no se copia el string, solo se guarda la referencia, es mas eficiente para strings que no se van a modificar
    const string nombre_completo;
    const tipo_variable valor_default;

    tipo_dato(char p, string n, tipo_variable v)
        : prefijo(p), nombre_completo(n), valor_default(v) {}
            
};

namespace Interprete
{

class Tipos_Datos
{
private:
    // Array es más eficiente que vector para tamaños fijos
    static const array<tipo_dato, 3> db_tipos;
    static const unordered_map<string, size_t> indice_prefijo;
    
public:
    // Con prefijo
    static size_t index(const char prefijo);
    static string nombre_completo(const char prefijo);
    static tipo_variable valor_default(const char prefijo);

    // Con indice
    static char prefijo(const size_t index);
    static string nombre_completo(const size_t index);
    static tipo_variable valor_default(const size_t index);

    // Con identificador
    static char prefijo(const string identificador);
    static size_t index(const string identificador);
    static string nombre_completo(const string identificador);
    static tipo_variable valor_default(const string identificador);

    // Comprobaciones
    static bool es_mismo_tipo(const tipo_variable & valor, const size_t index);
    static bool es_mismo_tipo(const tipo_variable & valor, const char prefijo_tipo);
    static bool es_mismo_tipo(const tipo_variable & valor, const string identificador);
};
class Interprete_Driver
{
private:
    Interprete::Interprete_Parser *parser = nullptr;
    Interprete::Interprete_Tokenizador *tokenizador = nullptr;

    // Aquí debo declarar el AST
    Interprete::Contexto contexto_actual;

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

// Clase para manejar el contexto y la tabla de símbolos
class Contexto
{
private:
    unordered_map<string, tipo_variable> tabla_simbolos_local;
    shared_ptr<Contexto> contexto_padre;
    const size_t profundidad;

public:
    // Uso shared_ptr para que se libere memoria cuando ya no se va a usar más un contexto (una función por ejemplo)
    Contexto(shared_ptr<Contexto> contexto_padre = nullptr)
    :   contexto_padre(contexto_padre),
        profundidad(contexto_padre ? contexto_padre->profundidad + 1 : 0)
    {}

    size_t get_profundidad() const { return profundidad; }

    // Agregar o actualizar una variable en el contexto actual
    void definir(const string& identificador, tipo_variable valor);

    // Obtener una variable (busca en el contexto actual y padres)
    tipo_variable valor_de(const string& identificador) const;

};

} // namespace Interprete
#endif