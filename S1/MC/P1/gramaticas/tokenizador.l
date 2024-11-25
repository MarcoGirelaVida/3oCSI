%{
#include "utils.hh"
#include <iostream>
#include "parser.hh"
%}


%%


[0-9]+                          { return NUMERO; }
[a-zA-Z_]+                      { return PALABRA; }
"int"|"char"|"float"|"double"   { return TIPO; }
=                               { return ASIGNACION; }
;                               { return TERMINAL; }
[+\-*]                          { return OPERADOR; }
[({]                            { return ABRE_SECCION; }
[)}]                            { return CIERRA_SECCION; }
"inicio_programa_marco"                         { return APERTURA_PROGRAMA; }
"fin_programa_marco"                            { return TERMINAL_PROGRAMA; }
[ \t\n]+                        { /* Ignoro espacios y saltos de línea */ }
.                               { cerr << TEXTO_ROJO << "ERROR: Token desconocido: " << TEXTO_NORMAL << endl; }

%%

int yywrap()
{ return 1; }