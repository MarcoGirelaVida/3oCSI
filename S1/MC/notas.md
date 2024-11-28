# Reflexiones previas a la realización del proyecto

## Todo list / Nodos del AST

- Comentarios
- Variables
- Operación aritmética
- Operación booleana
- Contextos
- Ifs
- Prints
- Bucle while
- Bucle for
- (Funciones)
- (Printear el abstract sintax tree)
- (Librería genetic algorithms)

## Conclusiones generales

- Dado que las variables tendrán el formato SIGLA_TIPO-nombre_variable NO HAY que hacer declaraciones, con nombrar una variable ya la declaras, te alerta si intentas usarla sin inicializarla, en el AST habrá que poner un valor default a las variables y si al usarla tiene el valor default dar warning
- Internamente las expresiones aritméticas (y por tanto los números sueltos también) son variables
- AL FINAL VA A SER UN INTERPRETE EN LUGAR DE UN COMPILADOR
- Se conserva el comportamiento de c++ de que si sumas un float y un int de da un float...

## Cuestiones / Reflexiones en general

- Creo que cada nodo del AST es o una operación o una variable (que al final es lo que son los programas)
- ~~Creo que se debe hacer una variable propia para una operación siempre que esa operación HAGA RETURN (por ejemplo, la multiplicación hace un return, los prints no hacen return)~~, corrijo, no es que sea una variable propia, es que ese nodo tiene un valor, y al compilar eso se representa proporcionandole una variable
- Quizás podría en lugar de compilar y hacer un programa en c++ o ensamblador, simplemente ejecutar en tiempo real, simplemente siguiendo el arbol y saltando de nodo en nodo conforme corresponda
- Sería interesante hacer de alguna forma un "debugeador" para ir paso a paso ya que lo voy a hacer como un intérprete -> Ir coginedo en el pipe linea a linea
- También es interesante crear una consola interactiva
- Creo que para las funciones habría que hacer pilas de contextos y que cada variable este asociada a un contexto
- Solo hace falta tener un símbolo de cierre de contexto porque si tuviese uno de apertura y cierre sería imposible hacer de intérprete ya que rara vez vas a tener el de cierre en la misma linea que el de apertura, entonces damos por hecho que las funciones y otras cosas que tienen que abrir contexto lo abren y luego lo cierran cuando toque, si al final del programa la pila no está vacía dará fallo.

## Elementos del tokenizador / Parser

### Declaración variables

#### En el AST

cada variable es un nodo

#### Formato

~~ Declaración variables: tipo (palabras clave) + identificador_variable (texto)~~
~~**CORRECCIÓN**: Declaración variables: declare (palabra clave de declaración) + SIGLA_TIPO-texto~~
**CORRECCIÓN 2**: NO HAY DECLARACIÓN

#### Reflexiones

**REFLEXIÓN**: ¿debería tener cada variable una regla por separado?
**CONCLUSIÓN**: no, basta con separar declaración y asignación

**REFLEXIÓN**: Quizás sería interesante no hacer una declaración y que automáticamente las varibles se declaren conforme salen solo que al compilar se de warning si no está inicializada o algo así.
**COROLARIO** Para ello habría que añadir en el nodo del AST un bool de "inicializada" lo cuál es poco eficiente y mandar un warning siempre que se intente operar con una variable no inicializada.
**REFLEXIÓN DEL COROLARIO** No hace falta, basta con tener un contructor a valores default y ver si tiene el valor default
**CONCLUSIÓN**: Lo voy a hacer así

### Asignación variables

#### En el AST

no está en el AST porque no hay

#### Formato

Asignación: ~~Declaración / llamada + valor correspondiente de cada tipo~~
**CORRECCIÓN:** variable + valor
Dentro de esta regla habrá que hacer un if por cada tipo

#### Reflexiones

**REFLEXIÓN 1** Si el identificador de la variable no existe, no lo encontrará en la tabla hash y dará fallo, eso es tarea del compilador.

**CUESTIÓN 1** ¿Debería hacer una regla de asignación diferente paara cada tipo? Quizás si porque sino no podemos distinguir en la cabecera de la regla el tipo concreto y necesitamos saberlo.
O quizás no, porque declaración / llamada puede englobarse en una regla "variable" y guardarlo en el AST con su correspondiente tipo y luego eso consultarlo dentro de la regla (quizás no es necesario hacer lo de la englobación, pero diría que preferible) y luego hacer que si el valor asignado no corresponde con el tipo, falle.
**CONCLUSIÓN** Creo que así funcionaría bien, también se puede hacer una regla por cada tipo pero creo que es más eficiente y más correcto hacerlo de esta manera.

**CUESTIÓN 2** Ahora, si se hace lo de englobar llamadas y declaraciones en "variable" creo que se generarían nodos de más ya que habrá declaraciones que no se usen para asignar. Probablemente sea mejor simplemente hacer un regla "variable" o "identificador_variable" para el texto a secas y usar variable o declaraciones como catalizador de la llamada.
Por tanto, la regla de declaración de variables quedaría de la siguiente manera:
tipo (palabra clave) + variable / identificador_variable (como lo quiera llamar) (texto a secas)
**CONCLUSIÓN**: No es necesario incluir la declaración como parte de la asignación, primero porque he decidido que no va a existir, pero aunque así fuese son cosas independientes, al final la declaración termina en texto por lo que se pueden "solapar" bastaría con poner antes la regla de declaración para evitar que se de la asignación antes de la declaración

### Prints

#### En el AST

cada print es un nodo pero no una variable

#### Formato

Palabra_clave_string + string

#### Reflexiones

### Tipos de datos

#### En el AST

