#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

/**********************************************************************
 * Communicating processes program that install and register signal 
 * handlers for the user defined signals. Spawns off a child process
 * that continuously send signals to the parent process. The child 
 * waits 1-5 seconds randomly before sending a user defined signal. 
 * Program terminates with Contol-C.
 *
 * Author: Edric Lin
 * Author: Bryce Hutton
 * Verson: 9/19/18
 **********************************************************************/

/**************************************************
 * Signal handler method used in sigaction call.
 * 
 * Param: sigNum the signal being sent
 **************************************************/
void sigHandler(int sigNum);

/**************************************************
 * Main method to run the communicating processes
 * program. 
 **************************************************/
int main(int argc, char** argv) {

	pid_t pid;
	
	// install handler for SIGUSR1 and SIGUSR2
	signal(SIGUSR1, sigHandler);
	signal(SIGUSR2, sigHandler);

	// create a child process
	pid = fork();

	//printf("%d\n", SIGUSR1);
	
	// fork failed
	if (pid < 0) {
		printf("Error! Fork was not successful!");
	}
	
	// process is parent, wait for child
	else if (pid) {

		// install handler for SIGINT
		signal(SIGINT, sigHandler);	

		// print parent and child of first fork
		printf("parent PID# is %d\n", getpid());	
		printf("spawned child PID# %d\n", pid);
		
		// keep checking for signals
		while(1) {
			fprintf(stderr, "waiting...\t");
			pause();
		}
	}

	// process is child
	else {

		// print child pid
		//printf("spawned child PID# %d\n", getpid());

		while(1) {

			// wait for signal
			//fprintf(stderr, "waiting...\t");
			
			// random number between 1-5
			int r = rand() % 4 + 1;
		
			// sleep for 1-5 seconds
			sleep(r);

			// if random number is odd, send SIGUSR1, else send SIGUSR2
			if (r % 2) {
				kill(getppid(), SIGUSR1);
			} else {
				kill(getppid(), SIGUSR2);
			}
		}
	}	

	return 0;
}


/**************************************************
 * Signal handler method used in sigaction call.
 * 
 * Param: sigNum the signal being sent
 **************************************************/
void sigHandler(int sigNum) {

	// signal is SIGUSR1
	if (sigNum == SIGUSR1) {
		printf("received a SIGUSR1 interrupt.\n");

	}

	// signal is SIGUSR2
	if (sigNum == SIGUSR2) {
		printf("received a SIGUSR2 interrupt.\n");

	}

	// signal is SIGINT, quit program
	if (sigNum == SIGINT) {
		// shutdown
		printf(" recieved. That's it, I'm shutting you down...\n");
		exit(0);
	}
} 
