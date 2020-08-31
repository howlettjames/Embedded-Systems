/** @brief: Este programa muestra el uso del UART
*/
/** @brief muestreo.c, este programa captura una señal de pulso cardíaco por UART
*   y la procesa usando, ventana de Hamming, Autocorrelación y ...
*/
#include <pthread.h>
#include "archivos.h"
#include "procesamiento.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <netinet/in.h>
#include <termios.h>
#include <fcntl.h>

#define EVER 1

void *hilo_hamming(void *arg);
void *hilo_autocorr(void *arg);
int config_serial(char *, speed_t);
void to_daemon(void);

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

    to_daemon();

	fd_serie = config_serial("/dev/ttyUSB0", B19200);
	syslog(LOG_INFO, "serial abierto con descriptor: %d\n", fd_serie);

	//Leemos datos del UART
	dato = 0;
	for(int i = 0; i < MUESTRAS; i++)
	{
		read(fd_serie, &dato, 1);
		syslog(LOG_INFO, "%d\n", dato);
		pulso[i] = ((float) dato) - 135.0;
	}
	genera_hamming(hamming);
	guarda_datos(hamming, "hamming.dat");
	guarda_datos(pulso, "pulso_raw.dat");

	// ----------------------------------------------------------------------------
	syslog(LOG_INFO, "Aplicando Hamming\n");
	for(nh = 0; nh < NO_HILOS; nh++)
    {
        nhs[nh] = nh;
        pthread_create(&tids[nh], NULL, hilo_hamming, (void *) &nhs[nh]);
    }

    for(nh = 0; nh < NO_HILOS; nh++)
    {
        pthread_join(tids[nh], (void **) &res_nh);
        syslog(LOG_INFO, "Hilo hamming %d terminado\n", *res_nh);
    }
    syslog(LOG_INFO, "Hamming terminado\n");
    guarda_datos(resultado_hamming, "pulso_hamming.dat");
    // ----------------------------------------------------------------------------
    syslog(LOG_INFO, "Aplicando Autocorrelación\n");
    for(nh = 0; nh < MUESTRAS; nh++)
    {
        nhs[nh] = nh;
        pthread_create(&tids[nh], NULL, hilo_autocorr, (void *) &nhs[nh]);
    }

    for(nh = 0; nh < MUESTRAS; nh++)
    {
        pthread_join(tids[nh], (void **) &res_nh);
        syslog(LOG_INFO, "Hilo autocorr %d terminado\n", *res_nh);
    }
    syslog(LOG_INFO, "Atocorrelación terminada\n");
    guarda_datos(resultado_autocorr, "pulso_autocorr.dat");
	// ----------------------------------------------------------------------------
    syslog(LOG_INFO, "Aplicando Algoritmo para encontrar frecuencia\n");    
    for(i = 150; i < MUESTRAS; i++)
    {
        if(resultado_autocorr[i - 1] < resultado_autocorr[i])
            if(resultado_autocorr[i + 1] < resultado_autocorr[i])
                if(max < resultado_autocorr[i])
                {
                    max = resultado_autocorr[i];
                    index_max = i;
                }
    }

    syslog(LOG_INFO, "Segundo mas alto: %f\n", max);
    syslog(LOG_INFO, "Segundo mas alto posicion: %d\n", index_max);
    frecuencia_pulso = 512 / (float)index_max;
    syslog(LOG_INFO, "Frecuencia senal: %f\n", frecuencia_pulso);
    pulsaciones_min = frecuencia_pulso * 60;
    syslog(LOG_INFO, "Pulsaciones min: %f\n", pulsaciones_min);
	
	close(fd_serie);

	return 0;
}

void *hilo_hamming(void *arg)
{
	int nh = *(int *) arg;
    register int i = 0;

    syslog(LOG_INFO, "Hilo %d ejecutado\n", nh);

    for(i = nh; i < MUESTRAS; i += NO_HILOS)
        resultado_hamming[i] = pulso[i] * hamming[i];
    
    pthread_exit(arg);
}

void *hilo_autocorr(void *arg)
{
	int nh = *(int *) arg; // nh = l
    register int n = 0;

    syslog(LOG_INFO, "Hilo %d ejecutado\n", nh);

    resultado_autocorr[nh] = 0;
    for(n = nh; n < MUESTRAS; n++)
        resultado_autocorr[nh] += resultado_hamming[n] * resultado_hamming[n - nh]; // pulso? 

   pthread_exit(arg);
}

void to_daemon(void)
{
    // DEMONIO VARS
    FILE *apArch;
    pid_t pid = 0;
    pid_t sid = 0;
    
    // Se crea el proceso hijo
    pid = fork();
    if( pid == -1 )
    {
        perror("Error al crear el primer proceso hijo\n");
        exit(EXIT_FAILURE);
    }

    if( pid )
    {
        printf("Se termina proceso padre, PID del proceso hijo %d \n", pid);
        exit(0);
    }

    umask(0);

    sid = setsid();
    if( sid < 0 )
    {
        perror("Error al iniciar sesion");
        exit(EXIT_FAILURE);
    }
    // Se realiza un segundo fork para separarnos completamente de la sesion del padre
    pid = fork( );
    if( pid == -1 )
    {
        perror("Error al crear el segundo proceso hijo\n");
        exit(EXIT_FAILURE);
    }
    if( pid )
    {
        printf("PID del segundo proceso hijo %d \n", pid);
        apArch = fopen("/home/james/pulso_demonio.pid", "w");
        fprintf(apArch, "%d", pid);
        fclose(apArch);

        exit(0);
    }
    chdir("/");

    close( STDIN_FILENO  );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );
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
		syslog(LOG_INFO, "Error al abrir el dispositivo tty \n");
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
		syslog(LOG_INFO, "Error al establecer velocidad de salida \n");
		exit( EXIT_FAILURE );
  	}
// Configura la velocidad de entrada del UART
	if( cfsetispeed( &newtermios, baudios ) == -1 )
	{
		syslog(LOG_INFO, "Error al establecer velocidad de entrada \n" );
		exit( EXIT_FAILURE );
	}
// Limpia el buffer de entrada
	if( tcflush( fd, TCIFLUSH ) == -1 )
	{
		syslog(LOG_INFO, "Error al limpiar el buffer de entrada \n" );
		exit( EXIT_FAILURE );
	}
// Limpia el buffer de salida
	if( tcflush( fd, TCOFLUSH ) == -1 )
	{
		syslog(LOG_INFO, "Error al limpiar el buffer de salida \n" );
		exit( EXIT_FAILURE );
	}
/*
 * Se establece los parametros de terminal asociados con el
 * descriptor de archivo fd utilizando la estructura termios
 * TCSANOW - Cambia los valores inmediatamente
 */
	if( tcsetattr( fd, TCSANOW, &newtermios ) == -1 )
	{
		syslog(LOG_INFO, "Error al establecer los parametros de la terminal \n" );
		exit( EXIT_FAILURE );
	}
//Retorna el descriptor de archivo
	return fd;
}

