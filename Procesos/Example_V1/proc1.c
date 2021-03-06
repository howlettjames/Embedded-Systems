#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM_PROC 8

void proceso_hijo(int np);
void proceso_padre();

int main(int argc, char const *argv[])
{
    pid_t pid;
    register int np;

    printf("Probando procesos...\n");

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
            proceso_hijo(np);
        }
    }    

    proceso_padre();

    return 0;
}

void proceso_hijo(int np)
{
    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());
    while(1);
    exit(np);
}

void proceso_padre()
{
    pid_t pid;
    register int np;
    int estado;

    printf("Proceso padre con pid: %d\n", getpid());
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&estado);
        printf("Proceso hijo: %d con pid: %d terminado\n", estado >> 8, pid);
    }
}