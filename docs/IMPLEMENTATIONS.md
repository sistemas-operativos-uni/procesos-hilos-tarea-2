# IMPLEMENTATIONS

Documento técnico de la Práctica Dirigida N.º 03 (SW407, Sistemas Operativos, UNI-FIIS): "CocinaPerú Express". Describe qué se construyó, en qué orden y por qué, para que cualquier integrante del equipo pueda entender y sustentar el sistema completo sin depender de haber escrito una parte específica.

# 1. Introducción al proyecto

CocinaPerú Express es una simulación de un sistema de pedidos de comida rápida, diseñada para demostrar dos conceptos centrales del curso: procesos y sus llamadas al sistema (fork, exec, wait), e hilos con la biblioteca threading de Python.

El sistema se divide en dos módulos independientes que representan dos capas distintas de concurrencia:

- Módulo 1 (C): simula la recepción de pedidos. Cada pedido que llega se convierte en un proceso del sistema operativo, no en una simple función. Esto obliga a usar fork() para crear el proceso, exec() para que ese proceso ejecute un programa distinto, y wait() para que el proceso principal no cierre el restaurante antes de que todos los pedidos terminen.

- Módulo 2 (Python): simula la cocina dentro de un pedido ya aceptado. En vez de procesos, aquí se usan hilos: varias tareas de cocina (cortar, freír, emplatar) se reparten entre hilos que corren al mismo tiempo, y se compara ese tiempo contra hacerlas una por una.

La idea de fondo es que un pedido no es una sola unidad de trabajo indivisible: hacia afuera (frente a otros pedidos) se comporta como un proceso independiente; hacia adentro (frente a sus propias tareas de cocina) se reparte en hilos. Los dos módulos no están conectados por código -- son programas separados -- pero sí están conectados conceptualmente, y esa relación se explica con el diagrama de la sección 2.

# 2. Setup inicial

## Fase 0: estructura del repositorio

Antes de escribir cualquier línea de C o Python, se definió la organización del repositorio para que cada módulo, cada resultado y cada documento tuviera un lugar fijo:

```
procesos-hilos-tarea-2/
├── modulo1_procesos_c/
│   ├── src/
│   │   ├── mostrador.c
│   │   └── preparar_pedido.c
│   ├── bin/              (generado por make, no se versiona)
│   └── Makefile
├── modulo2_hilos_python/
│   ├── cocina_secuencial.py
│   ├── cocina_concurrente.py
│   └── comparar_tiempos.py
├── docs/
│   ├── context/          (copias de los documentos de planificación)
│   ├── diagrams/         (diagramas de diseño)
│   └── capturas/         (capturas de ejecución de ambos módulos)
├── informe/              (informe final en PDF)
├── README.md
├── WORKPLAN.md
├── PROYECT.md
└── .gitignore
```

Esta separación en dos carpetas de nivel superior (modulo1_procesos_c, modulo2_hilos_python) fue deliberada: como son dos lenguajes y dos paradigmas de concurrencia distintos, mezclarlos en una sola carpeta habría dificultado explicar en la sustentación cuál código corresponde a qué concepto.

El .gitignore excluye los binarios compilados de C (bin/), el caché de Python (__pycache__/) y archivos de sistema, para que el repositorio solo contenga código fuente y documentación, no artefactos que cada máquina puede regenerar con make o python3.

## Diagrama de flujo del sistema

Se diseñaron tres diagramas, guardados en docs/diagrams/:

- Uno para el Módulo 1: muestra al Mostrador (proceso padre) creando 5 procesos hijo con fork(), cada uno transformándose con execl(), y el padre esperando a los 5 con wait() antes de anunciar el cierre.

- Uno para el Módulo 2: muestra un pedido lanzando 3 hilos (cortar, freír, emplatar) con start(), y el hilo principal esperando a los 3 con join() antes de calcular el tiempo total.

- Un diagrama integrado (SVG-FULL-DIAGRAM.svg) que junta los dos anteriores en una sola página, con una flecha punteada que conecta el cierre del Módulo 1 con el inicio del Módulo 2. La flecha es punteada, y no continua, porque en el código real no hay una llamada que una un módulo con el otro -- son dos simulaciones independientes que se ejecutan por separado. La flecha representa la relación conceptual (cada pedido del mostrador tendría, en un sistema real, una cocina concurrente detrás), no una dependencia de código.

En términos de texto, el flujo completo se resume así:

