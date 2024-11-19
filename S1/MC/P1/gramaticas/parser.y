%{
#include "utils.hpp"
#include "y.tab.h"

//SyntaxTree syntaxTree;

extern FILE *yyin; // Archivo de entrada para el lexer
int yylex(void);   // Función generada por Lex

void yyerror(const char* msg)
{
    fprintf(stderr, TEXTO_AMARILLO + "Error de sintaxis: %s\n" + TEXTO_NORMAL, msg);
}
%}

// DEFINICIONES DE TIPOS
%token PRINT INT ID NUMBER
%token ASSIGN SEMICOLON LPAREN RPAREN LBRACE RBRACE

%union
{
    int num;
    char* str;
    SyntaxTreeNode* node;
}

// Tipos asociados a tokens y reglas
%type <node> statement expression
%type <num> NUMBER
%type <str> ID

%%

// REGLAS
program:
    statement_list
    ;

statement_list:
    statement_list statement
    | /* vacío */
    ;

statement:
    INT ID ASSIGN expression SEMICOLON
        { 
            SyntaxTreeNode node = {"variable_declaration", $2, std::to_string($4)};
            syntaxTree.nodes.push_back(node);
        }
    |
    ID ASSIGN expression SEMICOLON
        { 
            SyntaxTreeNode node = {"assignment", $1, std::to_string($3)};
            syntaxTree.nodes.push_back(node);
        }
    |
    PRINT LPAREN expression RPAREN SEMICOLON
        { 
            SyntaxTreeNode node = {"print", "", std::to_string($3)};
            syntaxTree.nodes.push_back(node);
        }
    ;

expression:
    NUMBER
        { $$ = $1; }
    |
    ID
        { $$ = 0; /* Puedes manejar valores más adelante */ }
    ;

%%

// CODIGO
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <archivo fuente>\n", argv[0]);
        return 1;
    }

    // Abrir archivo fuente
    FILE* file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Error al abrir archivo");
        return 1;
    }
    yyin = file;

    // Ejecutar análisis sintáctico
    if (yyparse() == 0)
    {
        // Generar el código C++
        Compiler compiler("output.cpp");
        compiler.generateCode(syntaxTree);
    }

    fclose(file);
    return 0;
}
