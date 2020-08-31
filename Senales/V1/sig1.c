#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void isr(int sig);

int main(void)
{
	if(signal(SIGINT, isr) == SIG_ERR)
	{
		perror("Error en la ISR");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGTERM, isr) == SIG_ERR)
	{
		perror("Error en la ISR");
		exit(EXIT_FAILURE);
	}
	
	while(1);
		pause();

	return 0;
}

void isr(int sig)
{
	if(sig == SIGINT)
		printf("No voy a terminar!\n");
	if(sig == SIGTERM)
		printf("No voy a terminar! tampoco\n");
}
