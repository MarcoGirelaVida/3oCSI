#include "tokenizador_wav.hpp"
#include <iostream>
#include <fstream>
using namespace std;

string tokenizar_cancion(const string &cancion)
{
    /*****************************************************************************/
    // Administracion de ficheros
    /*****************************************************************************/

    // Abro el archivo de la canción
    ifstream c(cancion);
    if (!c)
    {
        cerr << "ERROR: No se pudo abrir " << cancion << endl;
        exit(1);
    }

    // Creo el archivo donde se almacenará la canción decodificada en tokens
    size_t pos_punto = cancion.find_last_of(".");
    const string cancion_tokens_nombre = ((pos_punto == string::npos) ? cancion : cancion.substr(0, pos_punto)) + "_tokens.txt";
    ofstream cancion_tokens(cancion_tokens_nombre);
    if (!cancion_tokens)
    {
        cerr << "ERROR: No se pudo abrir/crear el fichero " << cancion_tokens_nombre << endl;
        exit(1);
    }

    /*****************************************************************************/
    // Tokenización de Canción
    /*****************************************************************************/




    /*****************************************************************************/
    cerr << "TOKENIZACIÓN DE LA CANCIÓN [" << cancion << "] COMPLETADA" << endl;
    return cancion_tokens_nombre;
}