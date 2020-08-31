//Buscar mayor, menor, promedio, el mas repetido

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "defs.h"
#include "procesos.h"
#include "arreglos.h"

int main(int argc, char const *argv[])
{
    int *datos;
    pid_t pid;
    register int np;

    printf("Probando procesos...\n");
    srand(getpid());
    datos = reservar_memoria();
    llenar_arreglo(datos);
    imprimir(datos);

    for(np = 0; np < NUM_PROC; np++)
    {
        pid = fork();
        if(pid == -1)
        {
            perror("Error al crear el proceso");
            exit(EXIT_FAILURE);
        }
        if(!pid)
        {
            proceso_hijo(np, datos);
        }
    }    

    proceso_padre();
    free(datos);

    return 0;
}