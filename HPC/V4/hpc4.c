// Comparar version secuencial contra no secuencial
// Primero lanzar promedio y despues bubble secuencial
// Después hacerlo concurrente con hilos y va a salir diferente a veces
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NO_HILOS 4
#define N 32

int *A, promedio;
pthread_mutex_t bloqueo;

// Arreglos
int *reservar_memoria();
void llenar_arreglo(int *datos);
void imprimir(int *datos);
// Threads
void * funHilo(void *arg);

int main()
{
    register int nh;
    int nhs[NO_HILOS], *res_nh;
    pthread_t tids[NO_HILOS];
    
    A = reservar_memoria();
    llenar_arreglo(A);
    imprimir(A);

    promedio = 0;
    pthread_mutex_init(&bloqueo, NULL);
    for(nh = 0; nh < NO_HILOS; nh++)
    {
        nhs[nh] = nh;
        pthread_create(&tids[nh], NULL, funHilo, (void *) &nhs[nh]);
    }

    for(nh = 0; nh < NO_HILOS; nh++)
    {
        pthread_join(tids[nh], (void **) &res_nh);
        printf("Hilo %d terminado\n", *res_nh);
    }    
    //promedio /= N;
    promedio >>= 5;
    printf("El promedio es: %d\n", promedio);
    pthread_mutex_destroy(&bloqueo);
    free(A);
    
    return 0;
}

void *funHilo(void *arg)
{
    int nh = *(int *) arg;
    register int i = 0;
    int suma_parcial = 0;

    printf("Hilo %d ejecutado\n", nh);
    for(i = nh; i < N; i += NO_HILOS)
        suma_parcial += A[i];
    
    pthread_mutex_lock(&bloqueo);
    promedio += suma_parcial;
    pthread_mutex_unlock(&bloqueo);
    
    pthread_exit(arg);
}

/* V1
void *funHilo(void *arg)
{
    int nh = *(int *) arg;
    int tam_bloque = N / NO_HILOS;
    int inicio_bloque = nh * tam_bloque;
    int fin_bloque = inicio_bloque + tam_bloque;
    register int i = 0;

    printf("Hilo %d ejecutado\n", nh);

    for(i = inicio_bloque; i < fin_bloque; i++)
        P[i] = A[i] * B[i];
    
    pthread_exit(arg);
}
*/

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