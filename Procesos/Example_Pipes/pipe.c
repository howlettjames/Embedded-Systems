//Buscar mayor, menor, promedio, el mas repetido

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 4

void proceso_hijo(int np, int pipefd[]);
void proceso_padre(int pipefd[]);

int main(int argc, char const *argv[])
{
    pid_t pid;
    register int np;
    int pipefd[2], edo_pipe;

    printf("Probando procesos...\n");
    edo_pipe = pipe(pipefd);

    if(edo_pipe == -1)
    {
        perror("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }
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
            proceso_hijo(np, pipefd);
        }
    }    

    proceso_padre(pipefd);

    return 0;
}

void proceso_hijo(int np, int pipefd[])
{
    int num1 = 20, num2 = 10;
    int suma, resta, mult, div;
    
    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());
    
    close(pipefd[0]);
    if(np == 0)
    {
        suma = num1 + num2;
        write(pipefd[1], &suma, sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 1)
    {
        resta = num1 - num2;
        write(pipefd[1], &resta, sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 2)
    {
        mult = num1 * num2;
        write(pipefd[1], &mult, sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 3)
    {
        div = num1 / num2;
        write(pipefd[1], &div, sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
}

void proceso_padre(int pipefd[])
{
    pid_t pid;
    register int np;
    int edo_np, resultado;

    printf("Proceso padre con pid: %d\n", getpid());
    close(pipefd[1]);
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&edo_np);
        read(pipefd[0], &resultado, sizeof(int));
        edo_np = edo_np >> 8;

        if(edo_np == 0)
            printf("Proceso %d, con pid: %d suma: %d terminado\n", edo_np, pid, resultado);
        else if(edo_np == 1)
            printf("Proceso %d, con pid: %d resta: %d terminado\n", edo_np, pid, resultado);
        else if(edo_np == 2)
            printf("Proceso %d, con pid: %d multiplicacion: %d terminado\n", edo_np, pid, resultado);
        else if(edo_np == 3)
            printf("Proceso %d, con pid: %d division: %d terminado\n", edo_np, pid, resultado);
    }
    close(pipefd[0]);
}