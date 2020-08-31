/** @brief archivos.c, este programa guarda un arreglo de datos
*   en un archivo
*/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void guarda_datos(double *datos, char filename[])
{
	FILE *ap_arch;
	register int n;

	ap_arch = fopen(filename, "w");

	if(!ap_arch)
	{
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0; n < MUESTRAS; n++)
	{
		fprintf(ap_arch, "%f\n", datos[n]);
	}

	fclose(ap_arch);
}