%{
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
#include <iostream>
/* Sobreescribo la función yylex de flex con la mia*/
#include "parser.tab.hh"
#include "tokenizador.hh"
using Parser = Interprete::Interprete_Parser; // Sino es un coñazo
#undef YY_DECL
#define YY_DECL int Interprete::Interprete_Tokenizador::yylex(Parser::semantic_type * const lval, Parser::location_type *loc)
%}

%option debug
%option nodefault
%option noyywrap
%option c++
%option yyclass="Interprete::Interprete_Tokenizador"

%{


/* el fin del fichero es un token en lugar de null */
#define yyterminate() return(Parser::token::FIN_FICHERO)

/* No sé para qué sirve pero hay que ponerlo porque sino no funciona */
#define YY_NO_UNISTD_H
%}

%{
// Constructores de tokens
//Parser::symbol_type make_NUMERO (const std::string &s, const Parser::location_type& loc);
//CARACTER        \'{LETRA}\' 
%}

MAYUSCULA       [A-Z]
LETRA           [a-zA-Z]
DIGITO          [0-9]
LETRA_DIGITO    {LETRA}|{DIGITO}
NOMBRE_ID       {LETRA}({LETRA_DIGITO})*
ID              {MAYUSCULA}-{NOMBRE_ID}
ID_CONSTANTE    "C"{ID}
ID_FUNCION      "F"-{NOMBRE_ID}
ID_FUNCION_CONSTANTE "C"{ID_FUNCION}
STRING          \"[^\"]*\"

NUMERO_ENTERO   {DIGITO}+
NUMERO_REAL     {NUMERO_ENTERO}\.{NUMERO_ENTERO}

SALTO_LINEA     \n
ESPACIO_BLANCO  [ \t\r]

%{
    #define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}
%%
%{
    // Este código se ejecuta cada vez que se encuentra un token
    // Actualizo la localización y valor
    //Interprete::location& loc = drv.location;
    yylval = lval;
    //std::cerr << "Token leido: " << yytext << std::endl;
%}
{ESPACIO_BLANCO}+                                       { /* no hacer nada */ }
{SALTO_LINEA}+                                          { loc->lines(yyleng); return Parser::token::SALTO_LINEA; }

{STRING}                                                {   yylval->build<std::string>(yytext);
                                                            return Parser::token::STRING; }
                                                        
{ID}|{ID_CONSTANTE}                                     {   yylval->build<std::string>(yytext);
                                                            return Parser::token::IDENTIFICADOR; }
{ID_FUNCION}|{ID_FUNCION_CONSTANTE}                     {   yylval->build<std::string>(yytext);
                                                            return Parser::token::ID_FUNCION; }
{NUMERO_ENTERO}                                         {   yylval->build<int>(std::stoi(yytext));
                                                            return Parser::token::ENTERO; }
{NUMERO_REAL}                                           {   yylval->build<float>(std::stof(yytext));
                                                            return Parser::token::FLOAT; }

"->"                            { return Parser::token::ASIGNACION; }
"|***"                          { return Parser::token::ABRE_CONTEXTO; }
"|___"                          { return Parser::token::CIERRA_CONTEXTO; }
"¿("                            { return Parser::token::IF; }
")?-->"                         { return Parser::token::THEN; }
"|X-->"                         { return Parser::token::ELSEIF; }
"|X--"                          { return Parser::token::ELSE; }
"FOR"                           { return Parser::token::FOR; }
"FROM"                          { return Parser::token::FROM; }
"TO"                            { return Parser::token::TO; }
"BY_STEP"                       { return Parser::token::STEP; }
"REPEAT-->"                     { return Parser::token::DO_BUCLES; }
"WHILE"                         { return Parser::token::WHILE; }
"DEVUELVE"                      { return Parser::token::RETURN; }
"MUESTRAME"                     { return Parser::token::PRINT; }

"^"                             { return Parser::token::BOOL_AND; }
"v"                             { return Parser::token::BOOL_OR; }
"~"                             { return Parser::token::BOOL_NOT; }
"=="                            { return Parser::token::BOOL_IGUAL; }
"~="                            { return Parser::token::BOOL_DISTINTO; }
">"                             { return Parser::token::BOOL_MAYOR; }
">="                            { return Parser::token::BOOL_MAYORIGUAL; }
"<"                             { return Parser::token::BOOL_MENOR; }
"<="                            { return Parser::token::BOOL_MENORIGUAL; }
"TRUE"                          { return Parser::token::BOOL_TRUE; }
"FALSE"                         { return Parser::token::BOOL_FALSE; }

"+"                             { return Parser::token::MAS; }
"-"                             { return Parser::token::MENOS; }
"*"                             { return Parser::token::MULT; }
"/"                             { return Parser::token::DIVISION; }

"("                             { return Parser::token::ABRE_PAREN; }
")"                             { return Parser::token::CIERRA_PAREN; }
"["                             { return Parser::token::ABRE_CORCHETE; }
"]"                             { return Parser::token::CIERRA_CORCHETE; }
"{"                             { return Parser::token::ABRE_LLAVE; }
"}"                             { return Parser::token::CIERRA_LLAVE; }
"."                             { return Parser::token::PUNTO; }
";"                             { return Parser::token::PUNTO_COMA; }
","                             { return Parser::token::COMA; }
":"                             { return Parser::token::DOS_PUNTOS; }


.   { throw Parser::syntax_error (*loc, "token no reconocido: " + std::string(yytext)); }
%%
/*
// Copiado y pegado de la documentación de bison:
// https://www.gnu.org/software/bison/manual/bison.html#A-Simple-C_002b_002b-Example
// Basicamente crea otro constructor de numero con un string y cuando ya lo tiene como
// un float se lo pasa al contructor original
// CUIDADO!! Al llamar al contrcutor hay que llamar a este, no el verdadero
Parser::symbol_type
make_NUMERO (const std::string &s, const Parser::location_type& loc)
{
    errno = 0;
    char *end = nullptr;
    float f = strtof(s.c_str(), &end);

    // Verifica si la conversión fue exitosa
    if (end == s.c_str() || *end != '\0' || errno == ERANGE)
        throw Parser::syntax_error(loc, "float is out of range or invalid: " + s);

    return Parser::token::NUMERO((float) f, loc);
}

// En caso de que no quiera tener distincciones de tipos
//{TEXTO}                                                 {   yylval->build<std::string>(yytext);
//                                                            return Parser::token::IDENTIFICADOR; }
*/

/*
//"COMENTARIO"{TEXTO}[\n]       { return token::LOGIC_COMENTARIO; }

*/