/*
 * mostrador.c
 * -----------
 * Módulo 1 — "Recepción de pedidos" (CocinaPerú Express)
 *
 * El proceso principal (el "mostrador") recibe N_PEDIDOS pedidos y, por cada
 * uno, crea un proceso hijo independiente con fork(). Ese hijo reporta su
 * propio PID y luego se transforma, mediante exec(), en el programa
 * preparar_pedido, que simula la preparación real del pedido.
 *
 * El proceso principal espera (wait) a que TODOS los hijos terminen antes
 * de anunciar que el restaurante cierra.
 *
 * Compilación y ejecución (desde modulo1_procesos_c/):
 *   make
 *   cd bin && ./mostrador
 * Se ejecuta desde bin/ porque execl() usa la ruta relativa
 * "./preparar_pedido", que se resuelve contra el directorio actual.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_PEDIDOS 5   /* Cantidad de pedidos simultáneos a simular (sugerido: 3 a 5) */

int main(void) {
    printf("=== CocinaPeru Express -- Mostrador abierto ===\n");
    printf("Llegan %d pedidos. Creando un proceso por cada uno...\n\n", N_PEDIDOS);
    fflush(stdout);
    /* IMPORTANTE: se vacía el búfer de salida ANTES del primer fork().
     * Si no se hace esto, cuando la salida no va a una terminal interactiva
     * (por ejemplo, al redirigirla a un archivo o a otro programa), stdio
     * usa buffering completo en vez de buffering por línea. Cada fork()
     * duplica la memoria del proceso, incluyendo el búfer de stdout aún no
     * vaciado, así que cada hijo terminaría imprimiendo también ese texto
     * pendiente cuando el propio hijo haga flush (por ejemplo, al llamar a
     * exec o al terminar). Resultado: el encabezado aparecería repetido
     * una vez por cada hijo creado. */

    for (int i = 1; i <= N_PEDIDOS; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            /* Se deja de crear pedidos, pero sin salir: los hijos ya
             * creados deben esperarse igual para no dejarlos huérfanos. */
            perror("fork");
            break;
        }

        if (pid == 0) {
            /* ---- Código que ejecuta SOLO el proceso hijo ---- */
            char numero_str[12];
            snprintf(numero_str, sizeof(numero_str), "%d", i);

            printf("Pedido #%d creado (PID %d)\n", i, getpid());
            fflush(stdout);

            /* El hijo se reemplaza a sí mismo por el programa preparar_pedido */
            execl("./preparar_pedido", "preparar_pedido", numero_str, NULL);

            /* Si execl() retorna, es porque falló */
            perror("execl");
            exit(1);
        }
        /* ---- El proceso padre continúa el ciclo para crear el siguiente pedido ---- */
    }

    /* wait() devuelve -1 con errno == ECHILD cuando ya no quedan hijos:
     * así se espera a todos los que realmente se crearon, aunque un
     * fork() haya fallado a mitad del ciclo. */
    int estado;
    pid_t hijo_terminado;
    while ((hijo_terminado = wait(&estado)) > 0) {
        if (WIFEXITED(estado))
            printf("Mostrador: el proceso hijo PID %d terminó (código %d).\n",
                   hijo_terminado, WEXITSTATUS(estado));
        else
            printf("Mostrador: el proceso hijo PID %d terminó de forma anormal.\n",
                   hijo_terminado);
    }
    if (errno != ECHILD)
        perror("wait");

    printf("\nRestaurante cerrado: todos los pedidos fueron atendidos\n");
    return 0;
}
