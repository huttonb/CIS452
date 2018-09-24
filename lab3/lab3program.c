#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sigHandler(int);

int main(){
	pid_t pid;
	int status;
	signal(SIGUSR1, sigHandler);
	signal(SIGUSR2, sigHandler);
	pid = fork();
	if (pid < 0){
		printf("Error! Fork was not successful.");
	}
	//Parent process
	else if (pid) {
		signal(SIGINT, sigHandler);
		printf("spawned child PID# %d\n", pid);
		wait(&status);
	}
	//Child process waits from 1-5 seconds then sends a random signal to its parent
	else{
		while(1){
		int r = (rand() % 4)+1;
		fprintf(stderr, "waiting...       ");
		sleep(r);
		if(r % 2)
			kill(getppid(),SIGUSR1);
		else	
			kill(getppid(),SIGUSR2);
		}
		
	
	}

	return 0;

}

void sigHandler(int sigNum){
	if(sigNum == SIGUSR1)
		printf("Received a SIGUSR1 interrupt\n");
	else if (sigNum == SIGUSR2)
		printf("Received a SIGUSR2 interrupt\n");
	else if (sigNum == SIGINT){
		printf(" received. That's it, I'm shutting you down...\n");
		exit(0);
	}	
		
}
