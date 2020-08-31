#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h>
#include <signal.h>

#define NUM_PROC 8

void proceso_hijo(int np);
void proceso_padre(pid_t pids[]);
void isr(int sig);

int main(int argc, char const *argv[])
{
    pid_t pids[NUM_PROC];
    register int np;

    printf("Probando procesos...\n");
    if(signal(SIGUSR1, isr) == SIG_ERR)
    {
        perror("Error en la isr");
        exit(EXIT_FAILURE);
    }

    for(np = 0; np < NUM_PROC; np++)
    {
        pids[np] = fork();
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

    proceso_padre(pids);

    return 0;
}

void proceso_hijo(int np)
{
    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());
    pause();
    printf("Senal detectada en el hijo: %d \n", np);
    exit(np);
}

void proceso_padre(pid_t pids[])
{
    pid_t pid;
    register int np;
    int estado;

    printf("Proceso padre con pid: %d\n", getpid());
    for(np = 0; np < NUM_PROC; np++)
    {
        kill(pids[np], SIGUSR1);
    }
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&estado);
        printf("Proceso hijo: %d con pid: %d terminado\n", estado >> 8, pid);
    }
}

void isr(int sig)
{
    if(sig == SIGUSR1)
        printf("Senal USR1 detectada\n");
}