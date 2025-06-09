# Cons-R3

Este programa en C analiza un conjunto de vectores en R3 (leídos desde un archivo de entrada) y determina la estructura geométrica del cono que generan. Luego calcula un sistema minimal de generadores (los "vectores esenciales") y lo guarda en un archivo de salida.

🧩 ¿Qué hace paso a paso?
Lectura de entrada:
Lee de un archivo (argv[1]) una lista de vectores en 
𝑅
3
R 
3
 .
Cada línea debe tener tres componentes reales separadas por comas.

Validaciones:
Verifica que no haya vectores nulos (prohibidos).
Reserva memoria para almacenar los vectores y los resultados.

Clasificación del cono generado:
Calcula el tipo de cono generado por los vectores. Puede ser:

0: {0} (conjunto trivial)

1: Semirecta

2: Recta

3: Ángulo plano

4: Semiplano

5: Plano

6: Cono poliédrico (con varias aristas)

7: Ángulo diédrico

8: Semiespacio

9: Espacio completo (
𝑅
3
R 
3
 )

Lo hace mediante proyecciones ortogonales, producto escalar, ángulos y cálculo de determinantes.

Cálculo de generadores mínimos:
Determina qué subconjunto mínimo de los vectores originales sigue generando el mismo cono.
Elimina los vectores redundantes uno por uno de forma recursiva.

Salida del resultado:

Muestra por consola el tipo de cono y los índices de los vectores generadores mínimos.

Escribe esos vectores mínimos en el archivo de salida (argv[2]), uno por línea.

