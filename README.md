# CocinaPerú Express — Simulación de Procesos e Hilos

Práctica Dirigida N.º 03 · Sistemas Operativos (SW407) · UNI-FIIS

## Descripción
Simulación de un sistema de pedidos de comida rápida:
- **Módulo 1 (C):** cada pedido = un proceso (`fork()` + `exec()`).
- **Módulo 2 (Python):** dentro de un pedido, tareas de cocina = hilos concurrentes (`threading`).

## Requisitos
- gcc (`sudo apt install build-essential`)
- Python 3.8+

## Cómo ejecutar

### Módulo 1
```bash
cd modulo1_procesos_c
make
./bin/mostrador
```

### Módulo 2
```bash
cd modulo2_hilos_python
python3 comparar_tiempos.py
```

## Estructura
Ver árbol de carpetas en el repositorio.

## Equipo
| Rol | Integrante |
|---|---|
| Coordinador(a) | — |
| Dev C | — |
| Dev Python | — |
| Tester/Analista | — |

Detalle de tareas y dependencias: ver `WORKPLAN.md`.