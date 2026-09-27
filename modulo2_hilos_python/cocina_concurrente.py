"""
cocina_concurrente.py
----------------------
Módulo 2 -- "Cocina concurrente" (CocinaPerú Express)

Versión CONCURRENTE: crea un hilo (cocinero) por cada tarea y los
ejecuta al mismo tiempo con threading.Thread.

Reutiliza TAREAS y tarea() desde cocina_secuencial.py, para que ambos
módulos preparen exactamente las mismas tareas y sea justo comparar sus
tiempos.

Este archivo se puede:
  1) Ejecutar solo, para ver únicamente la preparación concurrente:
         python3 cocina_concurrente.py
  2) Importar desde comparar_tiempos.py.
"""

import threading
import time

from cocina_secuencial import TAREAS, tarea


def ejecutar_concurrente(tareas):
    """Ejecuta todas las tareas en paralelo, un hilo (cocinero) por
    tarea. Devuelve el tiempo total que tomó."""
    print("\n--- Preparación CONCURRENTE (un cocinero por tarea) ---")
    inicio = time.time()

    hilos = [threading.Thread(target=tarea, args=(nombre, duracion))
             for nombre, duracion in tareas]

    for h in hilos:
        h.start()   # cada hilo empieza a trabajar de inmediato

    for h in hilos:
        h.join()    # el hilo principal espera a que TODOS terminen

    total = time.time() - inicio
    print(f"Tiempo total concurrente: {total:.2f} s")
    return total


def main():
    print("=== CocinaPerú Express -- Preparación concurrente ===")
    print(f"Tareas del pedido: {[t[0] for t in TAREAS]}")
    ejecutar_concurrente(TAREAS)


if __name__ == "__main__":
    main()
