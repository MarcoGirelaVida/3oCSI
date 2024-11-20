%{
#include "utils.hpp"
#include <iostream>
#include "y.tab.h" // Para tener los tokens declarados en yacc
%}

NUMERO          [0-9]+
PALABRA         [a-zA-Z_]+
TIPO            [int|char|float|double]
OPERADOR        [+-*]
ABRE_SECCION    [({]
CIERRA_SECCION  [)}]
ASIGNACION      [=]
TERMINAL        [;]

%%


[ \t\n]+            { /* Ignoro espacios y saltos de línea */ }
.                   { cerr << TEXTO_ROJO << "ERROR: Token desconocido: " << TEXTO_NORMAL << endl; }

%%

int yywrap()
{ return 1; }