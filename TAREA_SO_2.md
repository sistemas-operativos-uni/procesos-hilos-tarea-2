UNIVERSIDAD NACIONAL DE
INGENIERÍA Escuela de
Ingeniería de Software
PRÁCTICA DIRIGIDA N.º 03* · SEMANA 3
Procesos e Hilos: Simulación de un Sistema de Pedidos
Curso Sistemas Operativos (SW407)
Escuela Ingeniería de Software — Facultad de Ingeniería Industrial y de Sistemas, UNI
Tema Semana 3 — Procesos e Hilos
Docente Mg. Ing. José Carlos, García La Riva
Ciclo / Semestre 2026-II*
Modalidad Grupal — equipos de 3 a 4 integrantes
Duración estimada 1 semana de trabajo autónomo + sustentación en clase*
Entorno requerido Máquina virtual Linux (ver Taller de VirtualBox) con gcc y Python 3 instalados
* Dato asumido para fines de plantilla — verificar y ajustar antes de la publicación oficial.
I. PROPÓSITO DE LA PRÁCTICA
Al finalizar esta práctica dirigida, el equipo estará en condiciones de:
● Aplicar los conceptos de proceso, ciclo de vida, fork() y exec() en una implementación real en lenguaje C. ● Aplicar el
concepto de hilo (thread) y el uso de la biblioteca threading de Python para resolver tareas concurrentes. ● Comparar el
comportamiento y el costo de la ejecución secuencial frente a la ejecución concurrente. ● Desarrollar habilidades de
trabajo colaborativo mediante la asignación de roles dentro del equipo.
II. EL CASO: "COCINAPERÚ EXPRESS"
CocinaPerú Express es una cadena de comida rápida que está digitalizando su sistema de atención
de pedidos. Contratan a tu equipo para construir una simulación que demuestre cómo el
restaurante puede atender varios pedidos de forma simultánea, y cómo, dentro de cada pedido,
varias tareas de cocina pueden prepararse en paralelo.
La gerencia solicita dos módulos de simulación:
Módulo 1 — "Recepción de pedidos" (en C): cada pedido que llega debe convertirse en un
proceso independiente. El proceso principal (el mostrador) recibe los pedidos y crea un proceso
hijo por cada uno usando fork(); ese proceso hijo reporta su identificador (PID) y ejecuta,
mediante exec(), un programa que simula la preparación del pedido.
Módulo 2 — "Cocina concurrente" (en Python): dentro de un mismo pedido, la cocina debe
repartir el trabajo entre varios cocineros (hilos) que trabajan al mismo tiempo sobre tareas
distintas de ese pedido (por ejemplo: cortar, freír, emplatar), midiendo cuánto se reduce el tiempo
total de preparación al trabajar en paralelo frente a hacerlo de forma secuencial.

III. CONFORMACIÓN DEL EQUIPO Y ROLES
SW407 — Sistemas Operativos — Práctica Dirigida Página 1 de 5
UNIVERSIDAD NACIONAL DE INGENIERÍA
Escuela de Ingeniería de Software
Formen equipos de 3 a 4 integrantes y asignen los siguientes roles. Un mismo integrante puede asumir más de un rol
si el equipo tiene 3 personas.
Rol Responsabilidad principal
Coordinador(a) Organiza al equipo, integra las partes del informe y coordina la entrega y
la sustentación.
Desarrollador(a) C Implementa el Módulo 1: creación de procesos con fork() y ejecución con exec().
Desarrollador(a) Implementa el Módulo 2: simulación de cocineros concurrentes con threading.
Python
Tester / Analista Ejecuta las pruebas, mide los tiempos secuencial vs. concurrente y documenta
los resultados.
IV. PASOS DE IMPLEMENTACIÓN
Paso 1. Diseñar el caso
Antes de programar, el equipo debe traducir el caso a los conceptos del curso: decidan qué elementos serán procesos
y cuáles serán hilos, y esbocen un diagrama simple del flujo completo (mostrador → pedidos → cocina).
● Identifiquen cuántos pedidos simultáneos simulará el Módulo 1 (sugerido: entre 3 y 5).
● Identifiquen las tareas de cocina que se repartirán entre hilos en el Módulo 2 (sugerido: 3 tareas, p. ej. cortar, freír,
emplatar).
● Entregable de este paso: un diagrama de una página (a mano o digital) incluido en el informe final.
Paso 2. Preparar el entorno de trabajo
Verifiquen que la máquina virtual Linux del taller de VirtualBox cuenta con las herramientas necesarias para ambos
módulos.
● Compilador de C: gcc --version (instalar con sudo apt install build-essential si falta).
● Python 3: python3 --version (incluido por defecto en la mayoría de distribuciones Linux).
● Definan una carpeta o repositorio compartido del equipo para el código y el informe.
Paso 3. Implementar el Módulo 1 — Recepción de pedidos (C)
El proceso principal (mostrador) debe crear un proceso hijo por cada pedido entrante usando fork(); cada hijo reporta
su PID y ejecuta, mediante exec(), un programa que simule la preparación (puede ser un script que imprima mensajes
con sleep()). El proceso principal debe esperar a que todos los pedidos terminen antes de cerrar.
● Usen un ciclo en el proceso principal para generar N pedidos (fork() por cada uno).
● En cada proceso hijo, impriman un mensaje con su PID y el número de pedido antes de llamar a exec(). ● Usen
wait() o waitpid() en el proceso principal para esperar a todos los hijos antes de imprimir el mensaje final.

