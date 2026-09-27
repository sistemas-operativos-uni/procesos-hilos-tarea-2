# Procesos e Hilos: Simulación de un Sistema de Pedidos
 
## 1. Información académica
 
| Campo | Detalle |
|---|---|
| Curso | Sistemas Operativos (SW407) |
| Escuela | Ingeniería de Software — Facultad de Ingeniería Industrial y de Sistemas |
| Universidad | Universidad Nacional de Ingeniería (UNI) |
| Docente | Mg. Ing. José Carlos García La Riva |
| Ciclo | 2026-II |
| Práctica | Práctica Dirigida N.º 03, Semana 3 |
| Tema | Procesos e Hilos |
| Modalidad | Grupal, equipos de 3 a 4 integrantes |
| Duración | 1 semana de trabajo autónomo más sustentación en clase |
| Entorno requerido | Máquina virtual Linux con gcc y Python 3 instalados |
| Nombre del repositorio | procesos-hilos-tarea-2 |
 
## 2. Propósito de la práctica
 
Al finalizar el equipo debe estar en condiciones de:
 
- Aplicar los conceptos de proceso, ciclo de vida, `fork()` y `exec()` en una implementación real en C.
- Aplicar el concepto de hilo y el uso de la biblioteca `threading` de Python para resolver tareas concurrentes.
- Comparar el comportamiento y el costo de la ejecución secuencial frente a la ejecución concurrente.
- Desarrollar habilidades de trabajo colaborativo mediante la asignación de roles dentro del equipo.
## 3. El caso: CocinaPerú Express
 
CocinaPerú Express es una cadena de comida rápida que está digitalizando su sistema de atención de pedidos. El equipo debe construir una simulación que demuestre:
 
1. Cómo el restaurante puede atender varios pedidos de forma simultánea (procesos).
2. Cómo, dentro de cada pedido, varias tareas de cocina pueden prepararse en paralelo (hilos).
La gerencia solicita dos módulos independientes de simulación, descritos en la sección 4.
 
## 4. Alcance técnico del sistema
 
### Módulo 1 — Recepción de pedidos (C)
 
- El proceso principal (mostrador) actúa como punto de entrada y recibe los pedidos.
- Por cada pedido entrante, el mostrador crea un proceso hijo mediante `fork()`.
- Cada proceso hijo reporta su PID y el número de pedido antes de transformarse, mediante `exec()` (`execl`), en el programa que simula la preparación del pedido.
- El proceso principal utiliza `wait()` o `waitpid()` para esperar a que todos los hijos terminen antes de imprimir el mensaje de cierre.
- Cantidad sugerida de pedidos simultáneos: entre 3 y 5 (N_PEDIDOS, a definir por el equipo en el diseño).
### Módulo 2 — Cocina concurrente (Python)
 
- Para un pedido dado, se crea un hilo (`threading.Thread`) por cada tarea de cocina.
- Tareas sugeridas: cortar, freír, emplatar (3 tareas).
- Cada tarea simula trabajo real con `time.sleep()` en lugar de terminar instantáneamente.
- Se usa `start()` para iniciar cada hilo y `join()` para esperar a que todos terminen.
- Se registra el tiempo de inicio y fin con `time.time()` para calcular la duración total concurrente.
- Se implementa además una versión secuencial de las mismas tareas (sin hilos) para obtener un baseline de comparación.
### Medición comparativa
 
- Se calcula la mejora aproximada como: tiempo secuencial dividido entre tiempo concurrente.
- Se documenta, en 3 a 4 líneas, por qué el tiempo concurrente no es exactamente 3 veces menor (limitaciones reales del paralelismo: overhead de creación de hilos, el GIL de Python, planificación del sistema operativo, contención de recursos).
## 5. Conceptos clave involucrados
 
| Concepto | Resumen |
|---|---|
| Proceso | Instancia en ejecución de un programa, con su propio espacio de memoria independiente. |
| Ciclo de vida del proceso | Estados por los que pasa un proceso: nuevo, listo, en ejecución, en espera, terminado. |
| PCB (Process Control Block) | Estructura que el sistema operativo mantiene por cada proceso para gestionar su estado, registros, PID, prioridad, etc. |
| `fork()` | Llamada al sistema (POSIX) que crea un proceso hijo, copia del proceso padre. |
| `exec()` | Llamada al sistema que reemplaza la imagen de memoria del proceso actual por la de otro programa. |
| `wait()` / `waitpid()` | Llamadas que permiten al proceso padre esperar a que uno o varios hijos terminen. |
| Hilo (thread) | Unidad de ejecución dentro de un proceso; los hilos de un mismo proceso comparten memoria. |
| `threading.Thread` | Clase de Python para crear y gestionar hilos. |
| Concurrencia vs. paralelismo real | Distinción relevante al explicar por qué la mejora no es lineal (en Python, el GIL limita el paralelismo real de CPU entre hilos). |
 
## 6. Estructura del repositorio
 
