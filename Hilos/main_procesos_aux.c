//Buscar mayor, menor, promedio, el mas repetido

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include "defs.h"
#include "procesamiento.h"
#include "arreglos.h"

int main(int argc, char const *argv[])
{
    int *data, *result;
    register int no_thread;
    pthread_t thread_ids[NUM_THREADS];
 
    printf("Testing threads...\n");

    srand(getpid());
    data = reservar_memoria(N);
    llenar_arreglo(data);
    imprimir(data);

    for(no_thread = 0; no_thread < NUM_THREADS; no_thread++)
    {
        if(no_thread == 0)
            pthread_create(&thread_ids[no_thread], NULL, search_greater, (void *) data);
        else if(no_thread == 1)
            pthread_create(&thread_ids[no_thread], NULL, search_lesser, (void *) data);
        else if(no_thread == 2)
            pthread_create(&thread_ids[no_thread], NULL, faverage, (void *) data);
        else if(no_thread == 3)
            pthread_create(&thread_ids[no_thread], NULL, ftrend, (void *) data);
        else if(no_thread == 4)
            pthread_create(&thread_ids[no_thread], NULL, bubble, (void *) data);
        else if(no_thread == 5)
            pthread_create(&thread_ids[no_thread], NULL, ffrequencies, (void *) data);
    }

    printf("\n");
    for(no_thread = 0; no_thread < NUM_THREADS; no_thread++)
    {
        pthread_join(thread_ids[no_thread], (void **) &result);
        if(no_thread == 4)
        {
            printf("Thread %ld finished\n", thread_ids[no_thread]);
            for(int i = 0; i < N; i++)
            {
                if(!(i % 16))
                    printf("\n");
                printf("%3d ", result[i]);
            }
            printf("\n");
        }
        else if(no_thread == 5)
        {
            printf("Thread %ld finished\n", thread_ids[no_thread]);
            for(int i = 0; i < 256; i++)
            {
                if(!(i % 14))
                    printf("\n");
                printf("%3d:%2d ", i, result[i]);
            }
            printf("\n");
        }
        else
            printf("Thread %ld finished return value: %d\n", thread_ids[no_thread], *result);
    }    

    free(data);

    return 0;
}