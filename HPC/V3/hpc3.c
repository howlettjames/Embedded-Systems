//Buscar mayor, menor, promedio, el mas repetido

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 4
#define N 64

int *A, *B, *P;

// Procesos
void proceso_hijo(int np, int pipefd[]);
void proceso_padre(int pipefd[][2]);
// Arreglos
int *reservar_memoria();
void llenar_arreglo(int *datos);
void imprimir(int *datos);

int main(int argc, char const *argv[])
{
    pid_t pid;
    register int np;
    int pipefd[NUM_PROC][2], edo_pipe;

    A = reservar_memoria();
    llenar_arreglo(A);
    imprimir(A);

    B = reservar_memoria();
    llenar_arreglo(B);
    imprimir(B);

    P = reservar_memoria();

    printf("Probando procesos...\n");
    for(np = 0; np < NUM_PROC; np++)
    {
        edo_pipe = pipe(pipefd[np]);
        if(edo_pipe == -1)
        {
            perror("Error al crear la tuberia");
            exit(EXIT_FAILURE);
        }


        pid = fork();
        if(pid == -1)
        {
            perror("Error al crear el proceso");
            exit(EXIT_FAILURE);
        }
        if(!pid)
        {
            proceso_hijo(np, pipefd[np]);
        }
    }    

    proceso_padre(pipefd);
    imprimir(P);
    free(A);
    free(B);
    free(P);

    return 0;
}

void proceso_hijo(int np, int pipefd[])
{
    register int i;
    int iniBloque, finBloque, tamBloque;

    tamBloque = N / NUM_PROC;
    iniBloque = np * tamBloque;
    finBloque = iniBloque + tamBloque;
    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());
    close(pipefd[0]);
    
    for(i = iniBloque; i < finBloque; i++)
        P[i] = A[i] * B[i];   

    write(pipefd[1], &P[iniBloque], tamBloque * sizeof(int));
    close(pipefd[1]);
    exit(np);
}

void proceso_padre(int pipefd[][2])
{
    pid_t pid;
    register int np;
    int edo_np, iniBloque, tamBloque;

    printf("Proceso padre con pid: %d\n", getpid());
    tamBloque = N / NUM_PROC;
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&edo_np);
        edo_np = edo_np >> 8;
        close(pipefd[edo_np][1]);
        iniBloque = edo_np * tamBloque;

        read(pipefd[edo_np][0], &P[iniBloque], tamBloque * sizeof(int));
        
        printf("Proceso %d hijo con pid: %d terminado\n", edo_np, pid);
        close(pipefd[edo_np][0]);
    }
}

void imprimir(int *datos)
{
    register int i;

    for(i = 0; i < N; i++)
    {
        if( !(i % 14) )
            printf("\n");
        printf("%5d ", datos[i]);
    }
    printf("\n");
}

void llenar_arreglo(int *datos)
{
    register int i;

    srand((unsigned) time(NULL));
    for(i = 0; i < N; i++)
        datos[i] = rand() % 256;
}

int *reservar_memoria()
{
    int *mem;
    
    mem = (int *) malloc(sizeof(int) * N);

    if(mem == NULL)
    {
        perror("Error de asignacion de memoria");
        exit(EXIT_FAILURE);
    }

    return mem;
}