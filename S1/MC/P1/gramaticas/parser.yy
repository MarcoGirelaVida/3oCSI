/*
cosas de bison:
- $$ el valor semantico de la expresión
- $1, $2, ... los valores semánticos de los elementos de la expresión
- #define YYSTYPE es el tipo de los valores semánticos en general (no se si se puede personalizar a uno concreto por cada uno)
- Si es un solo caracter se puede poner directamente en la gramática (ejemplo: '+'), sin tener un token asociado
- Por default se guarda $$ = $1, por ejemplo cuando haces expr: NUM, se guarda el valor semántico de NUM en $$
*/
%skeleton "lalr1.cc" // Sirve para que bison genere una clase parser en C++ (lo que bison llama la api)
%require  "3.0"
%debug 
%defines 
%define api.namespace {Interprete}          // Le estoy diciendo que el namespace de las clases que se generen sea Interprete
%define api.parser.class {Interprete_Parser}// Le estoy diciendo que el nombre de la clase que me va a crear sea Interprete_Parser

%code requires{
   namespace Interprete {
      class Interprete_Driver;
      class Interprete_Tokenizador;
   }
}
%parse-param { Interprete_Tokenizador  &tokenizador  }
%parse-param { Interprete_Driver  &driver  }

%code{
    #include <iostream>
    #include <fstream>
    #include <string>
    #include "interprete.hh"
    using namespace std;

#undef yylex
#define yylex tokenizador.yylex
}

//%define api.token.table true // Para que genere una tabla con los tokens y sus valores (se puede quitar)
//%define parse.error verbose // Para que muestre los errores de forma más detallada
//%define parse.trace true // Para que muestre la traza de la pila
%define api.value.type variant // Para que los tokens no sean de tipo YYSTYPE sino de tipo cualquiera
%define parse.assert // Para depurar, reduce el rendimiento

%token FIN_FICHERO 0 "Fin del fichero"
%token <std::string> VARIABLE
%token <float>  NUMERO
//%token LOGIC_ASIGNACION LOGIC_CIERRA_CONTEXTO LOGIC_IF LOGIC_THEN LOGIC_FROM LOGIC_TO LOGIC_STEP LOGIC_FOR LOGIC_DO_BUCLES LOGIC_WHILE LOGIC_TERMINAL_PROGRAMA //LOGIC_UNTIL LOGIC_REPEAT LOGIC_BREAK LOGIC_CONTINUE
//%token BOOL_AND BOOL_OR BOOL_NOT BOOL_IGUAL BOOL_DISTINTO BOOL_MAYOR BOOL_MAYORIGUAL BOOL_MENOR BOOL_MENORIGUAL BOOL_TRUE BOOL_FALSE
//%token PALABRA TIPO OPERADOR ABRE_SECCION CIERRA_SECCION TERMINAL APERTURA_PROGRAMA TERMINAL_PROGRAMA

%locations // Habilita que las ubicaciones se guarden en Interprete_Parser.location
/* Ejemplo
    WORD { printf("Palabra '%s' en línea %d, columnas %d-%d\n", $1.c_str(), @1.first_line, @1.first_column, @1.last_column); }
*/

%type <float> expr_arit

%%

fichero_programa: programa_marco FIN_FICHERO
        ;

programa_marco: /* nada o */
        | programa_marco expr_arit
        /* para hacerlo igual que C habria que añadir una regla para 
        las declaraciones pero ya he dicho que eso se hace automatico 
        Además siempre las nuevas cosas se ponen a la derecha de las antiguas
        "después de leer un número cualquiera de definiciones, intenta leer una nueva
        definición hasta que no puedas leer más*/
        /*
        | programa_marco error
        {
           yyerror("\"" + $2 + "\" no es una definición válida");
           yyerrok;
        }
        */
        ;
        /*no tiene acciones asociadas porque solo sirve para ver si el programa en si es válido*/
/*
variable: VARIABLE
        {
            // Mirar en la tabla de símbolos del contexto correspondiente si la variable ya existe
            // Si no existe, añadir a la tabla de símbolos 
        }
        ;
*/
expr_arit: NUMERO
        /*
        | variable
        {
            // Comprobar que la variable es un número
            if (tabla_simbolos[variable].tipo != ENTERO && tabla_simbolos[variable].tipo != REAL)
                yyerror("La variable " + variable + " no es un número");
        }
        */
        /*mandar warnings si los operandos son de tipos distintos*/
        | '(' expr_arit ')'
        {
            $$ = $2;
        }
        | expr_arit '*' expr_arit
        {
            $$ = $1 * $3;
        }
        | expr_arit '/' expr_arit
        {
            if ($3 == 0)
            {
                //yyerror("Se ha intentado dividir por 0, se usará 1 como dividendo");
                //yyerrok;
                $$ = $1 / 1;
            }
            else
                $$ = $1 / $3;
        }
        | expr_arit '+' expr_arit
        {
            $$ = $1 + $3;
        }
        | expr_arit '-' expr_arit
        {
            $$ = $1 - $3;
        }
        | '-' expr_arit
        {
            $$ = -$2;
        }
        ;
    
/*
asignacion_variable: variable LOGIC_ASIGNACION variable
        {
            // Comprobar que los tipos de las variables son compatibles
            // Dado que el atributo valor de las variables es siempre un string independientemente de su tipo,
            // se puede hacer la asignación directamente sin hacer switch ni cosas por el estilo
        }
        | variable LOGIC_ASIGNACION expr_arit
        {
            // Comprobar que sea un numero
        }
        | variable LOGIC_ASIGNACION expr_bool
        {
            // Comprobar que sea un booleano
        }
*/
%%

void Interprete::Interprete_Parser::error(const location_type &localizacion_error, const string &error_msg)
{
    // yyerror(const char * error_msg)
    //{cerr << TEXTO_AMARILLO << "ERROR SINTACTICO en línea " << lineno() << ": " << error_msg << TEXTO_NORMAL << endl;}
    // actualizado para usar location y clases:
    cerr << TEXTO_AMARILLO << "ERROR SINTACTICO en [" << localizacion_error << "] :" << error_msg << TEXTO_NORMAL << endl;
}