```
Mostrador (proceso padre)
   |
   |  fork() x5
   v
Pedido #1 .. Pedido #5 (procesos hijo)
   |
   |  execl()
   v
preparar_pedido (simula 2s de cocina por pedido)
   |
   |  wait() x5
   v
Restaurante cerrado
   .
   .  (relación conceptual, no de código)
   v
Un pedido, por dentro (Módulo 2)
   |
   |  threading.Thread + start() x3
   v
Hilo: cortar / Hilo: freír / Hilo: emplatar
   |
   |  join() x3
   v
Tiempo total de cocina calculado
```

# 3. Implementación

## Fase 1: diseño

Se definieron dos decisiones que condicionan todo el código posterior:

- Cantidad de pedidos simultáneos en el Módulo 1: 5 (N_PEDIDOS = 5 en mostrador.c). El enunciado sugería entre 3 y 5; se tomó el límite superior para que la concurrencia entre procesos sea más visible en la salida por consola.

- Tareas de cocina del Módulo 2: cortar, freír, emplatar, cada una simulada con 2 segundos de sleep(). Son las tres tareas sugeridas por el enunciado, y se mantuvieron sin agregar una cuarta porque no aportaba nada distinto a la comparación secuencial contra concurrente que pide la rúbrica.

## Fase 2: Módulo 1 -- Recepción de pedidos (C)

El módulo se compone de dos programas y un Makefile.

mostrador.c es el proceso padre. Su lógica central es un ciclo que repite fork() cinco veces:

- Si fork() devuelve un valor menor a 0, la creación del proceso falló (por ejemplo, por falta de recursos del sistema). En ese caso se imprime el error con perror() y se corta el ciclo con break, no con exit(). La razón de usar break y no exit() es que salir del programa en ese punto abandonaría a los hijos que sí se habían creado en las iteraciones anteriores, dejándolos sin que el padre los recoja con wait(). break permite seguir con el resto del programa y esperar igual a los que ya existen.

- Si fork() devuelve 0, ese bloque de código lo ejecuta el hijo. El hijo imprime su propio PID y el número de pedido que le tocó, y luego llama a execl("./preparar_pedido", "preparar_pedido", numero_str, NULL). Esta llamada reemplaza por completo la memoria del proceso hijo por el programa preparar_pedido -- el hijo deja de ejecutar código de mostrador.c a partir de esa línea. Si execl() falla (por ejemplo, porque el binario no existe en el directorio actual), la llamada retorna en vez de reemplazar el proceso; por eso justo después se imprime el error con perror() y se termina el hijo con exit(1), para que no siga ejecutando por accidente código pensado para el padre.

- Si fork() devuelve un valor mayor a 0, ese bloque lo ejecuta el padre, que simplemente continúa el ciclo para crear el siguiente pedido.

Después del ciclo, el padre entra en un segundo ciclo que llama a wait() repetidamente hasta que la función devuelve -1 con errno igual a ECHILD, que es la señal del sistema operativo de que ya no quedan procesos hijo pendientes. Por cada hijo recogido se imprime su PID y su código de salida, usando las macros WIFEXITED y WEXITSTATUS para leer ese código de forma segura. Este código de salida es la evidencia de que exec() funcionó: si el hijo llegó a ejecutar preparar_pedido y este terminó con return 0, el código de salida es 0; si exec() hubiera fallado, el código sería 1.

Solo después de que el ciclo de wait() termina se imprime el mensaje de cierre del restaurante, garantizando que ningún proceso hijo queda en estado zombie.

preparar_pedido.c es el programa que cada hijo ejecuta vía exec(). Es intencionalmente mínimo: recibe el número de pedido como argumento de línea de comandos (el mismo que execl() le pasó desde mostrador.c), imprime que empieza a preparar el pedido, simula el trabajo con sleep(2), e imprime que terminó. No necesita más lógica porque su único propósito es representar el tiempo real que toma cocinar, no procesar datos.

Un detalle importante para la ejecución: mostrador busca preparar_pedido con una ruta relativa (./preparar_pedido), así que el binario debe correrse desde la misma carpeta donde vive preparar_pedido -- en este caso, desde modulo1_procesos_c/bin/, que es donde el Makefile deja ambos binarios compilados.

## Fase 3: Módulo 2 -- Cocina concurrente (Python)

El módulo se divide en tres archivos que se importan entre sí para no duplicar código:

cocina_secuencial.py define la lista TAREAS (los pares nombre-duración de cada tarea de cocina) y la función tarea(), que simula el trabajo de un cocinero con time.sleep(). También define ejecutar_secuencial(), que recorre las tareas una por una, llamando a tarea() y esperando a que cada una termine antes de pasar a la siguiente. El tiempo total se mide con time.time() antes y después del ciclo.

