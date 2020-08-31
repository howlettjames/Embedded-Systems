/** @brief muestreo.c, este programa genera una señal senoidal
*   y la guarda en un archivo
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"

double seno[MUESTRAS], hamming[MUESTRAS], resultado[MUESTRAS];

void *thread(void *arg);

int main(void)
{
	register int nh;
	int nhs[NO_HILOS], *res_nh;
    pthread_t tids[NO_HILOS];

	genera_seno(seno);
	guarda_datos(seno, "seno.dat");

	genera_hamming(hamming);
	guarda_datos(hamming, "hamming.dat");

	for(nh = 0; nh < NO_HILOS; nh++)
    {
        nhs[nh] = nh;
        pthread_create(&tids[nh], NULL, thread, (void *) &nhs[nh]);
    }

    for(nh = 0; nh < NO_HILOS; nh++)
    {
        pthread_join(tids[nh], (void **) &res_nh);
        printf("Hilo %d terminado\n", *res_nh);
    }

    guarda_datos(resultado, "resultado.dat");

	//free(seno);
    //free(hamming);
    //free(resultado);

	return 0;
}

void *thread(void *arg)
{
	int nh = *(int *) arg;
    register int i = 0;

    printf("Hilo %d ejecutado\n", nh);

    for(i = nh; i < MUESTRAS; i += NO_HILOS)
        resultado[i] = seno[i] * hamming[i];
    
    pthread_exit(arg);
}