/** @brief: Este programa muestra el uso del UART
*/
/** @brief muestreo.c, este programa captura una señal de pulso cardíaco por UART
*   y la procesa usando, ventana de Hamming, Autocorrelación y ...
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"

#define EVER 1

void *hilo_hamming(void *arg);
void *hilo_autocorr(void *arg);
int config_serial(char *, speed_t);

float pulso[MUESTRAS];
float hamming[MUESTRAS], resultado_hamming[MUESTRAS], resultado_autocorr[MUESTRAS];

int main()
{
	int fd_serie, i = 0, index_max = 0;
    float max = 0, frecuencia_pulso = 0, pulsaciones_min = 0;
	unsigned char dato;
	register int nh;
	int nhs[MUESTRAS], *res_nh;
    pthread_t tids[MUESTRAS];

	fd_serie = config_serial("/dev/ttyUSB0", B19200);
	printf("serial abierto con descriptor: %d\n", fd_serie);

	//Leemos datos del UART
	dato = 0;
	for(int i = 0; i < MUESTRAS; i++)
	{
		read(fd_serie, &dato, 1);
		printf("%d\n", dato);
		pulso[i] = ((float) dato) - 135.0;
	}
	genera_hamming(hamming);
	guarda_datos(hamming, "hamming.dat");
	
	guarda_datos(pulso, "pulso_raw.dat");

	// ----------------------------------------------------------------------------
	printf("Aplicando Hamming\n");
	for(nh = 0; nh < NO_HILOS; nh++)
    {
        nhs[nh] = nh;
        pthread_create(&tids[nh], NULL, hilo_hamming, (void *) &nhs[nh]);
    }

    for(nh = 0; nh < NO_HILOS; nh++)
    {
        pthread_join(tids[nh], (void **) &res_nh);
        printf("Hilo hamming %d terminado\n", *res_nh);
    }
    printf("Hamming terminado\n");
    guarda_datos(resultado_hamming, "pulso_hamming.dat");
    // ----------------------------------------------------------------------------
    printf("Aplicando Autocorrelación\n");
    for(nh = 0; nh < NO_HILOS; nh++)
    {
        nhs[nh] = nh;
        pthread_create(&tids[nh], NULL, hilo_autocorr, (void *) &nhs[nh]);
    }

    for(nh = 0; nh < NO_HILOS; nh++)
    {
        pthread_join(tids[nh], (void **) &res_nh);
        printf("Hilo autocorr %d terminado\n", *res_nh);
    }
    printf("Atocorrelación terminada\n");
    guarda_datos(resultado_autocorr, "pulso_autocorr.dat");
	// ----------------------------------------------------------------------------
    printf("Aplicando Algoritmo para encontrar frecuencia\n");    
    for(i = 300; i < MUESTRAS; i++)
    {
        if(resultado_autocorr[i - 1] < resultado_autocorr[i])
            if(resultado_autocorr[i + 1] < resultado_autocorr[i])
                if(max < resultado_autocorr[i])
                {
                    max = resultado_autocorr[i];
                    index_max = i;
                }
    }

    printf("Segundo mas alto: %f\n", max);
    printf("Segundo mas alto posicion: %d\n", index_max);
    frecuencia_pulso = 512 / (float)index_max;
    printf("Frecuencia senal: %f\n", frecuencia_pulso);
    pulsaciones_min = frecuencia_pulso * 60;
    printf("Pulsaciones min: %f\n", pulsaciones_min);
	
	close(fd_serie);

	return 0;
}

void *hilo_hamming(void *arg)
{
	int nh = *(int *) arg;
    register int i = 0;

    printf("Hilo %d ejecutado\n", nh);

    for(i = nh; i < MUESTRAS; i += NO_HILOS)
        resultado_hamming[i] = pulso[i] * hamming[i];
    
    pthread_exit(arg);
}

void *hilo_autocorr(void *arg)
{
	int nh = *(int *) arg; // nh = l
	int limite_hilo = 0, inicio_hilo, i;
    register int n = 0;

    printf("Hilo %d ejecutado\n", nh);

    if(nh == 0)
    {
    	inicio_hilo = 0;
    	limite_hilo = 1024;
    }
    else if(nh == 1)
    {
    	inicio_hilo = 1024;
    	limite_hilo = 2048;	
    }
    else if(nh == 2)
    {
    	inicio_hilo = 2048;
    	limite_hilo = 3072;	
    }
    else if(nh == 3)
    {
    	inicio_hilo = 3072;
    	limite_hilo = 4096;	
    }
    for(i = inicio_hilo; i < limite_hilo; i++)
    {
    	resultado_autocorr[i] = 0;
	    for(n = i; n < MUESTRAS; n++)
	        resultado_autocorr[i] += resultado_hamming[n] * resultado_hamming[n - i];  	
    }
    
   pthread_exit(arg);
}

/** @brief: Esta funcion Configura la interfaz serie
 *  @param: dispositivo_serial, Nombre el dispositivo serial a usar: ttyUSB0, ttyUSB1, etc
 *  @param: baudios, Velocidad de comunicacion. Se usa la constante Bxxxx, donde xxxx  es la
 *          velocidad. Estan definidas en termios.h. Ejemplo: B9600, B19200..
 *  @return: fd, Descriptor del serial
 *******************************************************************************************
 */
