/** @brief procesos.c, este programa incluye las
*   funciones que ejecutaran cada proceso
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "defs.h"
#include "procesamiento.h"

void proceso_hijo(int np, int *datos)
{
    int mayor, menor, promedio, moda;

    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());

    if(np == 0)
    {
        mayor = buscar_mayor(datos);
        exit(mayor);
    }
    else if(np == 1)
    {
        menor = buscar_menor(datos);
        exit(menor);
    }
    else if(np == 2)
    {
        promedio = fpromedio(datos);
        exit(promedio);
    }
    else if(np == 3)
    {
        moda = fmoda(datos);
        exit(moda);
    }
}

void proceso_padre()
{
    pid_t pid;
    register int np;
    int estado;

    printf("Proceso padre con pid: %d\n", getpid());
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&estado);
        printf("Proceso hijo con pid: %d y retorno: %d terminado\n", pid, estado >> 8);
    }
}