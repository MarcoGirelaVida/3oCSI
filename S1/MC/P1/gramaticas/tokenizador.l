%{
#include "utils.hh"
#include <iostream>
#include "parser.hh"
%}

MAYUSCULA [A-Z]
TEXTO     [a-zA-Z_]+
NUMERO    [0-9]+

%%

{NUMERO}                        { return NUMERO; }
{MAYUSCULA}-{TEXTO}             { return ID_VARIABLE; }

"->"                            { return LOGIC_ASIGNACION; }
"---"                           { return LOGIC_CIERRA_CONTEXTO; }
"SI"                            { return LOGIC_IF; }
"ENTONCES"                      { return LOGIC_THEN; }
"POR"                           { return LOGIC_FOR; }
"HACER"                         { return LOGIC_DO; }
"MIENTRAS"                      { return LOGIC_WHILE; }
//[EOF]                         { return LOGIC_TERMINAL_PROGRAMA; }

//"COMENTARIO"{TEXTO}[\n]       { return LOGIC_COMENTARIO; }
"+"                             { return ARIT_MAS; }
"-"                             { return ARIT_MENOS; }
"*"                             { return ARIT_MULT; }
"/"                             { return ARIT_DIV; }

"Y"                             { return BOOL_AND; }
"O"                             { return BOOL_OR; }
"NO"                            { return BOOL_NOT; }
"="                             { return BOOL_IGUAL; }
"NO="                            { return BOOL_DISTINTO; }
">"                             { return BOOL_MAYOR; }
">="                            { return BOOL_MAYORIGUAL; }
"<"                             { return BOOL_MENOR; }
"<="                            { return BOOL_MENORIGUAL; }
"VERDADERO"                     { return BOOL_TRUE; }
"FALSO"                         { return BOOL_FALSE; }

[ \t\n]+                        { /* Ignoro espacios y saltos de línea */ }
.                               { cerr << TEXTO_ROJO << "ERROR: Token desconocido: " << yytext << TEXTO_NORMAL << endl; }

%%

int yywrap()
{ return 1; }