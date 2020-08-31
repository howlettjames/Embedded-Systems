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

#define PUERTO 			5000	//Número de puerto asignado al servidor
#define COLA_CLIENTES 	5 	//Tamaño de la cola de espera para clientes
#define TAM_BUFFER 		100
#define N 				1024

void isr( int sig);
int config_serial ( char *, speed_t );
void to_daemon(void);

int main(int argc, char **argv)
{
   	int sockfd, cliente_sockfd;
   	struct sockaddr_in direccion_servidor; //Estructura de la familia AF_INET, que almacena direccion
   	char leer_mensaje[TAM_BUFFER];
	pid_t pid;
	register int i;
	int buffer_index, string_index, no_times;
	int fd_serie;
	unsigned char dato;
	unsigned char buffer[100], buffer_string[50];
	
	to_daemon();

	// Se abre un archivo log en modo de escritura.
    openlog( "demonio", LOG_NDELAY | LOG_PID, LOG_LOCAL0 );

	fd_serie = config_serial("/dev/ttyACM0", B9600 );
	syslog(LOG_INFO, "serial abierto con descriptor: %d\n", fd_serie);

	/*	
	 *	AF_INET - Protocolo de internet IPV4
	 *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
	 *  en arquitecturas little-endian se deben revertir los bytes
	 *  INADDR_ANY - Se elige cualquier interfaz de entrada
	 -*/	
   	memset (&direccion_servidor, 0, sizeof (direccion_servidor));	//se limpia la estructura con ceros
   	direccion_servidor.sin_family 		= AF_INET;
   	direccion_servidor.sin_port 		= htons(PUERTO);
   	direccion_servidor.sin_addr.s_addr 	= INADDR_ANY;
	
	if(signal(SIGCHLD, isr) == SIG_ERR)
	{
		syslog(LOG_INFO, "Error al signar la señal\n");
		exit(EXIT_FAILURE);
	}

	/*
	 *	Creacion de las estructuras necesarias para el manejo de un socket
	 *  SOCK_STREAM - Protocolo orientado a conexión
	 */
   	syslog(LOG_INFO, "Creando Socket ....\n");
   	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		syslog(LOG_INFO, "Ocurrio un problema en la creacion del socket");
		exit(1);
   	}
	/*
	 *  bind - Se utiliza para unir un socket con una dirección de red determinada
	 */
   	syslog(LOG_INFO, "Configurando socket ...\n");
   	if( bind(sockfd, (struct sockaddr *) &direccion_servidor, sizeof(direccion_servidor)) < 0 )
	{
		syslog(LOG_INFO, "Ocurrio un problema al configurar el socket");
		exit(1);
   	}
	/*
	 *  listen - Marca al socket indicado por sockfd como un socket pasivo, esto es, como un socket
	 *  que será usado para aceptar solicitudes de conexiones de entrada usando "accept"
	 *  Habilita una cola asociada la socket para alojar peticiones de conector procedentes
	 *  de los procesos clientes
	 */
   	syslog(LOG_INFO, "Estableciendo la aceptacion de clientes...\n");
	if( listen(sockfd, COLA_CLIENTES) < 0 )
	{
		syslog(LOG_INFO, "Ocurrio un problema al crear la cola de aceptar peticiones de los clientes");
		exit(1);
   	}
	/*
	 *  accept - Aceptación de una conexión
	 *  Selecciona un cliente de la cola de conexiones establecidas
	 *  se crea un nuevo descriptor de socket para el manejo
	 *  de la nueva conexion. Apartir de este punto se debe
	 *  utilizar el nuevo descriptor de socket
	 *  accept - ES BLOQUEANTE 
	 */
	while(1)
	{
	   	syslog(LOG_INFO, "En espera de peticiones de conexión ...\n");
	   	if( (cliente_sockfd = accept(sockfd, NULL, NULL)) < 0 )
         {
   			syslog(LOG_INFO, "Ocurrio algun problema al atender a un cliente");
   			exit(1);
	   	}
		pid = fork();
		if(!pid)
		{
			while(1)
			{
				dato = 0;
				buffer_index = 0;
				while(dato != '\n')
				{
					read(fd_serie, &dato, 1);	
					buffer[buffer_index++] = dato;
				}
				buffer[buffer_index] = '\0';

				if(buffer[3] == 'G' && buffer[4] == 'L' && buffer[5] == 'L')
				{
					syslog(LOG_INFO, "This is a GPGLL string: \n");
					syslog(LOG_INFO, "%s\n", buffer);
					no_times = 0;
					while(no_times < 3)
					{
						if(no_times == 0)
						{
							buffer_index = 7;
							string_index = 0;
							while(buffer[buffer_index] != ',')
								buffer_string[string_index++] = buffer[buffer_index++];

							buffer_string[string_index++] = buffer[++buffer_index];	// Inserting 'N' into string
							buffer_string[string_index] = '\0';
							syslog(LOG_INFO, "Latitude: %s\n", buffer_string);
							if(write(cliente_sockfd, buffer_string, strlen(buffer_string)) < 0 )
							{
				   				syslog(LOG_INFO, "Ocurrio un problema en el envio de un mensaje al cliente");
				   				exit(1);
						   	}	
							buffer_index += 2; // Skipping 'N,'
						}
						else if(no_times == 1)
						{
							string_index = 0;
							while(buffer[buffer_index] != ',')
								buffer_string[string_index++] = buffer[buffer_index++];					

							buffer_string[string_index++] = buffer[++buffer_index];	// Inserting 'W' into string
							buffer_string[string_index] = '\0';
							syslog(LOG_INFO, "Longitude: %s\n", buffer_string);
							if(write(cliente_sockfd, buffer_string, strlen(buffer_string)) < 0 )
							{
				   				syslog(LOG_INFO, "Ocurrio un problema en el envio de un mensaje al cliente");
				   				exit(1);
						   	}
							buffer_index += 2; // Skipping 'W,'
						}
						else if(no_times == 2)
						{
							string_index = 0;
							while(buffer[buffer_index] != ',')
								buffer_string[string_index++] = buffer[buffer_index++];					

							buffer_string[string_index] = '\0';	
							syslog(LOG_INFO, "Fix Taken at: %s\n", buffer_string);
							
							if(buffer[++buffer_index] == 'A')
								syslog(LOG_INFO, "Data Active\n");
							else
								syslog(LOG_INFO, "Void\n");

							buffer_index += 2;
							string_index = 0;
							while(buffer[buffer_index] != '\0')
								buffer_string[string_index++] = buffer[buffer_index++];					

							buffer_string[string_index] = '\0';
							syslog(LOG_INFO, "Checksum data: %s\n", buffer_string);
						}	
						no_times++;
					}
					break;
				}
				else
					syslog(LOG_INFO, "%s\n", buffer);
			}
			close(cliente_sockfd);
			exit(0);
		}
	}
  	syslog(LOG_INFO, "Concluimos la ejecución de la aplicacion Servidor \n");
   	close (sockfd);

	return 0;
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
		apArch = fopen("/home/james/mi_demonio.pid", "w");
		fprintf(apArch, "%d", pid);
		fclose(apArch);

		exit(0);
    }
    chdir("/");

    close( STDIN_FILENO  );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );
}

void isr(int sig)
{
	int estado;
	pid_t pid;

	if(sig == SIGCHLD)
	{
		pid = wait(&estado);
		syslog(LOG_INFO, "Señal CHLD detectada en proceso hijo: %d\n", pid);

	}
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
