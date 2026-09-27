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
 * Compilación:
 *   gcc mostrador.c -o mostrador
 * Ejecución (desde la misma carpeta que preparar_pedido):
 *   ./mostrador
 */

#include <stdio.h>
#include <stdlib.h>
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
            /* fork() falló: no se pudo crear el proceso hijo */
            perror("fork");
            exit(1);
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

    /* El proceso padre espera a que TODOS los hijos terminen */
    for (int i = 0; i < N_PEDIDOS; i++) {
        int estado;
        pid_t hijo_terminado = wait(&estado);
        printf("Mostrador: el proceso hijo PID %d terminó.\n", hijo_terminado);
    }

    printf("\nRestaurante cerrado: todos los pedidos fueron atendidos\n");
    return 0;
}
