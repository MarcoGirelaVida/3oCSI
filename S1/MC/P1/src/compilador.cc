#include "compilador.hpp"
#include "utils.hpp"

#include <iostream>
#include <fstream>
using namespace std;

/*************************************************************************/
// Funciones principales
/*************************************************************************/
Compilador::Compilador(const string &fichero_marco) : fuente(fichero_marco), nombre_fuente(fichero_marco)
{
    // Abro fichero en lenguaje marco
    if (!fuente)
    {
        cerr << "ERROR: No se pudo abrir " << fichero_marco << endl;
        exit(1);
    }

    // Limpio el string destino
    destino.clear();
}

//Compilador::~Compilador(){}
void Compilador::compilar(const string &ejecutable, const string variables_compilacion, const bool conservar_cpp)
{
    compilar_to_cpp();
    compilar_to_ejecutable(ejecutable, variables_compilacion, conservar_cpp);

    if (MODO_DEBUG)
        cerr << "COMPILACIÓN DEL FICHERO DE FORMA DIRECTA [" << nombre_fuente << "] COMPLETADA" << endl;
}

/*************************************************************************/
// Funciones de compilacion
/*************************************************************************/
void Compilador::compilar_to_cpp()
{
    /*****************************************************************************/
    // Administracion de ficheros
    /*****************************************************************************/
    // Abro o creo fichero .cpp destino
    // El nombre del cpp será siempre igual ya que realmente es transparente al usuario
    ofstream pipe_cpp(FICHERO_CPP);
    if (!pipe_cpp)
    {
        cerr << "ERROR: No se pudo abrir/crear el fichero " << FICHERO_CPP << endl;
        exit(1);
    }

    /*****************************************************************************/
    // Compilador
    /*****************************************************************************/


    /*****************************************************************************/
    if (MODO_DEBUG)
        cerr << "COMPILACIÓN DEL FICHERO [" << nombre_fuente << "] COMPLETADA" << endl;
}

void Compilador::compilar_to_ejecutable(const string &ejecutable,const string variables_compilacion, const bool conservar_cpp)
{
    /*****************************************************************************/
    // Compilación a ejecutable
    /*****************************************************************************/
    string comando = string("g++ ") + FICHERO_CPP + " -o " + variables_compilacion + ejecutable;
    system(comando.c_str());

    // Borro el archivo .cpp
    if (!conservar_cpp)
        remove(FICHERO_CPP);

    if (MODO_DEBUG)
        cerr << "Compilación de c++ a ejecutable: " << ejecutable << " completada" << endl;
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