// Fragmento de referencia — mostrador.c
for (int i = 1; i <= N_PEDIDOS; i++) {
pid_t pid = fork();
if (pid == 0) {
printf("Pedido #%d creado (PID %d)\n", i, getpid());
execl("./preparar_pedido", "preparar_pedido", NULL);
perror("execl"); // solo se alcanza si exec falla
exit(1);
SW407 — Sistemas Operativos — Práctica Dirigida Página 2 de 5
UNIVERSIDAD NACIONAL DE INGENIERÍA
Escuela de Ingeniería de Software
}
}
for (int i = 0; i < N_PEDIDOS; i++) wait(NULL);
printf("Restaurante cerrado: todos los pedidos fueron atendidos\n");
Paso 4. Implementar el Módulo 2 — Cocina concurrente (Python)
Para un pedido dado, creen un hilo por cada tarea de cocina y ejecútenlos de forma concurrente. Midan el tiempo
total con el módulo time.
● Cada tarea debe simular trabajo real con time.sleep() (p. ej. 2 segundos) en lugar de terminar instantáneamente. ●
Usen threading.Thread para cada tarea, start() para iniciarlas y join() para esperar a que todas terminen. ● Registren
el tiempo de inicio y fin con time.time() para calcular la duración total concurrente.
# Fragmento de referencia — cocina.py
import threading, time
def tarea(nombre, duracion):
time.sleep(duracion)
print(f"{nombre} completada")
tareas = ["cortar", "freír", "emplatar"]
inicio = time.time()
hilos = [threading.Thread(target=tarea, args=(t, 2))
for t in tareas]
for h in hilos: h.start()
for h in hilos: h.join()
print(f"Tiempo concurrente: {time.time() - inicio:.2f} s")
Paso 5. Medir y comparar: secuencial vs. concurrente
Ejecuten las mismas tareas de cocina de forma secuencial (una función que las llame una tras otra, sin hilos) y
comparen el tiempo total contra la versión concurrente del Paso 4.
● Calculen la mejora aproximada: tiempo secuencial ÷ tiempo concurrente.
● Presenten los dos tiempos y la mejora en una tabla dentro del informe.
● Expliquen en 3-4 líneas por qué el tiempo concurrente no es exactamente 3 veces menor (limitaciones reales del
paralelismo).
Paso 6. Documentar los resultados
Redacten un informe grupal que integre ambos módulos y sus resultados.
● Portada, descripción del caso y el diagrama de diseño del Paso 1.

