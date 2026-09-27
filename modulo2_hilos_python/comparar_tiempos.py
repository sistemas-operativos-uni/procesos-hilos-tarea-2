"""
comparar_tiempos.py
--------------------
Módulo 2 -- "Cocina concurrente" (CocinaPerú Express)

Junta los otros dos módulos: ejecuta la preparación secuencial
(cocina_secuencial.py) y la concurrente (cocina_concurrente.py) sobre
el mismo conjunto de tareas, y calcula/imprime la mejora obtenida.

Ejecución:
    python3 comparar_tiempos.py

Requiere que cocina_secuencial.py y cocina_concurrente.py estén en la
misma carpeta (los importa directamente).
"""

from cocina_secuencial import TAREAS, ejecutar_secuencial
from cocina_concurrente import ejecutar_concurrente


def main():
    print("=== CocinaPerú Express -- Módulo 2: Cocina concurrente ===")
    print(f"Tareas del pedido: {[t[0] for t in TAREAS]}")

    t_secuencial = ejecutar_secuencial(TAREAS)
    t_concurrente = ejecutar_concurrente(TAREAS)

    mejora = t_secuencial / t_concurrente

    print("\n=== Comparación de resultados ===")
    print(f"{'Modo':<15}{'Tiempo (s)':>12}")
    print(f"{'Secuencial':<15}{t_secuencial:>12.2f}")
    print(f"{'Concurrente':<15}{t_concurrente:>12.2f}")
    print(f"\nMejora (secuencial / concurrente): {mejora:.2f}x")


if __name__ == "__main__":
    main()