cocina_concurrente.py importa TAREAS y tarea() desde cocina_secuencial.py -- esto asegura que ambas versiones preparan exactamente las mismas tareas, con las mismas duraciones, y que la comparación de tiempos sea justa. Define ejecutar_concurrente(), que crea un objeto threading.Thread por cada tarea, los inicia a todos con start(), y luego espera a todos con join() antes de calcular el tiempo total. La diferencia clave frente a la versión secuencial es que los start() se ejecutan en un ciclo separado del de los join(): si se llamara a join() justo después de cada start(), el hilo principal esperaría a que cada hilo termine antes de arrancar el siguiente, y el resultado sería equivalente a la versión secuencial.

comparar_tiempos.py es el punto de entrada que un usuario ejecuta directamente. Llama a ejecutar_secuencial() y luego a ejecutar_concurrente() sobre el mismo conjunto de tareas, calcula la mejora como el tiempo secuencial dividido entre el tiempo concurrente, e imprime una tabla comparativa con cuatro decimales de precisión. Esa precisión no es arbitraria: con dos decimales el resultado se redondea a un valor como 3.00x, que oculta el pequeño costo real de crear y planificar los hilos; con cuatro decimales ese costo se alcanza a ver (ver sección 4).

## Fase 4: medición y comparación

Esta fase no agrega código nuevo -- consiste en ejecutar ambos módulos, capturar su salida como evidencia, y verificar que el comportamiento observado coincide con lo que la teoría predice (concurrencia real entre procesos, concurrencia real entre hilos cuando el trabajo simulado no compite por el intérprete de Python). Los resultados obtenidos se documentan en la sección 4 de este archivo.

# 4. Resultados

## Módulo 1

Al ejecutar ./mostrador desde modulo1_procesos_c/bin/, los 5 procesos se crean y anuncian su PID en un orden distinto al de creación (por ejemplo, el pedido #2 puede anunciarse antes que el #1). Esto no es un error: es la evidencia de que el sistema operativo, no el programa, decide el orden real en que se planifican los procesos. Los 5 procesos duermen 2 segundos cada uno de forma simultánea, por lo que el programa completo termina en aproximadamente 2 segundos, no en 5 x 2 = 10 segundos, que es lo que tomaría si los pedidos se atendieran uno detrás de otro. El padre recoge a los 5 hijos con wait(), cada uno reportando código de salida 0, y solo entonces imprime el mensaje de cierre.

## Módulo 2

La ejecución de comparar_tiempos.py sobre las tareas cortar, freír y emplatar (2 segundos cada una) dio los siguientes resultados:

| Modo         | Tiempo (s) |
|--------------|-----------:|
| Secuencial   |     6.0101 |
| Concurrente  |     2.0033 |
| Mejora       | 3.0002x    |

La versión secuencial tomó poco más de 6 segundos, como se espera de sumar tres esperas de 2 segundos una tras otra. La versión concurrente tomó poco más de 2 segundos, porque las tres esperas ocurrieron al mismo tiempo en tres hilos distintos.

La mejora obtenida (3.0002x) está muy cerca del ideal teórico de 3x, pero no es exactamente 3x, y esa diferencia es la que pide explicar el enunciado. La razón concreta en este caso es doble:

Primero, time.sleep() libera el GIL (Global Interpreter Lock) de Python mientras el hilo está dormido, por lo que durante esos 2 segundos los tres hilos sí corren en paralelo de verdad, sin bloquearse entre sí por el GIL. Esto explica por qué la mejora se acerca tanto al ideal de 3x, a diferencia de lo que ocurriría si las tareas hicieran cálculo puro de CPU en vez de dormir: en ese caso el GIL sí obligaría a los hilos a turnarse, y la mejora real sería mucho menor a 3x.

Segundo, el pequeño desvío que sí aparece (0.0002x, del orden de milisegundos) corresponde al costo de crear los tres objetos Thread, iniciarlos con start(), y a que el sistema operativo los planifique -- ese overhead existe siempre que se usan hilos, sin importar qué tarea ejecuten, y es la razón de fondo por la que ninguna paralelización real llega a ser exactamente N veces más rápida que su versión secuencial con N tareas.

## Conclusión de la comparación

Los dos módulos demuestran el mismo principio en dos niveles distintos: repartir trabajo entre varias unidades de ejecución (procesos en el Módulo 1, hilos en el Módulo 2) reduce el tiempo total frente a atender ese mismo trabajo uno por uno, pero ese beneficio nunca es perfectamente lineal -- siempre hay un costo de crear, iniciar y sincronizar esas unidades de ejecución que la versión secuencial no paga.
