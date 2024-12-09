
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
#include <list>
#include <queue>
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


class Nodo_AST : public enable_shared_from_this<Nodo_AST>
{
public:
    weak_ptr<Nodo_AST> padre;  
    list<shared_ptr<Nodo_AST>> hijo;
    tipo_variable valor;

public:
    virtual ~Nodo_AST() = default;

    void add_hijo(const shared_ptr<Nodo_AST>& nuevo_hijo)
    {
        nuevo_hijo->padre = shared_from_this();
        hijo.push_back(nuevo_hijo);
    }

    virtual string to_string() const = 0;
};


// Clase para manejar el contexto y la tabla de símbolos
class Contexto
{
private:
    unordered_map<string, tipo_variable> tabla_simbolos_local;
    unordered_map<string, queue<Nodo_AST>> funciones;
    shared_ptr<Contexto> contexto_padre;
    const size_t profundidad;

public:
    // Uso shared_ptr para que se libere memoria cuando ya no se va a usar más un contexto (una función por ejemplo)
    Contexto(shared_ptr<Contexto> contexto_padre = nullptr)
    :   contexto_padre(contexto_padre),
        profundidad(contexto_padre != nullptr ? contexto_padre->profundidad + 1 : 0)
    {}
 
    size_t get_profundidad() const { return profundidad; }

    // Añadir una variable al contexto actual
    void definir(const string& identificador, tipo_variable valor);

    // Añadir una función al contexto actual
    void definir_funcion(const string& identificador, Nodo_AST& funcion);


    // Obtener una variable (busca en el contexto actual y padres)
    tipo_variable valor_de(const string& identificador) const;

    shared_ptr<Contexto> push_contexto()
    {
        return make_shared<Contexto>(Contexto(*this));
    }

    shared_ptr<Contexto> pop_contexto()
    {
        if (contexto_padre == nullptr)
        {
            throw runtime_error("Error: No se puede hacer más pops");
        }
        return contexto_padre;
    }
};

class Interprete_Driver
{
private:
    unique_ptr<Interprete::Interprete_Parser> parser;
    unique_ptr<Interprete::Interprete_Tokenizador> tokenizador;

    // Aquí debo declarar el AST
    shared_ptr<Contexto> contexto_actual;

    bool MODO_VERBOSO = false;

public:
    /*************************************************************************/
    // Funciones principales
    /*************************************************************************/
    Interprete_Driver()
    :   parser(nullptr),
        tokenizador(nullptr),
        contexto_actual(make_shared<Contexto>())
    {}
    virtual ~Interprete_Driver() = default;

    void set_modo_verboso(bool modo) { MODO_VERBOSO = modo; }

    void ejecutar(const string &fichero_fuente, const bool linea_a_linea);
    void ejecutar(istream &input_puente, const bool linea_a_linea);

    void push_contexto()
    {
        contexto_actual = contexto_actual->push_contexto();
    }

    void pop_contexto()
    {
        contexto_actual = contexto_actual->pop_contexto();
    }

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