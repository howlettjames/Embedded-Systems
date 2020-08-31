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

void fautocorr(double autocorr[]);

int main(void)
{
	genera_seno(seno);
	guarda_datos(seno, "seno.dat");

    fautocorr(autocorr);

    guarda_datos(autocorr, "autocorr.dat");

	return 0;
}

void fautocorr(double autocorr[])
{
    int l, n;

	for(l = 0; l < MUESTRAS; l++)
    {
        autocorr[l] = 0;
        for(n = l; n < MUESTRAS; n++)
            autocorr[l] += seno[n] * seno[n - l];
    }
}