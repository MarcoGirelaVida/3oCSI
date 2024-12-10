# Componentes abstractos de mi lenguaje

## Contexto

- Cuerpo (vector de instrucciones)

## Instruccion

### Definicion de función

- Identificador función: string
- argumentos: Argumentos
- Cuerpo: Contexto
- (Tipo de return)

### Return

- Valor: Expresion

### Asignación

- Objetivo: Identificador (o set, map, vector... si decido implementarlo)
- Valor: Expresión

### Bucle for

- Inicio: Expresion
- Fin: Expresion
- Paso: Expresion
- Cuerpo: Contexto
- (Elemento sobre el que se itera)

### Bucle while

- Condicion: Expresion
- Cuerpo: Contexto

### Condicional

- Condicion: Expresion
- Cuerpo_si_ocurre: Expresion
- Cuerpo_si_no_ocurre: Expresion

### Expresión_

- Valor: Expresion

## Expresión

- Tipo
- Valor

### Operación binaria

- Operando_1: Expresión
- Operador: + | - | * | / | %
- Operando_2: Expresión

### Operacion unaria

- Operador: + | - | !
- Operando: Expresión

### Operacion booleana

- Operador booleano: and | or
- Lista de expresiones (valores)

### Comparacion

- Operando 1: Expresión
- Lista de operadores de comparación: Igual | Distinto | Menor | Menor= | Mayor | Mayor=
- Lista de operadores: Lista de expresiones

### Llamada función

- Identificador¿?
- Argumentos

### Constante

- Valor: Int, float, string, {3,4,5,5} (vector literal)

### Set / Maps / Vectores

### Identificador

- Nombre_Identificador
- Contexto

## Argumentos

- Lista de expresiones

## Parametros (atributos de una función)

- Lista de identificadores
