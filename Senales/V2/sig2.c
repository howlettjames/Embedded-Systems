#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define PID 5921

int main(void)
{
	kill(PID, SIGINT);
	sleep(5);
	kill(PID, SIGTERM);
	sleep(5);
	kill(PID, SIGKILL);
	sleep(5);
	
	return 0;
}
