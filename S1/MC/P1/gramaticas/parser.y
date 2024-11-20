%{

#include "utils.hh"
#include <iostream>

extern int yylex();
void yyerror(const char* msg)
{
    cerr << TEXTO_AMARILLO << "ERROR: Token de sintaxis: " << msg << TEXTO_NORMAL << endl;
}
%}
%debug

%token NUMERO PALABRA TIPO OPERADOR ABRE_SECCION CIERRA_SECCION ASIGNACION TERMINAL APERTURA_PROGRAMA TERMINAL_PROGRAMA

%%

start : APERTURA_PROGRAMA cuerpo TERMINAL_PROGRAMA
    {
        std::cout << "Programa analizado correctamente" << std::endl;
    }
    ;

cuerpo : instruccion
       | cuerpo instruccion
       ;

instruccion : TIPO PALABRA ASIGNACION expresion TERMINAL
            | PALABRA OPERADOR PALABRA TERMINAL
            ;

expresion : NUMERO
          | PALABRA
          ;

%%