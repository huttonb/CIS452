/**
 * Author: Bryce Hutton
 * Date 10/1/2018
 *
 * Program: Creates a daisy-chain of processes, where you can pass a
 * message from one process to the other via a constantly circulating
 * token that the processes can grab and fill with information.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#define READ 0
#define WRITE 1
#define HEADER 32
#define BODY 224
#define MAX 256

void sigHandler(int sigNum);

int main(){
	pid_t pid;
	char* msg = (char*)malloc(sizeof(char)*MSG);
	char* dest = (char*)malloc(sizeof(char)*HEADER);
	int numOfComps;
	int id = 1;
	int destId;
	//Install handler for SIGUSR1
	signal(SIGUSR1, sigHandler);
 
	printf("Enter Number of Processes: ");
	numOfComps = atoi(fgets(msg, MAX, stdin));

	//Allocate memory for pipes.
	int* fdWrite = (int*)malloc(sizeof(int)*2);
	int* fdRead = (int*)malloc(sizeof(int)*2);
	
	//Creates first pipe in process. We want numOfComps+1 pipes
	if(pipe(fdWrite) < 0){
		perror("Pipe issues.");
		exit(1);
	}

	//Secondary pointer to fdWrite for the last process to use.
	int* fdp = fdWrite;

	//Create shared memory array here:
	/*
	 */

	//Creates daisy chain of processes. Each loop forks, the parent leaves the loop
	//the child continues. Two pipes exist for each process.
	
	for(int i = 1; i < numOfComps; i++){
		//Creates new fdWrite for each function.
		fdWrite = (int*)malloc(sizeof(int)*2);
		if(pipe(fdWrite) < 0){
			perror("Pipe issues.");
			exit(1);
		}
		if ((pid = fork()) < 0){
			printf("Error! Fork was not successful!");
			exit(2);
		}
		// If parent then leave loop.
		else if (pid){
			id = i;
			printf("ID: %d, PID: %d\n", id, getpid());
			i = numOfComps;
			/*Add process to shared memory. */
		}//If the child, stay in the loop. If last step of loop, link with p1
		else{
			if(i == (numOfComps - 1)){
				fdWrite = fdp;
				//id = i+1 because usually id is assigned in second loop
				id = i+1;
				printf("ID: %d, PID: %d\n", id, getpid());
			}
			else{
				fdRead = fdWrite;
			}
		}
	}
	//If not the last process, pause, waits for last process to finish 
	if(id != numOfComps)
		pause();
	if( id > 1)
		kill(getppid(), SIGUSR1);
	
	
	if(id == 1){
		printf("Enter destination: ");
		dest = (fgets(msg, HEADER-1, stdin));
		destId = atoi(dest);
		printf("Enter message: ");
		msg = fgets(msg, BODY-1, stdin);
		msg = 
		//Removes trailing newline
		strtok(msg, "\n");
		msg[MAX-1] = '\0';
	printf("DestId is: \"%d\"\n", destId);
	}
	sleep(1);
	int i = 1;
	while(i){
		if(id == 1){
			write(fdWrite[WRITE], (const void *) msg, (size_t) MAX);
			i = 0;
		}
		else{
			read(fdRead[READ], (void *) msg, (size_t) MAX);
			printf("(dest: %s)(msg: %s)", dest, msg);
			destId = atoi(dest);
			printf("(%d == %d)", id, destId);
			if (id == 4){	
				printf("Process%d received message:%s\n", id, msg);	
				//MAKE buffers = 0
			}
			else{
				printf("Process%d received \"%s\" and is forwarding it.\n", id, msg);
				write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
			}
			i = 0;
		}

	}
	
	free(fdRead);
	free(fdWrite);
	return 0;
}

void sigHandler(int sigNum){
	if (sigNum == SIGUSR1){
		return;
	}
	return;
}
