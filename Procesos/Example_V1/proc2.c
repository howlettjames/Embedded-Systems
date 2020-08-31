#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 32

int *reservar_memoria();
void llenar_arreglo(int *datos);
void imprimir(int *datos);
int buscar_mayor(int *datos);
int buscar_menor(int *datos);

int main(int argc, char const *argv[])
{
    int *datos, mayor, menor;

    datos = reservar_memoria();
    llenar_arreglo(datos);
    imprimir(datos);

    mayor = buscar_mayor(datos);
    menor = buscar_menor(datos);

    printf("El mayor es: %d y el menor es: %d\n", mayor, menor);
    free(datos);

    return 0;
}

int buscar_mayor(int *datos)
{
    register int i;
    int mayor;

    mayor = datos[0];
    for(i = 1; i < N; i++)
        if(datos[i] > mayor)
            mayor = datos[i];

    return mayor;
}

int buscar_menor(int *datos)
{
    register int i;
    int menor;

    menor = datos[0];
    for(i = 1; i < N; i++)
        if(datos[i] < menor)
            menor = datos[i];

    return menor;
}

void imprimir(int *datos)
{
    register int i;

    for(i = 0; i < N; i++)
    {
        if( !(i % 16) )
            printf("\n");
        printf("%3d ", datos[i]);
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