/** @brief: Este programa configura el BCM_GPIO 17 usando la libreria wiringPi
 * El GPIO 17 corresponde con el pin 0 en wiring Pi
 * Se debe compilar con: gcc gpio.c -o gpio -lwiringpi
 */

#include<stdio.h>
#include<unistd.h>
#include<wiringPi.h>

int main( )
{
//Se inicializa la libreria wiring Pi
	wiringPiSetup();
//Se configura el GPIO 17 como salida
	pinMode( 0, OUTPUT );

//Se escribe un valor digital al GPIO
	while( 1 )
	{
		digitalWrite( 0, HIGH );
	//	sleep(5);
		digitalWrite( 0, LOW );
	//	sleep(5);
	}
	return 0;
}
