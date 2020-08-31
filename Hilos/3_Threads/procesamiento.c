/** @brief procesamiento.c, este programa incluye las funciones
*   que ejecutara cada proceso 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include "defs.h"
#include "arreglos.h"

void *ffrequencies(void *arg)
{
    int *frequencies = reservar_memoria(256);
    int *data = (int *) arg;
    register int i;

    for(i = 0; i < 256; i++)
        frequencies[i] = 0;

    for(i = 0; i < N; i++)
        frequencies[data[i]]++;

    pthread_exit((void *) frequencies);
}

void *bubble(void *arg)
{
    register int i, j;
    int aux;
    int *data = (int *) arg;
    pthread_mutex_t block;

    pthread_mutex_init(&block, NULL);

    pthread_mutex_lock(&block);
    for(i = 2; i <= N; i++)
    {
        for(j = 0; j <= N - i; j++)
        {
            if(data[j] > data[j + 1])
            {
                aux = data[j];
                data[j] = data[j + 1];
                data[j + 1] = aux;
            }
        }
    }
    pthread_mutex_unlock(&block);
    
    pthread_mutex_destroy(&block);
    pthread_exit((void *) data);
}

void *search_greater(void *arg)
{
    register int i;
    int *greater = malloc(sizeof(int));
    int *data = (int *) arg;

    *greater = data[0];
    for(i = 1; i < N; i++)
        if(data[i] > *greater)
            *greater = data[i];

    pthread_exit((void *) greater);
}

void *search_lesser(void *arg)
{
    register int i;
    int *lesser = malloc(sizeof(int));
    int *data = (int *) arg;

    *lesser = data[0];
    for(i = 1; i < N; i++)
        if(data[i] < *lesser)
            *lesser = data[i];

    pthread_exit((void *) lesser);
}

void *faverage(void *arg)
{
    register int i;
	int *average = malloc(sizeof(int));
    int *data = (int *) arg;

    *average = 0;
	for(i = 0; i < N; i++)
		*average += data[i];

	*average /= N;

	pthread_exit((void *) average);
}

void *ftrend(void *arg)
{
    register int i, j;
    int *data = (int *) arg;
    int *max = malloc(sizeof(int));
	int times, max_times = 1;

    *max = data[0];
	for(i = 0; i < N - 1 ;i++)
	{
		times = 1;
		for(j = i + 1; j < N; j++)
		{
			if(data[i] == data[j])
				times++;
		}
		if(times > max_times)
		{
		 	max_times = times;
		 	*max = data[i];
		}
	}

	pthread_exit((void *) max);
}
