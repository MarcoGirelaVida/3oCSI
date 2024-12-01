
%{
#include <string>
#include <iostream>

/* Sobreescribo la función yylex de flex con la mia*/
#include "tokenizador.hh"
#undef  YY_DECL
#define YY_DECL int Interprete::Interprete_Tokenizador::yylex(Interprete::Interprete_Parser::semantic_type * const lval, Interprete::Interprete_Parser::location_type *loc)

using token = Interprete::Interprete_Parser::token;

/* el fin del fichero es un token en lugar de null */
#define yyterminate() return( token::FIN_FICHERO )

/* No sé para qué sirve pero hay que ponerlo porque sino no funciona */
#define YY_NO_UNISTD_H

/* macro para actualizar la localización automaticamente */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option nodefault
%option yyclass="Interprete::Interprete_Tokenizador"
%option noyywrap
%option c++


MAYUSCULA       [A-Z]
TEXTO           [a-zA-Z0-9_]+
NUMERO_ENTERO   [0-9]+

%%
%{          /** Simpre que empieza a ejecutarse lex iguala yylval a lval **/
            yylval = lval;
            std::cerr << "Token leido: " << yytext << std::endl;
%}

{MAYUSCULA}-{TEXTO}                                 { yylval->build<std::string>(yytext);   return token::VARIABLE; }
{NUMERO_ENTERO}|{NUMERO_ENTERO}\.{NUMERO_ENTERO}     { yylval->build<float>(strtof(yytext, NULL));         return token::NUMERO; }
"+"                             { return '+'; }
"-"                             { return '-'; }
"*"                             { return '*'; }
"/"                             { return '/'; }
"("                             { return '('; }
")"                             { return ')'; }
[ \t\n]+                        { /* Ignoro espacios y saltos de línea */ }
.                               { std::cerr << "ERROR: Token desconocido: " << yytext << std::endl; }
%%
/*
"->"                            { return token::LOGIC_ASIGNACION; }
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