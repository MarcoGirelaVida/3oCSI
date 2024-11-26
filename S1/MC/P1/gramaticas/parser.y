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
%union {
    char tipo_dato;
}

%token <tipo_dato> ID_VARIABLE NUMERO
%token LOGIC_ASIGNACION LOGIC_CIERRA_CONTEXTO LOGIC_IF LOGIC_THEN LOGIC_FOR LOGIC_DO LOGIC_WHILE LOGIC_TERMINAL_PROGRAMA //LOGIC_UNTIL LOGIC_REPEAT LOGIC_BREAK LOGIC_CONTINUE
%token ARIT_MAS ARIT_MENOS ARIT_MULT ARIT_DIV
%token BOOL_AND BOOL_OR BOOL_NOT BOOL_IGUAL BOOL_DISTINTO BOOL_MAYOR BOOL_MAYORIGUAL BOOL_MENOR BOOL_MENORIGUAL BOOL_TRUE BOOL_FALSE
//%token PALABRA TIPO OPERADOR ABRE_SECCION CIERRA_SECCION TERMINAL APERTURA_PROGRAMA TERMINAL_PROGRAMA

%%

variable: ID_VARIABLE
        {
            // Mirar en la tabla de símbolos del contexto correspondiente si la variable ya existe
            // Si no existe, añadir a la tabla de símbolos 
        }
        ;

asignacion_variable: ID_VARIABLE LOGIC_ASIGNACION variable
        {
        }
        ;
%%