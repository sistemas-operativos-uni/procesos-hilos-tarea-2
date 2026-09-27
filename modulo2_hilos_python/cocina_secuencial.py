"""
cocina_secuencial.py
---------------------
Módulo 2 -- "Cocina concurrente" (CocinaPerú Express)

Define las tareas de cocina del pedido y la versión SECUENCIAL: una
función que llama a las tareas una tras otra, sin hilos.

Este archivo se puede:
  1) Ejecutar solo, para ver únicamente la preparación secuencial:
         python3 cocina_secuencial.py
  2) Importar desde otros módulos (cocina_concurrente.py y
     comparar_tiempos.py reutilizan TAREAS y tarea() desde aquí, para no
     duplicar código).
"""

import time

# Tareas de cocina y su duración simulada (en segundos). En la vida real
# cada tarea tomaría un tiempo distinto; aquí se simula con time.sleep()
# para representar "trabajo real" de cocina.
TAREAS = [
    ("cortar", 2),
    ("freir", 2),
    ("emplatar", 2),
]


def tarea(nombre, duracion):
    """Simula el trabajo de un cocinero en una tarea específica."""
    print(f"  -> {nombre}: iniciando (dura {duracion}s)")
    time.sleep(duracion)
    print(f"  -> {nombre}: completada")


def ejecutar_secuencial(tareas):
    """Ejecuta todas las tareas una tras otra, en un solo 'cocinero'.
    Devuelve el tiempo total que tomó."""
    print("\n--- Preparación SECUENCIAL (un solo cocinero) ---")
    inicio = time.time()

    for nombre, duracion in tareas:
        tarea(nombre, duracion)

    total = time.time() - inicio
    print(f"Tiempo total secuencial: {total:.2f} s")
    return total


def main():
    print("=== CocinaPerú Express -- Preparación secuencial ===")
    print(f"Tareas del pedido: {[t[0] for t in TAREAS]}")
    ejecutar_secuencial(TAREAS)


if __name__ == "__main__":
    main()
