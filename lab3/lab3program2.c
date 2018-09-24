#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

/**********************************************************************
 * Communicating processes program that uses sigaction to install and
 * register signal handlers. Spawns off two child processes that
 * continuously send signals to the parent process. Each child waits
 * 1-5 seconds randomly before sending a user defined signal. Program
 * terminates with Contol-C.
 *
 * Author: Edric Lin
 * Author: Bryce Hutton
 * Verson: 9/19/18
 **********************************************************************/

/**************************************************
 * Signal handler method used in sigaction call.
 * 
 * Param: signum the signal being sent
 * Param: info data structure containing more
 * 	information about the signal
 * Param: context data structure containing signal
 *	context information
 **************************************************/
void sigHandler(int signum, siginfo_t* info, void* context);

/**************************************************
 * Main method to run the communicating processes
 * program. 
 **************************************************/
int main(int argc, char** argv) {

	pid_t pid;

	
	// struct used for sigaction call
	struct sigaction s;
	s.sa_sigaction = &sigHandler;
   	sigemptyset(&s.sa_mask);
	s.sa_flags = SA_SIGINFO;
	
	// install handler for SIGUSR1 and SIGUSR2
	sigaction(SIGUSR1, &s, NULL);
	sigaction(SIGUSR2, &s, NULL);
	
	// create a child process
	pid = fork();
	
	// fork failed
	if (pid < 0) {
		printf("Error! Fork was not successful!");
	}
	
	// process is parent, wait for child
	else if (pid) {

		// install handler for SIGINT
		sigaction(SIGINT, &s, NULL);

		// print parent and child of first fork
		printf("parent PID# is %d\n", getpid());	
		printf("spawned child PID# %d\n", pid);
		
		// keep checking for signals
		while(1) {
			//fprintf(stderr, "waiting...\t");
			pause();
		}	
	}

	// process is child
	else {
		
		// fork as child to create another child
		fork();
		
		// two children continuously send signals
		while(1) {
			
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
 * param: signum the signal being sent
 * param: info data structure containing more
 * 	information about the signal
 * param: context data structure containing signal
 *	context information
 **************************************************/
void sigHandler(int signum, siginfo_t* info, void* context) {

	// signal is SIGUSR1
	if (signum== SIGUSR1) {
		printf("received a SIGUSR1 interrupt from PID# %d.\n", info->si_pid);

	}

	// signal is SIGUSR2
	if (signum== SIGUSR2) {
		printf("received a SIGUSR2 interrupt from PID# %d.\n", info->si_pid);

	}
	
	// signal is SIGINT, quit program
	if (signum== SIGINT) {
		// shutdown
		printf(" recieved. That's it, I'm shutting you down...\n");
		exit(0);
	}
} 

