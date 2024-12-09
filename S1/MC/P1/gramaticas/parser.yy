/*
cosas de bison:
- $$ el valor semantico de la expresión
- $1, $2, ... los valores semánticos de los elementos de la expresión
- #define YYSTYPE es el tipo de los valores semánticos en general (no se si se puede personalizar a uno concreto por cada uno)
- Si es un solo caracter se puede poner directamente en la gramática (ejemplo: '+'), sin tener un token asociado
- Por default se guarda $$ = $1, por ejemplo cuando haces expr: NUM, se guarda el valor semántico de NUM en $$
- nterm y type es lo mismo realmente solo que nterm es solo para no terminales y type sirve para los dos. nterm es más moderno
*/
%skeleton "lalr1.cc" // Sirve para que bison genere una clase parser en C++ (lo que bison llama la api)
%require  "3.8"
%debug 
%header 
%define api.token.raw // Mejora la eficiencia porque los tokens no se asocian a su ascii
%define api.namespace {Interprete}          // Le estoy diciendo que el namespace de las clases que se generen sea Interprete
%define api.parser.class {Interprete_Parser}// Le estoy diciendo que el nombre de la clase que me va a crear sea Interprete_Parser
//%define api.token.table true // Para que genere una tabla con los tokens y sus valores (se puede quitar)
//%define api.token.constructor // Para que los tokens se construyan con el constructor por defecto
// lo del contructor da muchisimos problemas
%define api.value.type variant // Para que los tokens no sean de tipo YYSTYPE sino de tipo cualquiera
%define parse.assert // Para depurar, reduce el rendimiento

%code requires{
    namespace Interprete {
        class Interprete_Driver;
        class Interprete_Tokenizador;
    }
}
%parse-param { Interprete::Interprete_Tokenizador  &tokenizador  }
%parse-param { Interprete::Interprete_Driver  &interprete_driver  }
%locations // Habilita que las ubicaciones se guarden en Interprete_Parser.location

//%define parse.trace true // Para que muestre la traza de la pila, se incluye en debug
%define parse.error detailed // Para que muestre los errores de forma más detallada
%define parse.lac full // Para que lea con anticipación porque sino puede dar errores falsos

%code{
    #include <iostream>
    #include <fstream>
    #include <string>
    #include "interprete.hh"
    using namespace std;
    using Parser = Interprete::Interprete_Parser;

#undef yylex
#define yylex tokenizador.yylex
}

// Le pongo los alias a los signos porque los hemos puesto con raw
//%define api.token.prefix {TOKEN_}
%token FIN_FICHERO 0 "Fin del fichero"
%token SALTO_LINEA "Salto de linea"
%token <std::string> IDENTIFICADOR "Identificador"
%token <std::string> ID_FUNCION "Identificador_funcion"


%token
    ASIGNACION        "->"

    ABRE_CONTEXTO
    CIERRA_CONTEXTO
    IF
    THEN
    ELSE
    ELSEIF
    FOR
    FROM
    TO
    STEP
    DO_BUCLES
    WHILE

    BOOL_OR           "or"
    BOOL_AND          "and"
    BOOL_NOT          "not"
    BOOL_IGUAL        "=="
    BOOL_DISTINTO     "!="
    BOOL_MAYOR        ">"
    BOOL_MAYORIGUAL   ">="
    BOOL_MENOR        "<"
    BOOL_MENORIGUAL   "<="
    BOOL_TRUE         "True"
    BOOL_FALSE        "False"

    MENOS             "-"
    MAS               "+"
    MULT              "*"
    DIVISION          "/"

    ABRE_PAREN        "("
    CIERRA_PAREN      ")"
    ABRE_CORCHETE     "["
    CIERRA_CORCHETE   "]"
    ABRE_LLAVE        "{"
    CIERRA_LLAVE      "}"
    COMA              ","
    PUNTO_COMA        ";"
    PUNTO             "."
    DOS_PUNTOS        ":"
;


%token <int>    ENTERO  "Entero"
%token <float>  FLOAT   "Float"
%token <std::string>    STRING  "string"

%token RETURN "return"
%token PRINT "print"
//%token LOGIC_ASIGNACION LOGIC_CIERRA_CONTEXTO LOGIC_IF LOGIC_THEN LOGIC_FROM LOGIC_TO LOGIC_STEP LOGIC_FOR LOGIC_DO_BUCLES LOGIC_WHILE LOGIC_TERMINAL_PROGRAMA //LOGIC_UNTIL LOGIC_REPEAT LOGIC_BREAK LOGIC_CONTINUE
//%token BOOL_AND BOOL_OR BOOL_NOT BOOL_IGUAL BOOL_DISTINTO BOOL_MAYOR BOOL_MAYORIGUAL BOOL_MENOR BOOL_MENORIGUAL BOOL_TRUE BOOL_FALSE
//%token PALABRA TIPO OPERADOR ABRE_SECCION CIERRA_SECCION TERMINAL APERTURA_PROGRAMA TERMINAL_PROGRAMA