```
procesos-hilos-tarea-2/
├── modulo1_procesos_c/
│   ├── src/
│   │   ├── mostrador.c          (fork + exec + wait)
│   │   └── preparar_pedido.c    (programa ejecutado vía exec)
│   ├── bin/                     (binarios compilados, no versionado)
│   └── Makefile
├── modulo2_hilos_python/
│   ├── cocina_concurrente.py    (threading)
│   ├── cocina_secuencial.py     (baseline sin hilos)
│   └── comparar_tiempos.py      (ejecuta ambos y genera la tabla)
├── docs/
│   ├── diseno/                  (diagrama del Paso 1)
│   └── capturas/
│       ├── modulo1/
│       └── modulo2/
├── informe/                     (informe final en PDF)
├── README.md
├── WORKPLAN.md
├── PROJECT.md
└── .gitignore
```
 
## 7. Flujo de trabajo (workflow)
 
```
Fase 1: Diseño (todo el equipo)
   │
   ├──► Fase 2: Módulo 1 en C (Dev C)      ──┐
   │                                          │
   └──► Fase 3: Módulo 2 en Python (Dev Py) ──┼──► Fase 4: Medición (Tester)
                                               │           │
                                               │           ▼
                                               └──► Fase 5: Informe y sustentación (Coordinador)
```
 
Reglas de dependencia:
 
- La Fase 1 (diseño) bloquea a todas las demás; nada se implementa sin definir antes procesos, hilos y el flujo mostrador-pedidos-cocina.
- Las Fases 2 y 3 son independientes entre sí y se ejecutan en paralelo.
- La Fase 4 no puede iniciar hasta que ambas Fases 2 y 3 estén cerradas, porque necesita ambos módulos funcionando para medir y comparar.
- La Fase 5 depende del cierre de la Fase 4.
## 8. Roles del equipo
 
| Rol | Responsabilidad principal |
|---|---|
| Coordinador(a) | Organiza al equipo, integra las partes del informe, coordina entrega y sustentación. |
| Desarrollador(a) C | Implementa el Módulo 1: creación de procesos con `fork()` y ejecución con `exec()`. |
| Desarrollador(a) Python | Implementa el Módulo 2: simulación de cocineros concurrentes con `threading`. |
| Tester / Analista | Ejecuta las pruebas, mide los tiempos secuencial vs. concurrente y documenta los resultados. |
 
Un mismo integrante puede asumir más de un rol si el equipo tiene 3 personas. El detalle de tareas por integrante, con sus bloqueos específicos, está en `WORKPLAN.md`.
 
## 9. Entregables
 
- Código fuente de ambos módulos (C y Python), organizado en el repositorio del equipo.
- Diagrama de diseño de una página (Paso 1).
- Informe escrito en PDF, que incluye: portada, descripción del caso, diagrama de diseño, explicación del código de ambos módulos, tabla comparativa de tiempos con interpretación, capturas de pantalla de la ejecución de ambos módulos y conclusiones del equipo.
- Capturas de pantalla de la ejecución de cada módulo.
- Sustentación grupal en clase, de 5 a 8 minutos, con demostración en vivo de ambos módulos.
## 10. Criterios de evaluación (20 puntos, grupal)
 
| Criterio | Puntaje |
|---|---|
| Comprensión conceptual del caso | 3 pts |
| Módulo 1 (fork/exec en C) | 4 pts |
| Módulo 2 (threading en Python) | 4 pts |
| Medición y análisis comparativo | 3 pts |
| Documentación e informe escrito | 3 pts |
| Sustentación grupal | 3 pts |
 
Nota sobre la sustentación: todos los integrantes deben poder explicar cualquier parte del trabajo, no solo la de su rol asignado. Se debe preparar respuestas sobre el ciclo de vida del proceso, el PCB, y las diferencias entre `fork()`/`exec()` e hilos.
 
El docente puede aplicar un factor de coevaluación entre pares para ajustar la nota individual según la participación real en el equipo.
 
## 11. Entorno de desarrollo
 
- El setup inicial del repositorio (estructura de carpetas, README, WORKPLAN, PROJECT, .gitignore, git init) no requiere máquina virtual y puede hacerse directamente en Windows.
- La compilación y ejecución del Módulo 1 requiere obligatoriamente un entorno Linux, ya que `fork()`, `exec()` y `wait()` son llamadas al sistema POSIX que no existen de forma nativa en Windows.
- La ejecución del Módulo 2 y la toma de capturas de pantalla también deben realizarse desde la máquina virtual Linux, por consistencia con el entorno exigido por el docente y porque las capturas son evidencia obligatoria de la rúbrica.
- Herramientas requeridas en la VM: `gcc` (`sudo apt install build-essential`) y Python 3.
## 12. Referencias del curso
 
- Silberschatz, A., Galvin, P. B., y Gagne, G. (2018). *Operating System Concepts* (10th ed.). Wiley — Capítulos 3 (Processes) y 4 (Threads).
- Tanenbaum, A. S., y Bos, H. (2015). *Modern Operating Systems* (4th ed.). Pearson.
- The Linux man-pages project (2026). `fork(2)`, `execve(2)` y `wait(2)`, man7.org.
- Python Software Foundation (2026). `threading` — Thread-based parallelism, docs.python.org.
- Universidad Nacional de Ingeniería (2026). Sílabo SW407 — Sistemas Operativos, Escuela de Ingeniería de Software.
 