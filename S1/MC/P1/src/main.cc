#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "interprete.hh"
using namespace std;

void mostrar_ayuda()
{
    const unsigned char ancho_columna = 30;
    cout << TEXTO_MORADO << "USO: \"marco_language [fuente.marco] [OPCIONES]\"" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << endl;
    cout << TEXTO_MORADO << "----------OPCIONES----------"  << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << "GENERALES:"           << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << left;
    cout << TEXTO_MORADO << setw(ancho_columna) << "  fuente.marco"             << "Fichero en lenguaje marco a ejecutar" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< ""                            << "IMPORTANTE: Si no se provee, se leera de la terminal" << TEXTO_NORMAL << endl;
    //cout << TEXTO_MORADO << setw(ancho_columna)<< "  --test"                    << "Modo test, activa todas las opciones" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  --verboso"                 << "Activar el modo debug" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  -h/--help"                 << "Mostrar esta ayuda" << TEXTO_NORMAL << endl;
    cout << endl;
    cout << TEXTO_MORADO << "DE INTERPRETACIÓN:" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << left;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  --linea_a_linea"           << "Activar el modo de ejecución linea a linea" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  --mostrar_AST"             << "Muestra el arbol sintáctico conforme se ejecuta el programa" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< ""                            << "(Recomendable combinar con \"--linea_a_linea\")" << TEXTO_NORMAL << endl;
    cout << endl;
    cout << TEXTO_MORADO << "DE COMPILACIÓN y TRADUCCIÓN:" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << TEXTO_AMARILLO << "CUIDADO: Si usa entrada por terminal la compilación/traducción se aplicará tras hacer Ctrl+D"    << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << left;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  --compilar <ejecutable>"   << "Compila \"fichero.marco\" a \"ejecutable\""     << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  --traducir <fichero_cpp>"  << "El programa \"fichero.marco\" a C++"   << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  --interpretar_tambien"     << "Además de compilar y/o traducir, el programa se interpretará"    << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< ""                            << "Con esta opción también podrá usar las opciones del intérprete"    << TEXTO_NORMAL << endl;
    //cout << setw(ancho_columna)<< "  --opcion_cpp <opcion_de_c++>"   << "Opciones de compilación de c++ (-Og, --Wall...)"     << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << endl;
    cout << TEXTO_MORADO << "----------EJEMPLOS DE USO----------" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << left;
    cout << TEXTO_MORADO << "Como Intérprete" << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  Desde terminal básico:"  << "marco_language" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  Desde terminal completo:"<< "marco_language --linea_a_linea --mostrar_AST --verboso" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  Desde fichero básico:"   << "marco_language programa.marco" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  Desde fichero completo:" << "marco_language programa.marco --linea_a_linea --mostrar_AST --verboso" << TEXTO_NORMAL << endl;
    cout << endl;
    cout << TEXTO_MORADO << "Usos Alternativos" << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  Como compilador:"          << "marco_language programa.marco --compilar output.bin --verboso" << TEXTO_NORMAL << endl;
    cout << TEXTO_MORADO << setw(ancho_columna)<< "  Como traductor a c++:"     << "marco_language programa.marco --traducir output.cc --verboso" << TEXTO_NORMAL << endl;
    cout << endl;
    cout << TEXTO_MORADO << "USO COMPLETO" << endl;
    cout << TEXTO_MORADO << "marco_language programa.marco --linea_a_linea --mostrar_AST --interpretar_tambien --compilar output.bin --traducir output.cc --verboso" << TEXTO_NORMAL << endl;
}

