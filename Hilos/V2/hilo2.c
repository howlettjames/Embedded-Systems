#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 32

void imprimir(int *datos);
void llenar_arreglo(int *datos);
int *reservar_memoria(int elementos);

void *buscar_mayor(void *arg);
void *buscar_menor(void *arg);

int main()
{
    int *res_mayor, *res_menor, *datos;
    pthread_t tid_mayor, tid_menor;

    srand((unsigned) time(NULL));
    
    datos = reservar_memoria(N);
    llenar_arreglo(datos);
    imprimir(datos);

    pthread_create(&tid_mayor, NULL, buscar_mayor, (void *) datos);
    pthread_create(&tid_menor, NULL, buscar_menor, (void *) datos);

    pthread_join(tid_mayor, (void **) &res_mayor);
    pthread_join(tid_menor, (void **) &res_menor);
    
    printf("El mayor es %d\n", *res_mayor);
    printf("El menor es %d\n", *res_menor);
    
    free(datos);
    return 0;
}

void *buscar_mayor(void *arg)
{
    register int i;
    static int mayor;
    int *datos = (int *) arg;

    mayor = datos[0];
    for(i = 1; i < N; i++)
        if(datos[i] > mayor)
            mayor = datos[i];

    pthread_exit((void *) &mayor);
}

void *buscar_menor(void *arg)
{
    register int i;
    static int menor;
    int *datos = (int *) arg;

    menor = datos[0];
    for(i = 1; i < N; i++)
        if(datos[i] < menor)
            menor = datos[i];

    pthread_exit((void *) &menor);
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

int *reservar_memoria(int elementos)
{
    int *mem;
    
    mem = (int *) malloc(sizeof(int) * elementos);

    if(mem == NULL)
    {
        perror("Error de asignacion de memoria");
        exit(EXIT_FAILURE);
    }

    return mem;
}