/** @brief muestreo.c, este programa genera una señal senoidal
*   y la guarda en un archivo
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"

double seno[MUESTRAS], hamming[MUESTRAS], resultado[MUESTRAS];

// Procesos
void proceso_hijo(int np, int pipefd[]);
void proceso_padre(int pipefd[][2]);

int main(void)
{
	pid_t pid;
    register int np;
    int pipefd[NUM_PROC][2], edo_pipe;

    genera_seno(seno);
	guarda_datos(seno, "seno.dat");
	genera_hamming(hamming);
	guarda_datos(hamming, "hamming.dat");

	printf("Probando procesos...\n");
    for(np = 0; np < NUM_PROC; np++)
    {
        edo_pipe = pipe(pipefd[np]);
        if(edo_pipe == -1)
        {
            perror("Error al crear la tuberia");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if(pid == -1)
        {
            perror("Error al crear el proceso");
            exit(EXIT_FAILURE);
        }
        if(!pid)
        {
            proceso_hijo(np, pipefd[np]);
        }
    }

    proceso_padre(pipefd);
    guarda_datos(resultado, "resultado.dat");

	return 0;
}

void proceso_hijo(int np, int pipefd[])
{
    register int i;
    int iniBloque, finBloque, tamBloque;

    tamBloque = MUESTRAS / NUM_PROC;
    iniBloque = np * tamBloque;
    finBloque = iniBloque + tamBloque;
    printf("Proceso hijo: %d, con pid: %d\n", np, getpid());
    close(pipefd[0]);
    
    for(i = iniBloque; i < finBloque; i++)
        resultado[i] = seno[i] * hamming[i];   

    write(pipefd[1], &resultado[iniBloque], tamBloque * sizeof(double));
    close(pipefd[1]);
    exit(np);
}

void proceso_padre(int pipefd[][2])
{
    pid_t pid;
    register int np;
    int edo_np, iniBloque, tamBloque;

    printf("Proceso padre con pid: %d\n", getpid());
    tamBloque = MUESTRAS / NUM_PROC;
    for(np = 0; np < NUM_PROC; np++)
    {
        pid = wait(&edo_np);
        edo_np = edo_np >> 8;
        close(pipefd[edo_np][1]);
        iniBloque = edo_np * tamBloque;

        read(pipefd[edo_np][0], &resultado[iniBloque], tamBloque * sizeof(double));
        
        printf("Proceso %d hijo con pid: %d terminado\n", edo_np, pid);
        close(pipefd[edo_np][0]);
    }
}