#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
using namespace std;

// Colores para la terminal
constexpr const char* TEXTO_MORADO = "\033[1;35m";  // Para warnings / información
constexpr const char* TEXTO_VERDE = "\033[1;32m";   // Para mensajes de éxito
constexpr const char* TEXTO_ROJO = "\033[1;31m";    // Para mensajes de error
constexpr const char* TEXTO_AZUL = "\033[1;34m";    // Para configuraciones y anotaciones
constexpr const char* TEXTO_AMARILLO = "\033[1;33m";// Para errores sintácticos
constexpr const char* TEXTO_NORMAL = "\033[0m";

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <stack>
#include <exception>

// Tipos soportados para los valores de las variables
using tipo_variable = variant<int, float, string>;

// Prefijos para las variables de los tipos
const map<string, string> prefijos_varibles =
{
    {"I", "int"},
    {"F", "float"},
    {"S", "string"}
};

// Clase para manejar el contexto y la tabla de símbolos
class Contexto
{
public:
    // Uso shared_ptr para que se libere memoria cuando ya no se va a usar más un contexto (una función por ejemplo)
    Contexto(shared_ptr<Contexto> contexto_padre = nullptr)
    : contexto_padre(contexto_padre), profundidad(contexto_padre ? contexto_padre->profundidad + 1 : 0) {}

    // Agregar o actualizar una variable en el contexto actual
    void set_variable(const string& identificador, tipo_variable valor)
    {
        //const char prefijo = identificador.c_str()[0];
        //if (prefijos_varibles[valor.index()] !=  prefijos_varibles[prefijo])
            tabla_simbolos_local[identificador] = valor;
        
    }

    // Obtener una variable (busca en el contexto actual y padres)
    tipo_variable get_variable(const string& identificador)
    {
        // Miro a ver si tengo yo la variable
        if (tabla_simbolos_local.find(identificador) != tabla_simbolos_local.end())
            return tabla_simbolos_local[identificador];
        else if (contexto_padre != nullptr) // Si no lo encunetro busco hacia atrás
            return contexto_padre->get_variable(identificador);
        else
        // Como hemos dicho que no hay que declarar variables, si no la encuentra la declara con valor default y lo devuelve
        {
            //throw runtime_error("Variable no definida: " + identificador);
            cerr << TEXTO_MORADO << "WARNING: Variable [" << identificador <<  "] está siendo usada sin definirse previamente" << TEXTO_NORMAL << endl;
            set_variable(identificador, 0);
            return tabla_simbolos_local[identificador];
        }
    }

    const size_t profundidad;
private:
    unordered_map<string, tipo_variable> tabla_simbolos_local;
    shared_ptr<Contexto> contexto_padre;
};

// Clase base para los nodos del AST
class ExprAST
{
public:
    virtual ~ExprAST() = default;
    virtual tipo_variable evaluate(std::shared_ptr<Contexto> Contexto) const = 0;
};

// Nodo para números (enteros y dobles)
class NumberExprAST : public ExprAST {
    tipo_variable Value;

public:
    NumberExprAST(tipo_variable Value) : Value(Value) {}
    tipo_variable evaluate(std::shared_ptr<Contexto> Contexto) const override
    {
        return Value;
    }
};

// Nodo para variables
class VariableExprAST : public ExprAST
{
    std::string Name;

public:
    VariableExprAST(const std::string& Name) : Name(Name) {}
    tipo_variable evaluate(std::shared_ptr<Contexto> Contexto) const override
    {
        return Contexto->getVariable(Name);
    }
};

// Nodo para asignaciones
class AssignmentExprAST : public ExprAST
{
    std::string Name;
    std::unique_ptr<ExprAST> Expr;

public:
    AssignmentExprAST(const std::string& Name, std::unique_ptr<ExprAST> Expr)
        : Name(Name), Expr(std::move(Expr)) {}
    tipo_variable evaluate(std::shared_ptr<Contexto> Contexto) const override
    {
        tipo_variable value = Expr->evaluate(Contexto);
        Contexto->setVariable(Name, value);
        return value;
    }
};

// Nodo para operaciones binarias
class BinaryExprAST : public ExprAST
{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    tipo_variable evaluate(std::shared_ptr<Contexto> Contexto) const override
    {
        tipo_variable LeftVal = LHS->evaluate(Contexto);
        tipo_variable RightVal = RHS->evaluate(Contexto);

        // Realizar la operación según los tipos de los operandos
        if (std::holds_alternative<int>(LeftVal) && std::holds_alternative<int>(RightVal))
        {
            int left = std::get<int>(LeftVal);
            int right = std::get<int>(RightVal);
            switch (Op) {
            case '+':
                return left + right;
            case '-':
                return left - right;
            case '*':
                return left * right;
            case '/':
                if (right == 0) throw std::runtime_error("División por cero");
                return left / right;
            default:
                throw std::runtime_error("Operador desconocido");
            }
        }
        else if (std::holds_alternative<double>(LeftVal) && std::holds_alternative<double>(RightVal))
        {
            double left = std::get<double>(LeftVal);
            double right = std::get<double>(RightVal);
            switch (Op)
            {
            case '+':
                return left + right;
            case '-':
                return left - right;
            case '*':
                return left * right;
            case '/':
                if (right == 0.0) throw std::runtime_error("División por cero");
                return left / right;
            default:
                throw std::runtime_error("Operador desconocido");
            }
        }
        else
        {
            throw std::runtime_error("Tipos incompatibles para la operación");
        }
    }
};
/*
// Ejemplo de uso
int main() {
    // Crear el contexto global
    auto globalContext = std::make_shared<Contexto>();

    // Asignar variables
    auto assignX = std::make_unique<AssignmentExprAST>("x", std::make_unique<NumberExprAST>(5));
    auto assignY = std::make_unique<AssignmentExprAST>("y", std::make_unique<NumberExprAST>(3));

    assignX->evaluate(globalContext);
    assignY->evaluate(globalContext);

    // Usar variables en una expresión: x + y
    auto expr = std::make_unique<BinaryExprAST>(
        '+',
        std::make_unique<VariableExprAST>("x"),
        std::make_unique<VariableExprAST>("y")
    );

    tipo_variable result = expr->evaluate(globalContext);

    if (std::holds_alternative<int>(result)) {
        std::cout << "El resultado es: " << std::get<int>(result) << std::endl;
    } else if (std::holds_alternative<double>(result)) {
        std::cout << "El resultado es: " << std::get<double>(result) << std::endl;
    }

    // Ejemplo con strings
    auto assignStr = std::make_unique<AssignmentExprAST>("s", std::make_unique<NumberExprAST>(std::string("Hola")));

    assignStr->evaluate(globalContext);

    auto varStr = std::make_unique<VariableExprAST>("s");
    tipo_variable strResult = varStr->evaluate(globalContext);

    if (std::holds_alternative<std::string>(strResult)) {
        std::cout << "La cadena es: " << std::get<std::string>(strResult) << std::endl;
    }

    // Ejemplo con vectores
    auto assignVec = std::make_unique<AssignmentExprAST>(
        "v",
        std::make_unique<NumberExprAST>(std::vector<int>{1, 2, 3})
    );

    assignVec->evaluate(globalContext);

    auto varVec = std::make_unique<VariableExprAST>("v");
    tipo_variable vecResult = varVec->evaluate(globalContext);

    if (std::holds_alternative<std::vector<int>>(vecResult)) {
        std::cout << "El vector es: ";
        for (int val : std::get<std::vector<int>>(vecResult)) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
*/

#endif