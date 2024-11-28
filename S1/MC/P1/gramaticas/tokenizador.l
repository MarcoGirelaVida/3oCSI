%{
#include "utils.hh"
#include <iostream>
#include "parser.hh"
using namespace std;
%}

%option noyywrap
%option c++

MAYUSCULA       [A-Z]
TEXTO           [a-zA-Z0-9_]+
NUMERO_ENTERO   [0-9]+

%%

{MAYUSCULA}-{TEXTO}             { return yylval.str = new string (yytext); return VARIABLE; }
{NUMERO_ENTERO}                 { return NUMERO_ENTERO; }
{NUMERO_ENTERO}.{NUMERO_ENTERO} { return NUMERO_FLOAT; }

"->"                            { return LOGIC_ASIGNACION; }
"|__"                           { return LOGIC_CIERRA_CONTEXTO; }
"¿("                            { return LOGIC_IF; }
")?-->"                         { return LOGIC_THEN; }
"|X--"                          { return LOGIC_ELSE; }
"FOR"                           { return LOGIC_FOR; }
"FROM"                          { return LOGIC_FROM; }
"TO"                            { return LOGIC_TO; }
"BY_STEP"                       { return LOGIC_STEP; }
"REPEAT-->"                     { return LOGIC_DO_BUCLES; }
"WHILE"                         { return LOGIC_WHILE; }
//[EOF]                         { return LOGIC_TERMINAL_PROGRAMA; }

//"COMENTARIO"{TEXTO}[\n]       { return LOGIC_COMENTARIO; }
"+"                             { return ARIT_MAS; }
"-"                             { return ARIT_MENOS; }
"*"                             { return ARIT_MULT; }
"/"                             { return ARIT_DIV; }
"("                             { return ARIT_ABRE_PARENTESIS; }
")"                             { return ARIT_CIERRA_PARENTESIS; }

"^"                             { return BOOL_AND; }
"v"                             { return BOOL_OR; }
"~"                             { return BOOL_NOT; }
"=="                            { return BOOL_IGUAL; }
"~="                            { return BOOL_DISTINTO; }
">"                             { return BOOL_MAYOR; }
">="                            { return BOOL_MAYORIGUAL; }
"<"                             { return BOOL_MENOR; }
"<="                            { return BOOL_MENORIGUAL; }
"TRUE"                          { return BOOL_TRUE; }
"FALSE"                         { return BOOL_FALSE; }

[ \t\n]+                        { /* Ignoro espacios y saltos de línea */ }
.                               { cerr << TEXTO_ROJO << "ERROR: Token desconocido: " << yytext << TEXTO_NORMAL << endl; }

%%

int yyFlexLexer::yylex();