Cada dato es un nodo e independientemente de que estén en variable o sueltos siempre deben tener una variable asignada (si no la tienen explicitamente se crea una internamente que se declarará al principio del programa)

#### Formato

- string: apertura_string + texto + cierre_string
- float: numero.(algo o nada)
- int: numero

#### Reflexiones

- **CUESTIÓN** ¿Debería hacerse casting de float a int y viceversa?
**CONCLUSIÓN**: En principio voy a hacerlo sin casting, porque sospecho que quizás eso es algo que se pueda o deba hacer a nivel de compilador procesando el AST, es fácil de cambiar de todas formas, es solo añadir una regla más por cada uno

- **REFLEXIÓN 1**: Con esto entendemos que **hay que diferenciar los tipos no solo con sus palabras clave sino como elementos** per sé, ej: string = -texto-, float: numero.numero, int: numero.
- **REFLEXIÓN 2**: Además, puedo en vez de usar palabras clave como float.. usar modificaciones sobre el texto!! como por ejemplo F-texto para identificar a los floats, o de forma más genérica **SIGLA-texto para identificar variables primitivas** y luego los otros tipos de datos que se invente el usuario si los pongo como texto normal

### Operaciones aritméticas

#### En el AST

cada expresión aritmética debe ser un nodo

#### Formato yacc

expresion_aritmetica:
NUMERO
parentesis + expresión_aritmetica + parentesis
expresion_aritmetica + SIMBOLO_* + expresion_aritmetica
expresion_aritmetica + SIMBOLO_/ + expresion_aritmetica
expresion_aritmetica + SIMBOLO_+ + expresion_aritmetica
expresion_arimtecia + SIMBOLO_- + expresion_aritmetica
SIMBOLO_- + NUMERO // Para los números negativos a solas

#### Reflexiones

- **REFLEXIÓN 1**: Hay que tener en cuenta la prioridad de operaciones y parentesis
Para ello se puede usar lo siguiente:
%left '+' '-'
%left '*' '/'
Así yacc procesará primero las multiplicaciones/divisiones vayan antes

- **CUESTIÓN 0**: Hay que usar una pila?
**CONCLUSIÓN 0**: No, de la prioridad de operaciones y esas cosas ya se encarga yacc

- **REFLEXIÓN 2**: Los únicos tokens que harán falta serán los **simbolos de las operaciones** y los **numeros**

- **CUESTION 1**: ¿Cómo almaceno los resultados? ¿Los calculo en tiempo de ejecución? ¿cómo consultan los niveles superiores el valor resultante si no hay un variable asociada?
**CONCLUSIÓN 1**: No se debe calcular en tiempo de ejecución, simplemente "copias y pegas" la operacion y por cada nodo que no es un número como tal creas una variable nueva interna.
**CUESTIÓN 1.1**: En caso de estar traduciendo a cpp quizás debería, durante la compilación, almacenra las variables creadas y al iniciar el programa declararalas.
**CUESTIÓN 1.2**: Quizás solo haría falta una sola variable auxiliar, ya que es secuencial realmente no hay ningún caso en el que haya que "dejarla para después", eso solo pasa con las variables declaradas explicitamente
**REFLEXIÓN de la cuestion 1**: También podría hacer que, internamente, las expresiones aritméticas entren dentro de las reglas de creación de variables, es decir, que internamente cada operación/expresión sea realmente una variable. O MEJOR: que las operaciones sean un nodo del AST con su valor propio
**CUESTIÓN:** En caso de hacerlo en ensamblador no se puede hacer 3+2, habría que desglosarlo en dos pasos, por lo que quizás cobra aún más sentido la idea de la Reflexion de la cuestión 1 de hacer que cada expresión_aritmética sea internamente como una variable

### Operaciones booleanas

#### En el AST

cada expresión booleana debe ser un nodo con su valor asociado

#### Formato

expresion_booleana:
TOKEN_TRUE
TOKEN_FALSE
SIMBOLO_PARENTESIS + expresion_booleana + SIMBOLO_PARENTESIS
expresion_booleana SIMBOLO_or expresion_booleana
expresion_booleana SIMBOLO_and expresion_booleana
expresion_booleana SIMBOLO_igual_que expresion_booleana
expresion_aritmetica SIMBOLO_mayor_que expresion_aritmetica
expresion_aritmetica SIMBOLO_menor_que expresion_aritmetica
expresion_aritmetica SIMBOLO_igual_que expresión_aritmética
// También mayor y menor igual que pero me da pereza ponerlo
SIMBOLO_negación expresion_booleana


### Ifs

#### En el AST

Tendrá asociado una expresión_booleana  y un código
Como tal no debería tener ninguna variable

#### Formato

SIMBOLO_if + expresion_booleana + contexto

#### Reflexiones

**REFLEXION 1**: Los ifs son algo puro del interprete, yacc no debería tener que hacer nada

### Bucles WHILE

#### En el AST

#### Formato

SIMBOLO_while + expresion_booleana + nuevo_contexto

#### Reflexiones

#### Reflexiones

### Bucles FOR

#### En el AST

- Si hago varios tipos de for, debería especificar qué tipo es.
- Variable booleana "continuar"
- Función de evaluación, si tenemos varios tipos, varias opciones según el tipo

#### Formato

SIMBOLO_for + numero + SIMBOLO_iteraciones + nuevo_contexto
SIMBOLO_for + expresion_booleana + nuevo_contexto

#### Reflexiones

**REFLEXIÓN 1:** Debería hacer varios tipos de for? Como uno para iteraciones, otro para expresiones booleanas...

### Funciones

#### En el AST

#### Formato

#### Reflexiones

- **CUESTIÓN 1**: Habría que usar una pila de contextos para que no afecten las variables