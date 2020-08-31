/** @brief procesamiento.c, este programa incluye las funciones
*   que ejecutara cada proceso 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "defs.h"
#include "arreglos.h"

int *fhistograma(int *datos)
{
    int *histograma = reservar_memoria(256);
    register int i;

    for(i = 0; i < 256; i++)
        histograma[i] = 0;

    for(i = 0; i < N; i++)
        histograma[datos[i]]++;

    return histograma;
}

int *ordenamiento_burbuja(int *datos)
{
    register int i, j;

    for(i = 2; i <= N; i++)
    {
        for(j = 0; j <= N - i; j++)
        {
            if(datos[j] > datos[j + 1])
            {
                int aux = datos[j];
                datos[j] = datos[j + 1];
                datos[j + 1] = aux;
            }
        }
    }
	
    return datos;
}

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
