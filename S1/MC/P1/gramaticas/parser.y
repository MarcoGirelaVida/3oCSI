%{

#include "utils.hpp"
#include <iostream>

extern FILE *yyin;
extern int yylex();
extern int yyparse(); 

void yyerror(const char* msg)
{
    cerr << TEXTO_AMARILLO << "ERROR: Token de sintaxis: " << msg << TEXTO_NORMAL << endl;
}
%}

%token TOKEN1 TOKEN2

%%

start:
    TOKEN1 { printf("TOKEN1 procesado.\n"); }
  | TOKEN2 { printf("TOKEN2 procesado.\n"); }
  ;

%%