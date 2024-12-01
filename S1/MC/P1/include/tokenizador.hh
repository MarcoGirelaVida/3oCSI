#ifndef TOKENIZADOR_HPP
#define TOKENIZADOR_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

//Interprete::Interprete_Parser::symbol_type::symbol_type (int tok) : super_type(tok, nullptr);

//Interprete::Interprete_Parser::symbol_type::symbol_type(int tok) { Interprete::Interprete_Parser::symbol_type::super_type(Interprete::Interprete_Parser::token_kind_type(tok)); std::move (nullptr);}
namespace Interprete{

class Interprete_Tokenizador : public yyFlexLexer{

public:
   Interprete_Tokenizador(std::istream *input) : yyFlexLexer(input)
   {};
   virtual ~Interprete_Tokenizador() {};

   // Para evitar fallo del compilador por sobreescribir
   using FlexLexer::yylex;
   virtual
   int yylex(Interprete::Interprete_Parser::semantic_type * const lval, Interprete::Interprete_Parser::location_type *location);

   
private:
   Interprete::Interprete_Parser::semantic_type *yylval = nullptr;
  // Otras opciones
  //bool trace_scanning;
  //Interprete::location location;
};

} /* fin namespace */

#endif