%printer { yyo << $$; } <*>; // Para que imprima los valores semánticos de los tokens
%%

%start programa_marco;

// Un programa C es una serie de definiciones y declaraciones
programa_marco:
        "Fin del fichero"
        | elementos_programa "Fin del fichero"
        ;

elementos_programa:
        "Salto de linea"
        | elemento_programa "Salto de linea"
        | elementos_programa "Salto de linea"
        | elementos_programa elemento_programa "Salto de linea"
        /* para hacerlo igual que C habria que añadir una regla para 
        las declaraciones pero ya he dicho que eso se hace automatico 
        Además siempre las nuevas cosas se ponen a la derecha de las antiguas
        "después de leer un número cualquiera de definiciones, intenta leer una nueva
        definición hasta que no puedas leer más*/
        /*
        | programa_marco error
        */
        ;

elemento_programa: elemento_compuesto | elemento_simple;

elemento_simple:
        expresion
        | asignacion
        | return
        ;
    
elemento_compuesto:
        definicion_funcion
        | condicional
        | bucle_for
        | bucle_while
        ;

//---ELEMENTOS SIMPLES---
asignacion:
        "Identificador" "->" expresion
        {
        }
        | "Identificador" "->" error
        {
        }
        ;

return: "return" expresion
        {
        }
        | "return" error
        {
        }
        ;

expresion: lista_ors;

lista_ors:
        lista_ors "or" lista_ands
        | lista_ands
        ;

lista_ands:
        lista_ands "and" lista_not
        | lista_not
        ;

lista_not:
        "not" lista_not
        | lista_comparaciones
        ;

lista_comparaciones:
        suma_resta comparacion
        | suma_resta
        ;

comparacion:
        "==" suma_resta
        | "!=" suma_resta
        | ">" suma_resta
        | ">=" suma_resta
        | "<" suma_resta
        | "<=" suma_resta
        ;

suma_resta:
        suma_resta "+" producto_division_modulo
        | suma_resta "-" producto_division_modulo
        | producto_division_modulo
        ;

producto_division_modulo:
        producto_division_modulo "*" operando
        | producto_division_modulo "/" operando
        | operando
        ;
    
operando:
        "+" operando
        | "-" operando
        | llamada_funcion
        ;

llamada_funcion:
        llamada_funcion "(" argumentos ")"
        //| llamada_funcion "[" argumentos "]"
        | variable
        ;

variable:
        "Identificador"
        | "True"
        | "False"
        | "string"
        | "Entero"
        | "Float"
        ;

argumentos:
        argumentos "," expresion
        | expresion
        ;
//---ELEMENTOS COMPUESTOS---

bloque:
        ABRE_CONTEXTO elementos_programa CIERRA_CONTEXTO
        | elemento_programa
        ;

// FUNCIONES
definicion_funcion:
        "Identificador_funcion" ABRE_PAREN lista_parametros CIERRA_PAREN bloque
        ;

lista_parametros:
        lista_parametros "," IDENTIFICADOR
        | IDENTIFICADOR
        ;

// CONDICIONALES
condicional:
        IF expresion THEN bloque condicional_elseif
        | IF expresion THEN bloque ELSE bloque
        | IF expresion THEN bloque
        ;
condicional_elseif:
        ELSEIF expresion THEN bloque condicional_elseif
        | ELSEIF expresion THEN bloque ELSE bloque
        | ELSEIF expresion THEN bloque
        ;

// BUCLES
bucle_for:
        FOR expresion FROM expresion TO expresion STEP expresion DO_BUCLES bloque
        | FOR expresion FROM expresion TO expresion DO_BUCLES bloque
        | FROM expresion TO expresion STEP expresion DO_BUCLES bloque
        | FROM expresion TO expresion DO_BUCLES bloque
        | FOR expresion STEP expresion DO_BUCLES bloque
        | FOR expresion DO_BUCLES bloque
        ;

bucle_while:
        WHILE expresion DO_BUCLES bloque
        ;
%%

void Interprete::Interprete_Parser::error(const location_type &localizacion_error, const string &error_msg)
{
    // yyerror(const char * error_msg)
    //{cerr << TEXTO_AMARILLO << "ERROR SINTACTICO en línea " << lineno() << ": " << error_msg << TEXTO_NORMAL << endl;}
    // actualizado para usar location y clases:
    cerr << TEXTO_AMARILLO << "ERROR SINTACTICO en [" << localizacion_error << "] :" << error_msg << TEXTO_NORMAL << endl;
}