%{

#include "utils.hh"
#include <iostream>
using namespace std;

int yylex();
void yyerror(const char* msg)
{
    cerr << TEXTO_AMARILLO << "ERROR: Token de sintaxis: " << msg << TEXTO_NORMAL << endl;
}
%}
%debug

%token VARIABLE NUMERO
%token LOGIC_ASIGNACION LOGIC_CIERRA_CONTEXTO LOGIC_IF LOGIC_THEN LOGIC_FROM LOGIC_TO LOGIC_STEP LOGIC_FOR LOGIC_DO_BUCLES LOGIC_WHILE LOGIC_TERMINAL_PROGRAMA //LOGIC_UNTIL LOGIC_REPEAT LOGIC_BREAK LOGIC_CONTINUE
%token ARIT_MAS ARIT_MENOS ARIT_MULT ARIT_DIV ARIT_ABRE_PARENTESIS ARIT_CIERRA_PARENTESIS
%token BOOL_AND BOOL_OR BOOL_NOT BOOL_IGUAL BOOL_DISTINTO BOOL_MAYOR BOOL_MAYORIGUAL BOOL_MENOR BOOL_MENORIGUAL BOOL_TRUE BOOL_FALSE
//%token PALABRA TIPO OPERADOR ABRE_SECCION CIERRA_SECCION TERMINAL APERTURA_PROGRAMA TERMINAL_PROGRAMA

%%

variable: VARIABLE
        {
            // Mirar en la tabla de símbolos del contexto correspondiente si la variable ya existe
            // Si no existe, añadir a la tabla de símbolos 
        }
        ;

expresion_aritmetica: NUMERO
        {
        }
        | variable
        {
            // Comprobar que la variable es un número
        }
        | ARIT_ABRE_PARENTESIS expresion_aritmetica ARIT_CIERRA_PARENTESIS
        {
        }
        | expresion_aritmetica ARIT_MULT expresion_aritmetica
        {
        }
        | expresion_aritmetica ARIT_DIV expresion_aritmetica
        {
        }
        | expresion_aritmetica ARIT_MAS expresion_aritmetica
        {
        }
        | expresion_aritmetica ARIT_MENOS expresion_aritmetica
        {
        }
        | ARIT_MENOS expresion_aritmetica
        {
        }
        ;
    

asignacion_variable: variable LOGIC_ASIGNACION variable
        {
            // Comprobar que los tipos de las variables son compatibles
            // Dado que el atributo valor de las variables es siempre un string independientemente de su tipo,
            // se puede hacer la asignación directamente sin hacer switch ni cosas por el estilo
        }
        | variable LOGIC_ASIGNACION expresion_aritmetica
        {
            // Comprobar que sea un numero
        }
        | variable LOGIC_ASIGNACION expresion_booleana
        {
            // Comprobar que sea un booleano
        }

%%