int main(int argc, char *argv[])
{
    /*****************************************************************************/
    // Procesamiento de argumentos
    /*****************************************************************************/
    const unsigned char num_args_obligatorios = 2;
    const unsigned char num_args_opcionales = 10;
    if (argc < num_args_obligatorios or argc > (num_args_obligatorios + num_args_opcionales))
    {
        cerr << TEXTO_ROJO << "ERROR: Número de argumentos incorrectos" << TEXTO_NORMAL << endl;
        for (int i = 0; i < argc; i++)
        {
            cerr << "[" << i << "] = " << argv[i] << endl;
        }
        
        mostrar_ayuda();
        exit(1);
    }
    unsigned char arg_i = 1;


    // Procesamiento de argumentos obligatorios
    bool leer_de_fichero = true;
    string fichero_fuente;
    if (argv[arg_i][0] == '-')
    {
        cerr << TEXTO_AZUL << "NO SE HA PROPORCIONADO FICHERO.MARCO, SE LEERA DE TERMINAL" << TEXTO_NORMAL << endl;
        leer_de_fichero = false;
    }
    else 
    {
        // Compruebo que fichero marco tiene la extensión .marco
        fichero_fuente = argv[arg_i++];
        if (fichero_fuente.substr(fichero_fuente.find_last_of(".") + 1) != "marco")
        {
            cerr << TEXTO_ROJO << "ERROR: El fichero marco [" << fichero_fuente << "] no tiene extensión .marco" << TEXTO_NORMAL << endl;
            exit(1);
        }
    }

    // Procesamiento de argumentos opcionales
    bool modo_verboso = false;
    bool mostrar_arbol_sintactico = false;
    bool modo_linea_a_linea = false;
    bool compilar_a_ejecutable = false;
    bool traducir_a_cpp = false;
    bool ejecutar_tambien = false;
    string cpp_destino, ejecutable_destino;
    //string argumentos_compilacion_cpp;
    while (arg_i < argc)
    {
        const string arg_i_str = string(argv[arg_i]);
        if (arg_i_str == "--test")
        {
            modo_verboso = mostrar_arbol_sintactico = modo_linea_a_linea = compilar_a_ejecutable = traducir_a_cpp = ejecutar_tambien = true;
            cpp_destino = "test.cpp";
            ejecutable_destino = "test.bin";
            cerr << TEXTO_AZUL << "MODO TEST ACTIVADO" << TEXTO_NORMAL << endl;
            break;
        }
        else if (arg_i_str == "--verboso")
        {
            modo_verboso = true;
            cerr << TEXTO_AZUL << "MODO VERBOSO ACTIVADO" << TEXTO_NORMAL << endl;
        }
        else if (arg_i_str == "-h" or arg_i_str == "--help")
        {
            mostrar_ayuda();
            exit(0);
        }
        else if (arg_i_str == "--linea_a_linea")
        {
            modo_linea_a_linea = true;
            if (modo_verboso)
                cerr << TEXTO_AZUL << "MODO LINEA A LINEA ACTIVADO" << TEXTO_NORMAL << endl;
        }
        else if (arg_i_str == "--mostrar_AST")
        {
            mostrar_arbol_sintactico = true;
            if (modo_verboso)
                cerr << TEXTO_AZUL << "MODO MOSTRAR ARBOL SINTACTICO ACTIVADO" << TEXTO_NORMAL << endl;
        }
        else if (arg_i_str == "--compilar")
        {
            compilar_a_ejecutable = true;
            if (arg_i == argc - 1 or argv[arg_i + 1][0] == '-')
            {
                cerr << TEXTO_ROJO << "ERROR: El argmuento [--compilar] necesita un <ejecutable>" << TEXTO_NORMAL << endl;
                mostrar_ayuda();
                exit(1);
            }
            else
                ejecutable_destino = argv[++arg_i];

            if (modo_verboso)
                cerr << TEXTO_AZUL << "MODO COMPILADOR ACTIVADO" << TEXTO_NORMAL << endl;
        }
        else if (arg_i_str == "--traducir")
        {
            traducir_a_cpp = true;
            if (arg_i == argc - 1 or argv[arg_i + 1][0] == '-')
            {
                cerr << TEXTO_ROJO << "ERROR: El argmuento [--traducir] necesita un <nombre_cpp>" << TEXTO_NORMAL << endl;
                mostrar_ayuda();
                exit(1);
            }
            cpp_destino = argv[++arg_i];
            string terminacion = cpp_destino.substr(cpp_destino.find_last_of(".") + 1);
            if (terminacion != "cc" and terminacion != "cpp" and terminacion != "cxx" and terminacion != "c++" and terminacion != "C" and terminacion != "c")
            {
                cerr << TEXTO_ROJO << "ERROR: El proporcionado para la traducción [" << cpp_destino << "] no tiene extensión de c++" << TEXTO_NORMAL << endl;
                exit(1);
            }
            if (modo_verboso)
                cerr << TEXTO_AZUL << "TRADUCIR A C++ ACTIVADO" << TEXTO_NORMAL << endl;
        }
        else if (arg_i_str == "--interpretar_tambien")
        {
            ejecutar_tambien = true;
            if (modo_verboso)
                cerr << TEXTO_AZUL << "INTERPRETAR ADEMAS DE TRADUCIR/COMPILAR ACTIVADO" << TEXTO_NORMAL << endl;
        }
        /*
        else if (arg_i_str == "--opcion_cpp")
        {
            if (arg_i == argc - 1 or argv[arg_i + 1][0] == '-')
            {
                cerr << TEXTO_ROJO << "ERROR: El argmuento [-opcion_cpp] no puede ir solo, necesita una <opcion_de_c++>" << TEXTO_NORMAL << endl;
                mostrar_ayuda();
                exit(1);
            }

            argv++;
            if (modo_verboso)
                cerr << TEXTO_AZUL << "ARGUMENTO DE COMPILACION C++ AÑADIDO: " << argv[arg_i] << TEXTO_NORMAL << endl;
            argumentos_compilacion_cpp += string(argv[arg_i]) + " ";
        }
        */
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
    // Ejecución
    /*****************************************************************************/
    
    Interprete::Interprete_Driver interprete;
    interprete.set_modo_verboso(modo_verboso);
    /*
    if (compilar_a_ejecutable)
    {
        if (leer_de_fichero)
            interprete.compilar(fichero_fuente, ejecutable_destino);
        else
            interprete.compilar(cin, ejecutable_destino);
        //interprete.compilar_to_cpp(argv[2], argv[3], argumentos_compilacion_cpp, false);
    }
    if (traducir_a_cpp)
    {
        if (leer_de_fichero)
            interprete.traducir_a_cpp(fichero_fuente, cpp_destino);
        else
            interprete.traducir_a_cpp(cin, cpp_destino);
        //interprete.compilar_to_cpp(argv[2], NOMBRE_CPP_BASE, argumentos_compilacion_cpp, true);
    }
    */
    if ((!compilar_a_ejecutable and !traducir_a_cpp) or ejecutar_tambien)
    {
        if (leer_de_fichero)
            interprete.ejecutar(fichero_fuente, modo_linea_a_linea);
        else
            interprete.ejecutar(cin, modo_linea_a_linea);
    }
    return 0;
}