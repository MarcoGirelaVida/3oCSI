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
%}

MAYUSCULA       [A-Z]
TEXTO           [a-zA-Z_0-9]+
NUMERO_ENTERO   [0-9]+
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
{ESPACIO_BLANCO}+               { /* no hacer nada */ }
\n+                             { loc->lines(yyleng); }

{MAYUSCULA}-{TEXTO}                                     {   yylval->build<std::string>(yytext);
                                                            return Parser::token::IDENTIFICADOR; }
{NUMERO_ENTERO}                                         {   yylval->build<int>(std::stoi(yytext));
                                                            return Parser::token::ENTERO; }
{NUMERO_ENTERO}\.{NUMERO_ENTERO}                        {   yylval->build<float>(std::stof(yytext));
                                                            return Parser::token::FLOAT; }
"->"                                                    { return Parser::token::ASIGNACION; }

"+"                                                     { return Parser::token::MAS; }
"-"                                                     { return Parser::token::MENOS; }
"*"                                                     { return Parser::token::MULT; }
"/"                                                     { return Parser::token::DIVISION; }

"("                                                     { return Parser::token::ABRE_PAREN; }
")"                                                     { return Parser::token::CIERRA_PAREN; }

"MUESTRAME"                                             { return Parser::token::PRINT; }
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

"|__"                           { return token::LOGIC_CIERRA_CONTEXTO; }
"¿("                            { return token::LOGIC_IF; }
")?-->"                         { return token::LOGIC_THEN; }
"|X--"                          { return token::LOGIC_ELSE; }
"FOR"                           { return token::LOGIC_FOR; }
"FROM"                          { return token::LOGIC_FROM; }
"TO"                            { return token::LOGIC_TO; }
"BY_STEP"                       { return token::LOGIC_STEP; }
"REPEAT-->"                     { return token::LOGIC_DO_BUCLES; }
"WHILE"                         { return token::LOGIC_WHILE; }
*/

/*
//"COMENTARIO"{TEXTO}[\n]       { return token::LOGIC_COMENTARIO; }

"^"                             { return token::BOOL_AND; }
"v"                             { return token::BOOL_OR; }
"~"                             { return token::BOOL_NOT; }
"=="                            { return token::BOOL_IGUAL; }
"~="                            { return token::BOOL_DISTINTO; }
">"                             { return token::BOOL_MAYOR; }
">="                            { return token::BOOL_MAYORIGUAL; }
"<"                             { return token::BOOL_MENOR; }
"<="                            { return token::BOOL_MENORIGUAL; }
"TRUE"                          { return token::BOOL_TRUE; }
"FALSE"                         { return token::BOOL_FALSE; }
*/