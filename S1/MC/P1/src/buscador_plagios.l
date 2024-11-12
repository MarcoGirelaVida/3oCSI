%{
#include <iostream>
#include <fstream>
#include "tokenizador_wav.hpp"
using namespace std;
%}
%option noyywrap

%%
%%

int main(int argc, char *argv[])
{
    /*****************************************************************************/
    // Administracion de ficheros y argumentos
    /*****************************************************************************/
    if (argc != 3)
    {
        cerr << "ERROR: Los argumentos son incorrectos, deben ser: buscador_plagios [cancion_1] [cancion_2]" << endl;
        exit(1);
    }
    string cancion_1 = tokenizar_cancion(argv[1]);
    string cancion_2 = tokenizar_cancion(argv[2]);

    // Abro la cancion 1
    ifstream c1(cancion_1);
    if (!c1)
    {
        cerr << "ERROR: No se pudo abrir " << cancion_1 << endl;
        exit(1);
    }
    // Abro la cancion 2
    ifstream c2(cancion_2);
    if (!c2)
    {
        cerr << "ERROR: No se pudo abrir " << cancion_2 << endl;
        exit(1);
    }

    /*****************************************************************************/
    // Analizador de plagios
    /*****************************************************************************/

    return 0;
}