int config_serial( char *dispositivo_serial, speed_t baudios )
{
	struct termios newtermios;
  	int fd;
/*
 * Se abre un descriptor de archivo para manejar la interfaz serie
 * O_RDWR - Se abre el descriptor para lectura y escritura
 * O_NOCTTY - El dispositivo terminal no se convertira en el terminal del proceso
 * ~O_NONBLOCK - Se hace bloqueante la lectura de datos
 */
  	fd = open( dispositivo_serial, (O_RDWR | O_NOCTTY) & ~O_NONBLOCK );
	if( fd == -1 )
	{
		printf("Error al abrir el dispositivo tty \n");
		exit( EXIT_FAILURE );
  	}
/*
 * cflag - Proporciona los indicadores de modo de control
 *	CBAUD	- Velocidad de transmision en baudios.
 * 	CS8	- Especifica los bits por dato, en este caso 8
 * 	CLOCAL 	- Ignora las lineas de control del modem: CTS y RTS
 * 	CREAD  	- Habilita el receptor del UART
 * iflag - proporciona los indicadores de modo de entrada
 * 	IGNPAR 	- Ingnora errores de paridad, es decir, comunicación sin paridad
 * oflag - Proporciona los indicadores de modo de salida
 * lflag - Proporciona indicadores de modo local
 * 	TCIOFLUSH - Elimina datos recibidos pero no leidos, como los escritos pero no transmitidos
 * 	~ICANON - Establece modo no canonico, en este modo la entrada esta disponible inmediatamente
 * cc[]	 - Arreglo que define caracteres especiales de control
 *	VMIN > 0, VTIME = 0 - Bloquea la lectura hasta que el numero de bytes (1) esta disponible
 */
	newtermios.c_cflag 	= CBAUD | CS8 | CLOCAL | CREAD;
  	newtermios.c_iflag 	= IGNPAR;
  	newtermios.c_oflag	= 0;
  	newtermios.c_lflag 	= TCIOFLUSH | ~ICANON;
  	newtermios.c_cc[VMIN]	= 1;
  	newtermios.c_cc[VTIME]	= 0;

// Configura la velocidad de salida del UART
  	if( cfsetospeed( &newtermios, baudios ) == -1 )
	{
		printf("Error al establecer velocidad de salida \n");
		exit( EXIT_FAILURE );
  	}
// Configura la velocidad de entrada del UART
	if( cfsetispeed( &newtermios, baudios ) == -1 )
	{
		printf("Error al establecer velocidad de entrada \n" );
		exit( EXIT_FAILURE );
	}
// Limpia el buffer de entrada
	if( tcflush( fd, TCIFLUSH ) == -1 )
	{
		printf("Error al limpiar el buffer de entrada \n" );
		exit( EXIT_FAILURE );
	}
// Limpia el buffer de salida
	if( tcflush( fd, TCOFLUSH ) == -1 )
	{
		printf("Error al limpiar el buffer de salida \n" );
		exit( EXIT_FAILURE );
	}
/*
 * Se establece los parametros de terminal asociados con el
 * descriptor de archivo fd utilizando la estructura termios
 * TCSANOW - Cambia los valores inmediatamente
 */
	if( tcsetattr( fd, TCSANOW, &newtermios ) == -1 )
	{
		printf("Error al establecer los parametros de la terminal \n" );
		exit( EXIT_FAILURE );
	}
//Retorna el descriptor de archivo
	return fd;
}

