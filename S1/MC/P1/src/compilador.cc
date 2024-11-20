#include "compilador.hh"
#include "utils.hh"
#include "parser.hh"

#include <filesystem>
#include <iostream>
#include <fstream>
using namespace std;

/*************************************************************************/
int yydebug = 1;
extern FILE *yyin;
extern int yyparse(); 

const string Compilador::NOMBRE_CPP_BASE = "foo.cc";
/*************************************************************************/
// Funciones principales
/*************************************************************************/
Compilador::Compilador(const string &fichero_marco) : fuente(fichero_marco), nombre_fuente(fichero_marco)
{
    // Abro fichero en lenguaje marco
    if (!fuente)
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir " << fichero_marco << TEXTO_NORMAL << endl;
        exit(1);
    }

    // Limpio el string destino
    destino.clear();
}

//Compilador::~Compilador(){}
void Compilador::compilar(const string &ejecutable, const string &variables_compilacion, const bool conservar_cpp)
{
    // Nombre el fichero_cpp en base a FICHERO_CPP_BASE y la ubicación del ejecutable
    const string FICHERO_CPP = filesystem::path(ejecutable).parent_path().string() + "/" + NOMBRE_CPP_BASE;
    compilar_to_cpp(FICHERO_CPP);

    /*****************************************************************************/
    // Compilación a ejecutable
    /*****************************************************************************/
    string comando = string("g++ ") + FICHERO_CPP + " -o " + variables_compilacion + ejecutable;
    system(comando.c_str());

    // Borro el archivo .cpp
    if (!conservar_cpp)
        filesystem::remove(FICHERO_CPP);

    if (MODO_VERBOSO)
        cerr << TEXTO_VERDE << "COMPILACION DE ["<< FICHERO_CPP << "] A EJECUTABLE  [" << ejecutable << "] COMPLETADA" << TEXTO_NORMAL << endl;
}

/*************************************************************************/
// Funciones de compilacion
/*************************************************************************/
void Compilador::compilar_to_cpp(const string &FICHERO_CPP)
{
    /*****************************************************************************/
    // Administracion de ficheros
    /*****************************************************************************/
    // Abro o creo fichero .cpp destino
    // El nombre del cpp será siempre igual ya que realmente es transparente al usuario
    ofstream pipe_cpp(FICHERO_CPP);
    if (!pipe_cpp)
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir/crear el fichero " << FICHERO_CPP << TEXTO_NORMAL << endl;
        exit(1);
    }

    /*****************************************************************************/
    // Compilador
    /*****************************************************************************/


    /*****************************************************************************/
    if (MODO_VERBOSO)
        cerr << TEXTO_VERDE << "COMPILACIÓN DEL FICHERO [" << nombre_fuente << "] COMPLETADA" << TEXTO_NORMAL << endl;
}

/*************************************************************************/
// Funciones de escribir cosas
/*************************************************************************/
void Compilador::abrir_main()
{
    destino += "#include <iostream>\n";
    destino += "using namespace std;\n";
    destino += "int main()\n";
    destino += "{\n";
}

void Compilador::cerrar_main()
{
    destino += "}\n";
}
/*************************************************************************/
// 
/*************************************************************************/


/*************************************************************************/
// 
/*************************************************************************/


/*************************************************************************/
// 
/*************************************************************************/