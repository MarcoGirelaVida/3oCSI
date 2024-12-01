#include "interprete.hh"
#include <fstream>
using namespace std;
#include <cstring>
#include <sstream>

//const string Interprete::Interprete_Driver::NOMBRE_CPP_BASE = "foo.cc";
/*************************************************************************/
// Funciones principales
/*************************************************************************/

Interprete::Interprete_Driver::~Interprete_Driver()
{
    delete(parser);
    parser = nullptr;
    delete(tokenizador);
    tokenizador = nullptr;
}

void
Interprete::Interprete_Driver::leer_fuente(const string &fichero_fuente)
{
    ifstream input_fuente(fichero_fuente);
    /*
    if( !input_fuente.good() )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir el fichero [" << fichero_fuente << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }
    */
    leer_input(input_fuente);
}

void
Interprete::Interprete_Driver::leer_input(istream &input_fuente)
{
    /*
    if (!input_fuente.good() and input_fuente.eof())
    {
        cerr << TEXTO_ROJO << "ERROR: Error: " << strerror(errno) << " en el input del fuente" << TEXTO_NORMAL << endl;
        exit(EXIT_FAILURE);
    }
    */
    // Notese que si hubiese puento !input_fuente.good() fallará si se le proporciona un input vacio (EOF)
    // porque lo considera un error, es por eso que he puesto que falle si fail o bad

    // Intento crear el tokenizador
    delete(tokenizador);
    try
    {
        tokenizador = new Interprete::Interprete_Tokenizador( &input_fuente );
    }
    catch( bad_alloc &ba )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo crear el tokenizador: [" << ba.what() << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }

    // Intento crear el parser
    delete(parser); 
    try
    {
        parser = new Interprete::Interprete_Parser((*tokenizador), (*this));
    }
    catch( bad_alloc &ba )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo crear el Parser: [" << ba.what() << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }

    // Ejecuto el parser (si no devuelve 0 es que ha habido un error)
    if(parser->parse())
    {
        cerr << TEXTO_ROJO << "ERROR: Al ejecutar el parser" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }
}

void
Interprete::Interprete_Driver::ejecutar(const string &fichero_fuente, const bool linea_a_linea)
{
    ifstream input_fuente(fichero_fuente);
    /*
    if( ! input_fuente.good() )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir el fichero [" << fichero_fuente << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }
    */
    ejecutar(input_fuente, linea_a_linea);
}

void
Interprete::Interprete_Driver::ejecutar(istream &input_fuente, const bool linea_a_linea)
{
    if (!input_fuente.good() and input_fuente.eof())
    {
        cerr << TEXTO_ROJO << "ERROR: Error: " << strerror(errno) << " en el input del fuente" << TEXTO_NORMAL << endl;
        exit(EXIT_FAILURE);
    }

    if (linea_a_linea)
    {
        // Leer linea a linea
        string linea;
        while (getline(input_fuente, linea))
        {
            istringstream input_linea(linea); // Leo la linea
            leer_input(input_linea);
            // Ejecutar la linea
            // ...
            // Duermo hasta que me den la orden de seguir (\n)
            do
            {
                cerr << TEXTO_AZUL << "Pulse [ENTER] para continuar" << TEXTO_NORMAL << endl;
            }
            while (cin.get() != '\n');
        }
    }
    else
    {
        // Leer todo el input
        leer_input(input_fuente);
        // Ejecutar todo el input
        // ...
    }
}


/*************************************************************************/
// Funciones de compilacion
/*************************************************************************/
/*
void
Interprete::Interprete_Driver::compilar_to_cpp(const string &FICHERO_CPP, const string &ejecutable, const string &variables_compilacion, const bool conservar_cpp)
{
    // Nombre el fichero_cpp en base a FICHERO_CPP_BASE y la ubicación del ejecutable
    const string FICHERO_CPP = filesystem::path(ejecutable).parent_path().string() + "/" + NOMBRE_CPP_BASE;
    compilar_to_cpp(FICHERO_CPP);

    // Compilación a ejecutable
    string comando = string("g++ ") + FICHERO_CPP + " -o " + variables_compilacion + ejecutable;
    system(comando.c_str());

    // Borro el archivo .cpp
    if (!conservar_cpp)
        filesystem::remove(FICHERO_CPP);

    if (MODO_VERBOSO)
        cerr << TEXTO_VERDE << "COMPILACION DE ["<< FICHERO_CPP << "] A EJECUTABLE  [" << ejecutable << "] COMPLETADA" << TEXTO_NORMAL << endl;
    // El nombre del cpp será siempre igual ya que realmente es transparente al usuario
    ofstream pipe_cpp(FICHERO_CPP);
    if (!pipe_cpp)
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir/crear el fichero " << FICHERO_CPP << TEXTO_NORMAL << endl;
        exit(1);
    }

    if (MODO_VERBOSO)
        cerr << TEXTO_VERDE << "COMPILACIÓN DEL FICHERO [" << nombre_fuente << "] COMPLETADA" << TEXTO_NORMAL << endl;
}
*/
/*************************************************************************/
// 
/*************************************************************************/

/*************************************************************************/
// 
/*************************************************************************/


/*************************************************************************/
// 
/*************************************************************************/


/*************************************************************************/
// 
/*************************************************************************/