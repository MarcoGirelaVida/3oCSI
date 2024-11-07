# PRACTICA 3 - README

## Controles

Simplemente entre al programa y pulse ESC :)
Muy aconsejable usar el ratón. Mucho más cómodo (y me costó implementarlo), se entiende muy rápidamente es casi igual que blender y otros programas 3D.

## Cosas a destacar

### Molino

Si bien tiene un diseño sencillo, el movimiento de las aspas no lo es en absoluto.
Se aconseja poner el viento a velocidad mínima (10km/h) y apreciar el movimiento de las hélices, si se observa detenidamente tiene rozamiento por lo que solo se acelera cuando le llega una brisa. Esto funciona a cualquier velocidad, pero a mayores velocidades es más complicado apreciarlo.
Toma como velocidad angular 90 grados por los segundos que tarde la brisa en recorrer su radio a la velocidad que vaya.

### Girasol

Es la figura más compleja en lo que respecta diseño si bien tampoco es que sea gran cosa.

Algunos detalles destacables:

- NO HAY dos girasoles iguales, las ramas se disponen de formas, ángulos y tamaños aleatorios como podrá comprobar si los observa de cerca.
- Los girasoles se generan de acuerdo a la distribución real que usan las semillas de los girasoles, con las coordenadas polares r = csnt x sqrt(n) angulo = n x 137'5º, que curiosamente es el "angulo aureo".
- No hay una única cápa de pétalos (ni de hojas detras de los mismos) sino 3 que se disponen a distintas alturas y con colores levemente distintos para dar un mayor efecto de profundidad.
- La cabeza se orienta hacia la posición del sol en todo momento, da igual donde se encuentre este y si se desvía de su trayectoria. (Como dato curioso,yo no sabía usar arcotangentes, por lo que me resultó muy costoso).
- Cuando el sol se encuentra a menos de 15 grados sobre el eje x el girasol se pone en su posición de descanso y cierra los pétalos 45 grados.
- IMPORTANTE: Sobre los pétalos, estos son una nueva figura creada desde cero basada en **curvas de bezier**, que han sido un pilar fundamental en mi práctica. Dispone de 2 puntos de curvas. Se puede probar a modificar dichos valores en un caso de ejemplo en el modo de interfaz \[CALIBRACION CURVAS\]. Implementar estas figuras me resultó costoso puesto que no sabía implementar las curvas de bezier (ni lo que eran) pero finalmente lo agradecí enormemente y se usa tanto en el viento como en todas las hojas y pétalos para aportarles curvatura.

### Viento

- Son una serie de láminas hechas con dos curvas de bezier simétricas y a las que se les va alternando los puntos de la curvatura de acuerdo a una onda senoidal.
- El punto más costoso del desarrollo del viento fue algo que aparentemente parece simple: Calcular cada cuanto se deben generar una tanda de láminas y cuando "borrarla" para que se ajuste dentro del area de acción del viento. Esto parece simple pero cuando vas sumando factores como la velocidad, la frecuencia, la amplitud, los desplazamientos... se complica mucho. (Puede comprobar que el código de esa sección está repleto de comentarios)
- Sobre el cálculo de la onda per se no fue complicado, hice un struct onda y simplemente use la fórmula normal.

### Pradera / Suelo

- Sobre el suelo no hay nada que comentar, es solo un cubo verde
- La pradera es una serie de conos modificados de tal forma que se pudiese mover la cúspiede a voluntad, de esta manera es capaz de oscilar al son del viento, cosa que, a diferencia del molino donde fue relativamente sencillo, aquí se complicó mucho y no conseguí que funcionase correctamente. Mi intención es que mientras la brisa estuviese en contacto de una linea, estos oscilase entre el 70% y el 100% de su capacidad mientras que si no está en contacto oscilasen en todo el espectro, estuve provando modificado fases de las ondas y demás pero imposible. Sin embargo actualmente lo que hace es oscilar en el porcentaje del viento que se tiene, si el viento es el 60% del máximo, oscila al 60% de su rango y genera un efecto óptimo responsivo al viento.

### Nubes / Lluvia

- Mi parte favorita y sin embargo de las que menos tiempo me consumió.
- Se compone de una serie de "particulas" que no son más que esferas de tamaños, radios, fases y transparencias aleatorias y que crecen y decrecen de acuerdo a una onda normalizada.
- En el caso de la lluvia, además tienen una velocidad aleatoria, de ahí que hagan el efecto de "espray" tan bonito.
- Por el resto la lluvia es identica al viento, la velocidad, la "tasa de generación" de nuevas láminas... Todos los problemas que solucioné al hacer el viento los apliqué aquí.

### Controles y miscelanía

- Texto: Como habrá podido comprobar si ha leido los controles o si ha hecho zoom out en la escena principal, hay texto en pantalla. Esto lo conseguí con una función de glut, bastante limitada, pero mucho más simple que las alternativas. El principal problema es que no admite los saltos de linea en las cadenas asi que tienes que hacer un salto de linea y calcular el interlineado por cada linea que quieras hacer.
- Sobre los controles: En primer lugar tuve el descubrimiento de que se podían usar los modificadores como CTRL o SHIFT. A continuación descubrí que se podía usar el ratón. Lo que fue un sumidero de tiempo intentando hacer experimentos de "que x figura sigua el puentero". No obstante hice los controles por ratón que habrá experimentado y que fueron extremadamente cómodos para la realización de la práctica.
