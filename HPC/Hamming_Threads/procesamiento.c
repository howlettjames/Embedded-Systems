/** @brief procesamiento.c, este programa genera una señal
*   senoidal con una frecuencia fundamental f y de 
*   muestreo fs
*/

#include <stdio.h>
#include <math.h>
#include "defs.h"

void genera_seno(double *seno)
{
	double f = 1000, fs = 45000;
	register int n;

	for(n = 0; n < MUESTRAS; n++)
		seno[n] = sinf(2 * n * M_PI * f / fs);
}

void genera_hamming(double *hamming)
{
	register int n;

	for(n = 0; n < MUESTRAS; n++)
		hamming[n] = 0.54 - 0.46 * cosf(2 * n * M_PI / (MUESTRAS - 1));
}