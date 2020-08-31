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
#include "arreglos.h"

void proceso_hijo(int np, int *datos, int pipefd[])
{
    int mayor, menor;
    float promedio, moda;
    int *datos_resultado;

    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());

    close(pipefd[0]);
    if(np == 0)
    {
        mayor = buscar_mayor(datos);
        write(pipefd[1], &mayor, sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 1)
    {
        menor = buscar_menor(datos);
        write(pipefd[1], &menor, sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 2)
    {
        promedio = fpromedio(datos);
        write(pipefd[1], &promedio, sizeof(float));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 3)
    {
        moda = fmoda(datos);
        write(pipefd[1], &moda, sizeof(float));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 4)
    {
        datos_resultado = ordenamiento_burbuja(datos);
        write(pipefd[1], datos_resultado, N * sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
    else if(np == 5)
    {
        datos_resultado = fhistograma(datos);
        write(pipefd[1], datos_resultado, 256 * sizeof(int));
        close(pipefd[1]);
        exit(np);
    }
}

void proceso_padre(int pipefd[])
{
    pid_t pid;
    register int np, i;
    int estado, resultado_int;
    float resultado_float;
    int *resultado_ordenamiento = reservar_memoria(N);
    int *resultado_histograma = reservar_memoria(256);

    printf("Proceso padre con pid: %d\n", getpid());
    close(pipefd[1]);
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&estado);
        estado = estado >> 8;

        if(estado == 0 || estado == 1)
        {
            read(pipefd[0], &resultado_int, sizeof(int));
            printf("Proceso hijo numero: %d con pid: %d y retorno: %d terminado\n", estado, pid, resultado_int);
        }
        else if(estado == 2 || estado == 3)
        {   
            read(pipefd[0], &resultado_float, sizeof(float));
            printf("Proceso hijo numero: %d con pid: %d y retorno: %f terminado\n", estado, pid, resultado_float);
        }
        else if(estado == 4)
        {
            read(pipefd[0], resultado_ordenamiento, N * sizeof(int));
            printf("Proceso hijo numero: %d con pid: %d terminado\n", estado, pid);
            for(i = 0; i < N; i++)
            {
                if(!(i % 16))
                    printf("\n");
                printf("%3d ", resultado_ordenamiento[i]);
            }
            printf("\n");
        }
        else if(estado == 5)
        {
            read(pipefd[0], resultado_histograma, 256 * sizeof(int));
            printf("Proceso hijo numero: %d con pid: %d terminado\n", estado, pid);
            for(i = 0; i < 256; i++)
            {
                if(!(i % 16))
                    printf("\n");
                printf("%d-%d ", i, resultado_histograma[i]);
            }
            printf("\n");
        }
        
    }
    close(pipefd[0]);
}