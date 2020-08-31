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
    int pipefd[2], edo_pipe;

    printf("Probando procesos...\n");

    //Open pipe
    edo_pipe = pipe(pipefd);
    if(edo_pipe == -1)
    {
        perror("Error: Cannot create pipe");
        exit(EXIT_FAILURE);
    }

    srand(getpid());
    datos = reservar_memoria(N);
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
            proceso_hijo(np, datos, pipefd);
        }
    }    

    proceso_padre(pipefd);
    free(datos);

    return 0;
}