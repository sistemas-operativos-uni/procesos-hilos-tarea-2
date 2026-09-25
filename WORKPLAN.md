# WORKPLAN — CocinaPerú Express

## Fases y bloqueos

| Fase | Bloquea a | Responsable |
|---|---|---|
| 1. Diseño | Todo lo demás | Todos (liderado por Coordinador) |
| 2. Módulo 1 (C) | Fase 4 (Medición) | Dev C |
| 3. Módulo 2 (Python) | Fase 4 (Medición) | Dev Python |
| 4. Medición y comparación | Fase 5 (Informe) | Tester/Analista |
| 5. Informe + Sustentación | — | Coordinador (con apoyo de todos) |

Fases 2 y 3 corren **en paralelo**, sin dependencia entre sí. Fase 4 no puede iniciar hasta que ambas fases 2 y 3 estén cerradas.

---

## Tareas por integrante

### Coordinador(a)
- [ ] Convocar reunión de diseño (Fase 1), cerrar diagrama mostrador→pedidos→cocina.
- [ ] Crear repo, estructura de carpetas, README.md.
- [ ] Dar seguimiento diario/semanal al avance de Dev C y Dev Python.
- [ ] Integrar todas las secciones del informe final.
- [ ] Coordinar fecha y armado de la sustentación (5-8 min).
- **Bloqueado por:** nada al inicio. Su tarea de integración (informe) está bloqueada por Fases 2, 3 y 4.

### Dev C — Módulo 1
- [ ] Definir N_PEDIDOS (3-5) junto al equipo en Fase 1.
- [ ] Implementar `mostrador.c`: loop de `fork()`, cada hijo imprime PID + N° de pedido.
- [ ] Implementar `preparar_pedido.c` (o script) que ejecuta vía `execl()`.
- [ ] Implementar `wait()`/`waitpid()` en el padre; mensaje final de cierre.
- [ ] Escribir `Makefile` para compilar con un solo comando.
- [ ] Capturar pantalla de ejecución (para `docs/capturas/modulo1/`).
- **Bloqueado por:** Fase 1 (diseño). **Bloquea a:** Tester (Fase 4).

### Dev Python — Módulo 2
- [ ] Definir las 3 tareas de cocina (cortar, freír, emplatar) en Fase 1.
- [ ] Implementar `cocina_concurrente.py` con `threading.Thread`, `start()`, `join()`.
- [ ] Implementar `cocina_secuencial.py` (mismas tareas, sin hilos, para el baseline).
- [ ] Medir tiempos con `time.time()` en ambas versiones.
- [ ] Capturar pantalla de ejecución (para `docs/capturas/modulo2/`).
- **Bloqueado por:** Fase 1 (diseño). **Bloquea a:** Tester (Fase 4).

### Tester / Analista
- [ ] Escribir `comparar_tiempos.py` que corre secuencial y concurrente, calcula la mejora (secuencial ÷ concurrente).
- [ ] Verificar que Módulo 1 ejecuta sin errores (todos los `fork()`/`exec()`/`wait()` correctos).
- [ ] Armar la tabla comparativa de tiempos para el informe.
- [ ] Redactar las 3-4 líneas explicando por qué la mejora no es exactamente 3x (overhead de creación de hilos, GIL de Python, planificación del SO).
- **Bloqueado por:** cierre de Fase 2 y Fase 3 (necesita ambos módulos terminados). **Bloquea a:** Coordinador (informe, Fase 5).

---

## Checklist de entrega final
- [ ] Código fuente de ambos módulos en el repo.
- [ ] Diagrama de diseño (Paso 1) en `docs/diseno/`.
- [ ] Capturas numeradas en `docs/capturas/`.
- [ ] Tabla de tiempos + interpretación.
- [ ] Informe PDF en `informe/`.
- [ ] Ensayo de sustentación: TODOS explican fork/exec, hilos, PCB.