● Explicación del código de ambos módulos (qué hace cada parte, no solo pegar el código). ●
Tabla comparativa de tiempos (secuencial vs. concurrente) con su interpretación.
● Capturas de pantalla de la ejecución de ambos módulos.
● Conclusiones del equipo sobre lo aprendido.
Paso 7. Preparar la sustentación grupal
Cada equipo presentará su solución en clase (5 a 8 minutos), demostrando en vivo ambos módulos en
ejecución. ● Todos los integrantes deben poder explicar cualquier parte del trabajo, no solo la de su rol asignado.
SW407 — Sistemas Operativos — Práctica Dirigida Página 3 de 5
UNIVERSIDAD NACIONAL DE
INGENIERÍA Escuela de
Ingeniería de Software
● Preparen respuestas para preguntas sobre el ciclo de vida del proceso, el PCB, y las diferencias entre fork()/exec() e
hilos.
V. ENTREGABLES
● Código fuente de ambos módulos (C y Python), organizado en la carpeta o repositorio del equipo. ●
Informe escrito en PDF, según lo indicado en el Paso 6.
● Capturas de pantalla de la ejecución de cada módulo.
● Sustentación grupal en la fecha indicada por el docente.*
VI. RÚBRICA DE EVALUACIÓN
La práctica se califica sobre 20 puntos, aplicados de forma grupal según los siguientes criterios.
| Criterio   | Ptje.   Excelente   | Bueno   | Regular   | Deficiente  |
| ---------- | ------------------- | ------- | --------- | ----------- |
Comprensión    3 pts  Traduce con  Traduce  Aplica los conceptos  No logra traducir el
conceptual del caso   precisión  el caso a  correctamente  el  de  forma parcial o  caso a los
|     | procesos e  hilos,   | caso, con alguna    | con                  | conceptos de     |
| --- | -------------------- | ------------------- | -------------------- | ---------------- |
|     | justificando cada    | justificación       | confusiones entre    | proceso e hilo.  |
|     | decisión de diseño.  | incompleta.         | proceso e hilo.      |                  |
Módulo 1 —  4 pts  Crea procesos hijos    Implementación    Implementación    No logra
fork() /  exec() en  correctamente, usa    funcional con  parcial: fork()  implementar  la
|     | exec() y espera su    | detalles  menores  | funciona  pero  | creación de  |
| --- | --------------------- | ------------------ | --------------- | ------------ |
C   finalización    (p. ej. no    exec() falla o el   procesos  o el
|     | (wait/waitpid) sin    | siempre espera a    | flujo es incorrecto.  | código no         |
| --- | --------------------- | ------------------- | --------------------- | ----------------- |
|     | errores.              | todos  los hijos).  |                       | compila/ejecuta.  |
Módulo 2 —  4 pts  Crea e inicia los  Implementación    Implementación    No logra
|     | hilos   | funcional  con  | parcial: los hilos se    | implementar  los  |
| --- | ------- | --------------- | ------------------------ | ----------------- |
threading  en
|     | correctamente,    | detalles  menores  | crean pero no se    | hilos o el código no   |
| --- | ----------------- | ------------------ | ------------------- | ---------------------- |
Python
|     | sincroniza su     | en la gestión  de  | ejecutan de forma    | compila/ejecuta.  |
| --- | ----------------- | ------------------ | -------------------- | ----------------- |
|     | finalización con  | los hilos.         | realmente            |                   |
|     | join() y  el      |                    | concurrente.         |                   |
resultado es
consistente.
Medición y  3 pts  Mide con precisión  Mide los tiempos    Mide los tiempos  No presenta
|     | los  tiempos  | correctamente,  | de  forma  | medición  de  |
| --- | ------------- | --------------- | ---------- | ------------- |
análisis
|     | secuencial y   | con  una  | incompleta o  con  | tiempos ni    |
| --- | -------------- | --------- | ------------------ | ------------- |
comparativo
|     | concurrente,          | interpretación    | una interpretación   | comparación entre    |
| --- | --------------------- | ----------------- | -------------------- | -------------------- |
|     | calcula la  mejora y  | básica de los     | poco clara.          | enfoques.            |
|     | la interpreta         | resultados.       |                      |                      |
correctamente.

Documentación e    3 pts  Informe completo,    Informe completo  Informe  No presenta
| claro y bien    | con  alguna  | incompleto o  con  | informe o  el  |
| --------------- | ------------ | ------------------ | -------------- |
informe escrito
| estructurado, con       | sección poco   | secciones clave    | contenido no         |
| ----------------------- | -------------- | ------------------ | -------------------- |
| diagrama de             | desarrollada.  | ausentes           | corresponde al       |
| diseño,  código         |                | (diagrama,         | trabajo  realizado.  |
| explicado y             |                | evidencia o        |                      |
| capturas de evidencia.  |                | explicación).      |                      |
Sustentación grupal   3 pts  Todo el equipo  El equipo explica    La sustentación es    El equipo no puede
| domina  el tema,    | correctamente el    | superficial           | o  solo  sustentar su propio    |
| ------------------- | ------------------- | --------------------- | ------------------------------- |
| explica sus         | trabajo, con        | uno  o                |   dos  trabajo.                 |
| decisiones de       | participación       | integrantes pueden    |                                 |
| diseño y  responde  | desigual  entre     | explicar el trabajo.  |                                 |
| con                 | integrantes.        |                       |                                 |
seguridad las
preguntas del
docente.

Puntaje total: 20 puntos. El puntaje obtenido se integra a la nota de Prácticas (PP) del curso, según la fórmula de
evaluación vigente.*
* El docente puede aplicar un factor de evaluación individual (coevaluación entre pares) para ajustar la nota final de cada integrante
según su participación real en el equipo.
SW407 — Sistemas Operativos — Práctica Dirigida Página 4 de 5
UNIVERSIDAD NACIONAL DE
INGENIERÍA Escuela de
Ingeniería de Software
VII. REFERENCIAS
Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.). Wiley — Capítulos 3
(Processes) y 4 (Threads).
Tanenbaum, A. S., & Bos, H. (2015). Modern Operating Systems (4th ed.). Pearson.
The Linux man-pages project (2026). fork(2), execve(2) y wait(2) — manual pages. man7.org Python Software
Foundation (2026). threading — Thread-based parallelism. docs.python.org/3/library/threading.html Universidad
Nacional de Ingeniería (2026). Sílabo SW407 — Sistemas Operativos, Escuela de Ingeniería de Software.

SW407 — Sistemas Operativos — Práctica Dirigida Página 5 de 5