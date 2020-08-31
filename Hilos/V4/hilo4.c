#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NO_HILOS 4

void * funHilo(void *arg);

int counter;
pthread_mutex_t block;

int main()
{
    register int nh;
    int nhs[NO_HILOS], *res_nh;
    pthread_t tids[NO_HILOS];
    
    counter = 0;
    pthread_mutex_init(&block, NULL);

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
    
    pthread_mutex_destroy(&block);
    return 0;
}

void *funHilo(void *arg)
{
    int nh = *(int *) arg;
    register int i = 0;

    pthread_mutex_lock(&block);
    counter++;
    printf("Hilo %d ejecutado con contador %d\n", nh, counter);
    while(--i);
    printf("Terminado hilo %d ejecutado con contador %d\n", nh, counter);
    pthread_mutex_unlock(&block);
    
    pthread_exit(arg);
}