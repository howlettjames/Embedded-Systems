#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NO_HILOS 4

void * funHilo(void *arg);

int main()
{
    register int nh;
    int nhs[NO_HILOS], *res_nh;
    pthread_t tids[NO_HILOS];

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
    
    return 0;
}

void *funHilo(void *arg)
{
    int nh = *(int *) arg;

    printf("Hilo %d ejecutado\n", nh);
    
    pthread_exit(arg);
}