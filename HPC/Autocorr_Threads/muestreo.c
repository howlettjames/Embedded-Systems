/** @brief muestreo.c, este programa genera una señal senoidal
*   y la guarda en un archivo
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"

double seno[MUESTRAS], autocorr[MUESTRAS];

void *thread(void *arg);

int main(void)
{
	register int nh;
	int nhs[NO_HILOS], *res_nh;
    pthread_t tids[NO_HILOS];

	genera_seno(seno);
	guarda_datos(seno, "seno.dat");

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

    guarda_datos(autocorr, "autocorr.dat");

	return 0;
}

void *thread(void *arg)
{
	int nh = *(int *) arg; // nh = l
    register int n = 0;

    printf("Hilo %d ejecutado\n", nh);

    autocorr[nh] = 0;
    for(n = nh; n < MUESTRAS; n++)
        autocorr[nh] += seno[n] * seno[n - nh];

   pthread_exit(arg);
}