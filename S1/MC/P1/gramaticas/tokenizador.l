%{
#include "utils.hpp"
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
.                   { printf(TEXTO_ROJO + "ERROR: Token desconocido: %s\n" + TEXTO_NORMAL, yytext); }

%%

int yywrap()
{ return 1; }