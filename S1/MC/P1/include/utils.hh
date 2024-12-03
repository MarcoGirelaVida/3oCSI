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
using VariableValue = std::variant<int, double, std::string, std::vector<int>>;

// Clase para manejar el contexto y la tabla de símbolos
class Context {
public:
    // Constructor para crear un contexto hijo
    Context(std::shared_ptr<Context> parent = nullptr) : parent(parent) {}

    // Agregar o actualizar una variable en el contexto actual
    void setVariable(const std::string& name, VariableValue value) {
        symbols[name] = value;
    }

    // Obtener una variable (busca en el contexto actual y padres)
    VariableValue getVariable(const std::string& name) {
        if (symbols.find(name) != symbols.end()) {
            return symbols[name];
        } else if (parent) {
            return parent->getVariable(name);
        } else {
            throw std::runtime_error("Variable no definida: " + name);
        }
    }

private:
    std::unordered_map<std::string, VariableValue> symbols;
    std::shared_ptr<Context> parent;
};

// Clase base para los nodos del AST
class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual VariableValue evaluate(std::shared_ptr<Context> context) const = 0;
};

// Nodo para números (enteros y dobles)
class NumberExprAST : public ExprAST {
    VariableValue Value;

public:
    NumberExprAST(VariableValue Value) : Value(Value) {}
    VariableValue evaluate(std::shared_ptr<Context> context) const override {
        return Value;
    }
};

// Nodo para variables
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string& Name) : Name(Name) {}
    VariableValue evaluate(std::shared_ptr<Context> context) const override {
        return context->getVariable(Name);
    }
};

// Nodo para asignaciones
class AssignmentExprAST : public ExprAST {
    std::string Name;
    std::unique_ptr<ExprAST> Expr;

public:
    AssignmentExprAST(const std::string& Name, std::unique_ptr<ExprAST> Expr)
        : Name(Name), Expr(std::move(Expr)) {}
    VariableValue evaluate(std::shared_ptr<Context> context) const override {
        VariableValue value = Expr->evaluate(context);
        context->setVariable(Name, value);
        return value;
    }
};

// Nodo para operaciones binarias
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    VariableValue evaluate(std::shared_ptr<Context> context) const override {
        VariableValue LeftVal = LHS->evaluate(context);
        VariableValue RightVal = RHS->evaluate(context);

        // Realizar la operación según los tipos de los operandos
        if (std::holds_alternative<int>(LeftVal) && std::holds_alternative<int>(RightVal)) {
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
        } else if (std::holds_alternative<double>(LeftVal) && std::holds_alternative<double>(RightVal)) {
            double left = std::get<double>(LeftVal);
            double right = std::get<double>(RightVal);
            switch (Op) {
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
        } else {
            throw std::runtime_error("Tipos incompatibles para la operación");
        }
    }
};
/*
// Ejemplo de uso
int main() {
    // Crear el contexto global
    auto globalContext = std::make_shared<Context>();

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

    VariableValue result = expr->evaluate(globalContext);

    if (std::holds_alternative<int>(result)) {
        std::cout << "El resultado es: " << std::get<int>(result) << std::endl;
    } else if (std::holds_alternative<double>(result)) {
        std::cout << "El resultado es: " << std::get<double>(result) << std::endl;
    }

    // Ejemplo con strings
    auto assignStr = std::make_unique<AssignmentExprAST>("s", std::make_unique<NumberExprAST>(std::string("Hola")));

    assignStr->evaluate(globalContext);

    auto varStr = std::make_unique<VariableExprAST>("s");
    VariableValue strResult = varStr->evaluate(globalContext);

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
    VariableValue vecResult = varVec->evaluate(globalContext);

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