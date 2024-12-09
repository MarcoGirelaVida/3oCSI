#include "interprete.hh"
using namespace std;

//const string Interprete::Interprete_Driver::NOMBRE_CPP_BASE = "foo.cc";
/*************************************************************************/
// Funciones principales
/*************************************************************************/

void
Interprete::Interprete_Driver::leer_fuente(const string &fichero_fuente)
{
    ifstream input_fuente(fichero_fuente);
    /*
    if( !input_fuente.good() )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir el fichero [" << fichero_fuente << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }
    */
    leer_input(input_fuente);
}

void
Interprete::Interprete_Driver::leer_input(istream &input_fuente)
{
    /*
    if (!input_fuente.good() and input_fuente.eof())
    {
        cerr << TEXTO_ROJO << "ERROR: Error: " << stderr(errno) << " en el input del fuente" << TEXTO_NORMAL << endl;
        exit(EXIT_FAILURE);
    }
    */
    // Notese que si hubiese puento !input_fuente.good() fallará si se le proporciona un input vacio (EOF)
    // porque lo considera un error, es por eso que he puesto que falle si fail o bad

    // Intento crear el tokenizador
    try
    {
        tokenizador = make_unique<Interprete::Interprete_Tokenizador>(&input_fuente);
    }
    catch( bad_alloc &ba )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo crear el tokenizador: [" << ba.what() << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }

    // Intento crear el parser
    try
    {
        parser = make_unique<Interprete::Interprete_Parser>((*tokenizador), (*this));
    }
    catch( bad_alloc &ba )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo crear el Parser: [" << ba.what() << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }

    // Ejecuto el parser (si no devuelve 0 es que ha habido un error)
    parser->set_debug_level(false);
    if(parser->parse())
    {
        cerr << TEXTO_ROJO << "ERROR: Al ejecutar el parser" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }
    else
        cerr << TEXTO_VERDE << "EJECUCIÓN DEL PARSER COMPLETADA" << TEXTO_NORMAL << endl;
}

void
Interprete::Interprete_Driver::ejecutar(const string &fichero_fuente, const bool linea_a_linea)
{
    ifstream input_fuente(fichero_fuente);
    /*
    if( ! input_fuente.good() )
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir el fichero [" << fichero_fuente << "]" << TEXTO_NORMAL << endl;
        exit( EXIT_FAILURE );
    }
    */
    ejecutar(input_fuente, linea_a_linea);
}

void
Interprete::Interprete_Driver::ejecutar(istream &input_fuente, const bool linea_a_linea)
{
    if (!input_fuente.good() and input_fuente.eof())
    {
        cerr << TEXTO_ROJO << "ERROR: Error: " << strerror(errno) << " en el input del fuente" << TEXTO_NORMAL << endl;
        exit(EXIT_FAILURE);
    }

    if (linea_a_linea)
    {
        // Leer linea a linea
        string linea;
        while (getline(input_fuente, linea))
        {
            istringstream input_linea(linea); // Leo la linea
            leer_input(input_linea);
            // Ejecutar la linea
            // ...
            // Duermo hasta que me den la orden de seguir (\n)
            do
            {
                cerr << TEXTO_AZUL << "Pulse [ENTER] para continuar" << TEXTO_NORMAL << endl;
            }
            while (cin.get() != '\n');
        }
    }
    else
    {
        // Leer todo el input
        leer_input(input_fuente);
        // Ejecutar todo el input
        // ...
    }
}


/*************************************************************************/
// Funciones de compilacion
/*************************************************************************/
/*
void
Interprete::Interprete_Driver::compilar_to_cpp(const string &FICHERO_CPP, const string &ejecutable, const string &variables_compilacion, const bool conservar_cpp)
{
    // Nombre el fichero_cpp en base a FICHERO_CPP_BASE y la ubicación del ejecutable
    const string FICHERO_CPP = filesystem::path(ejecutable).parent_path().string() + "/" + NOMBRE_CPP_BASE;
    compilar_to_cpp(FICHERO_CPP);

    // Compilación a ejecutable
    string comando = string("g++ ") + FICHERO_CPP + " -o " + variables_compilacion + ejecutable;
    system(comando.c_str());

    // Borro el archivo .cpp
    if (!conservar_cpp)
        filesystem::remove(FICHERO_CPP);

    if (MODO_VERBOSO)
        cerr << TEXTO_VERDE << "COMPILACION DE ["<< FICHERO_CPP << "] A EJECUTABLE  [" << ejecutable << "] COMPLETADA" << TEXTO_NORMAL << endl;
    // El nombre del cpp será siempre igual ya que realmente es transparente al usuario
    ofstream pipe_cpp(FICHERO_CPP);
    if (!pipe_cpp)
    {
        cerr << TEXTO_ROJO << "ERROR: No se pudo abrir/crear el fichero " << FICHERO_CPP << TEXTO_NORMAL << endl;
        exit(1);
    }

    if (MODO_VERBOSO)
        cerr << TEXTO_VERDE << "COMPILACIÓN DEL FICHERO [" << nombre_fuente << "] COMPLETADA" << TEXTO_NORMAL << endl;
}
*/
/*************************************************************************/
// 
/*************************************************************************/

/*************************************************************************/
// 
/*************************************************************************/


/*************************************************************************/
// 
/*************************************************************************/


/*************************************************************************/
/*CLASE TIPOS DATOS*/
/*************************************************************************/
const array<tipo_dato, 3> Interprete::Tipos_Datos::db_tipos = 
{
    tipo_dato('I', "entero", 0),
    tipo_dato('D', "doble", 0.0),
    tipo_dato('S', "string", "")
};
const unordered_map<string, size_t> Interprete::Tipos_Datos::indice_prefijo = 
{
    {"I", 0},
    {"D", 1},
    {"S", 2}
};

size_t Interprete::Tipos_Datos::index(const char prefijo)
    {   return indice_prefijo.find(string(1, prefijo))->second;    }
string Interprete::Tipos_Datos::nombre_completo(const char prefijo)
    {   return db_tipos.at(index(prefijo)).nombre_completo; }
tipo_variable Interprete::Tipos_Datos::valor_default(const char prefijo)
    {   return db_tipos.at(index(prefijo)).valor_default;   }

// Con indice
char Interprete::Tipos_Datos::prefijo(const size_t index)
    {   return db_tipos.at(index).prefijo; }
string Interprete::Tipos_Datos::nombre_completo(const size_t index)
    {   return db_tipos.at(index).nombre_completo;  }
tipo_variable Interprete::Tipos_Datos::valor_default(const size_t index)
    {   return db_tipos.at(index).valor_default;    }

// Con identificador
char Interprete::Tipos_Datos::prefijo(const string identificador)
{
    string prefijo = identificador.substr(0, identificador.find('-'));
    if (prefijo.at(0) == 'F')
        prefijo = prefijo.substr(0, prefijo.find('-'));

    if (prefijo.at(0) == 'C')
        return prefijo.at(1);
    else
        return prefijo.at(0);
}
size_t Interprete::Tipos_Datos::index(const string identificador)
    {   return index(prefijo(identificador));           }
string Interprete::Tipos_Datos::nombre_completo(const string identificador)
    {   return nombre_completo(prefijo(identificador)); }
tipo_variable Interprete::Tipos_Datos::valor_default(const string identificador)
    {   return valor_default(prefijo(identificador));   }

// Comprobaciones
bool Interprete::Tipos_Datos::es_mismo_tipo(const tipo_variable & valor, const size_t index)
{
    return valor.index() == index;
}
bool Interprete::Tipos_Datos::es_mismo_tipo(const tipo_variable & valor, const char prefijo_tipo)
{
    return es_mismo_tipo(valor, index(prefijo_tipo));
}
bool Interprete::Tipos_Datos::es_mismo_tipo(const tipo_variable & valor, const string identificador)
{
    return es_mismo_tipo(valor, prefijo(identificador));
}

/*************************************************************************/
/*CLASE CONTEXTO*/
/*************************************************************************/
void Interprete::Contexto::definir(const string& identificador, tipo_variable valor)
{
    if (identificador.at(0) == 'C' and tabla_simbolos_local.find(identificador) != tabla_simbolos_local.end())
        cerr << TEXTO_AMARILLO << "ERROR: Se ha intentado asignar " << get<string>(valor) << " a la variable constante [" << identificador << "] ya inicializada" << TEXTO_NORMAL << endl; 

    // Comprobamos que el tipo y el valor sean válidos
    // Si se quisiera quitase esto, el lenguaje de programación permitiría que todas las variables tuviesen tipos variantes a lo largo del programa
    //if (prefijos_varibles.find(string(1, prefijo_tipo))->second.first ==  valor.index())
    if (Interprete::Tipos_Datos::es_mismo_tipo(valor, identificador))
        tabla_simbolos_local[identificador] = valor;
    else
        cerr << TEXTO_AMARILLO << "ERROR: Se ha intentado asignar un valor de tipo [" << Interprete::Tipos_Datos::prefijo(valor.index()) << "] a la variable [" << identificador << "] de tipo [" << Interprete::Tipos_Datos::nombre_completo(valor.index()) << "]" << TEXTO_NORMAL << endl;
}

// Obtener una variable (busca en el contexto actual y padres)
tipo_variable Interprete::Contexto::valor_de(const string& identificador) const
{
    // Miro a ver si tengo yo la variable
    if (tabla_simbolos_local.find(identificador) != tabla_simbolos_local.end())
        return tabla_simbolos_local.find(identificador)->second;
    else if (contexto_padre != nullptr) // Si no lo encunetro busco hacia atrás
        return contexto_padre->valor_de(identificador);
    else
    // Como hemos dicho que no hay que declarar variables, si no la encuentra devuelve valor default
    {
        //throw runtime_error("Variable no definida: " + identificador);
        cerr << TEXTO_AMARILLO << "ERROR: Variable [" << identificador <<  "] no inicializada, se usará valor default" << TEXTO_NORMAL << endl;
        //definir(identificador, 0);
        //return tabla_simbolos_local[identificador];
        // No puedo hacer lo que iba a hacer arriba porque estaría inicializando la variable en el contexto global
        return Interprete::Tipos_Datos::valor_default(identificador);
    }
}

/*************************************************************************/
// Clase base para los nodos del AST
/*************************************************************************/
/*
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
        return Contexto->valor_de(Name);
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
        Contexto->definir(Name, value);
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