#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "utils.hpp"
#include "compilador.hpp"
using namespace std;

void mostrar_ayuda()
{
    const unsigned char ancho_columna = 30;
    cout << "USO: \"marco_compiler fuente.marco ejecutable [OPCIONES]\"" << TEXTO_NORMAL << endl;
    cout << left;
    cout << setw(ancho_columna) << "  fuente.marco" << "Fichero en lenguaje marco que desea compilar" << TEXTO_NORMAL << endl;
    cout << setw(ancho_columna) << "  ejecutable" << "Nombre del ejecutable que se desea obtener" << TEXTO_NORMAL << endl;
    cout << endl;
    cout << "OPCIONES:" << TEXTO_NORMAL << endl;
    cout << left;
    cout << setw(ancho_columna)<< "  -debug"                        << "Activar el modo debug" << TEXTO_NORMAL << endl;
    cout << setw(ancho_columna)<< "  -h/--help"                     << "Mostrar esta ayuda" << TEXTO_NORMAL << endl;
    cout << setw(ancho_columna)<< "  -cpp"                          << "Conservar el fichero intermedio \"./foo.cc\" al compilar" << TEXTO_NORMAL << endl;
    cout << setw(ancho_columna)<< "  -opcion_cpp <opcion_de_c++>"   << "Opciones de compilación de c++ (-Og, --Wall...)" << TEXTO_NORMAL << endl;
    cout << TEXTO_NORMAL << endl;
    cout << "EJEMPLOS:" << TEXTO_NORMAL << endl;
    cout << "  Uso básico:"     << "\tmarco_compiler mi_programa.marco mi_ejecutable" << TEXTO_NORMAL << endl;
    cout << "  Uso avanzado:"   << "\tmarco_compiler mi_programa.marco mi_ejecutable -debug -cpp -opcion_cpp: -Og -opcion_cpp: --Wall" << TEXTO_NORMAL << endl;
}

int main(int argc, char *argv[])
{
    /*****************************************************************************/
    // Procesamiento de argumentos
    /*****************************************************************************/
    const unsigned char num_args_obligatorios = 3;
    const unsigned char num_args_opcionales = 4;
    if (argc < num_args_obligatorios or argc > (num_args_obligatorios + num_args_opcionales))
    {
        cerr << TEXTO_ROJO << "ERROR: Argumentos incorrectos" << TEXTO_NORMAL << endl;
        mostrar_ayuda();
        exit(1);
    }
    unsigned char arg_i = 1;


    // Procesamiento de argumentos obligatorios
    // Compruebo que fichero marco tiene la extensión .marco
    const string fichero_marco = argv[arg_i++];
    if (fichero_marco.substr(fichero_marco.find_last_of(".") + 1) != "marco")
    {
        cerr << TEXTO_ROJO << "ERROR: El fichero a compilar [" << fichero_marco << "] no tiene extensión .marco" << TEXTO_NORMAL << endl;
        exit(1);
    }
    const string ejecutable = argv[arg_i++];

    // Procesamiento de argumentos opcionales
    bool modo_debug = false;
    bool conservar_cpp = true;
    string argumentos_compilacion_cpp;
    while (arg_i < argc)
    {
        const string arg_i_str = string(argv[arg_i]);
        if (arg_i_str == "-debug")
        {
            cerr << "MODO DEBUG ACTIVADO" << TEXTO_NORMAL << endl;
            modo_debug = true;
            arg_i++;
        }
        else if (arg_i_str == "-cpp")
        {
            conservar_cpp = false;
            if (modo_debug)
                cerr << "CONSERVAR FICHERO INTERMEDIO CPP ACTIVADO" << TEXTO_NORMAL << endl;
        }
        else if (arg_i_str == "-h" or arg_i_str == "--help")
        {
            mostrar_ayuda();
            exit(0);
        }
        else if (arg_i_str == "-opcion_cpp")
        {
            if (arg_i == argc - 1)
            {
                cerr << TEXTO_ROJO << "ERROR: El argmuento [-opcion_cpp] no puede ir solo, necesita una <opcion_de_c++>" << TEXTO_NORMAL << endl;
                mostrar_ayuda();
                exit(1);
            }

            argv++;
            if (modo_debug)
                cerr << "ARGUMENTO DE COMPILACION C++ AÑADIDO: " << argv[arg_i] << TEXTO_NORMAL << endl;
            argumentos_compilacion_cpp += string(argv[arg_i]) + " ";
        }
        else
        {
            cerr << TEXTO_ROJO << "ERROR: Argumento opcional [" << arg_i_str << "] no reconocido" << TEXTO_NORMAL << endl;
            mostrar_ayuda();
            exit(1);
        }
        arg_i++;
    }
    //if (!modo_ejecutable and !argumentos_compilacion_cpp.empty())
    //    cerr << TEXTO_MORADO << "WARNING: Está poniendo argumentos de compilación c++ pero no está compilando a ejecutable, por tanto estos no tendrán efecto" << TEXTO_NORMAL << endl;

    /*****************************************************************************/
    // Compilación a c++
    /*****************************************************************************/
    
    Compilador compilador(fichero_marco);
    compilador.modo_debug(modo_debug);
    compilador.compilar(ejecutable, argumentos_compilacion_cpp, conservar_cpp);

    return 0;
}