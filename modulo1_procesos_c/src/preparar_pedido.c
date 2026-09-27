/*
 * preparar_pedido.c
 * ------------------
 * Programa que simula la preparación de UN pedido.
 * Este binario es el que cada proceso hijo ejecuta mediante execl()
 * después de haber sido creado con fork() en mostrador.c.
 *
 * Recibe el número de pedido como argumento de línea de comandos
 * (execl se lo pasa) y simula el tiempo de preparación con sleep().
 *
 * Compilación: make (genera bin/preparar_pedido junto a bin/mostrador).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int numero_pedido = 0;

    if (argc > 1) {
        numero_pedido = atoi(argv[1]);
    }

    printf("  [Pedido #%d] (PID %d) -> preparando...\n", numero_pedido, getpid());
    fflush(stdout);

    /* Simula el tiempo real que toma preparar el pedido en cocina */
    sleep(2);

    printf("  [Pedido #%d] (PID %d) -> LISTO\n", numero_pedido, getpid());
    fflush(stdout);

    return 0;
}
