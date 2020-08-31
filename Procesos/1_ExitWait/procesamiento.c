/** @brief procesamiento.c, este programa incluye las funciones
*   que ejecutara cada proceso 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "defs.h"

int buscar_mayor(int *datos)
{
    register int i;
    int mayor;

    mayor = datos[0];
    for(i = 1; i < N; i++)
        if(datos[i] > mayor)
            mayor = datos[i];

    return mayor;
}

int buscar_menor(int *datos)
{
    register int i;
    int menor;

    menor = datos[0];
    for(i = 1; i < N; i++)
        if(datos[i] < menor)
            menor = datos[i];

    return menor;
}

int fpromedio(int *datos)
{
    register int i;
	int promedio = 0;

	for(i = 0; i < N; i++)
		promedio += datos[i];

	promedio /= N;

	return promedio;
}

int fmoda(int *datos)
{
    register int i, j;
	int max = datos[0], times, max_times = 1;

	for(i = 0; i < N - 1 ;i++)
	{
		times = 1;
		for(j = i + 1; j < N; j++)
		{
			if(datos[i] == datos[j])
				times++;
		}
		if(times > max_times)
		{
		 	max_times = times;
		 	max = datos[i];
		}
	}

	return max;
}
