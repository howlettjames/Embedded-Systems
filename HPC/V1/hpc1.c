#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 64
#define NUM_PROC 2

int *A, *B, *P;
// Arreglos
int *reservar_memoria();
void llenar_arreglo(int *datos);
void imprimir(int *datos);
// Procesamiento
void producto();

int main(int argc, char const *argv[])
{
    int *datos, mayor, menor;

    A = reservar_memoria();
    llenar_arreglo(A);
    imprimir(A);

    B = reservar_memoria();
    llenar_arreglo(B);
    imprimir(B);

    P = reservar_memoria();

    producto();

    imprimir(P);

    free(A); 
    free(B); 
    free(P); 

    return 0;
}

void producto(void)
{
    register int i;

    for(i = 0; i < N; i++)
    {
        P[i] = A[i] * B[i];
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