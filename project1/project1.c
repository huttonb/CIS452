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
#include <pthread.h>

#define READ 0
#define WRITE 1
#define HEADER 32
#define BODY 224
#define MAX 256

void sigHandler(int sigNum);
void* handleInput(void* arg);

struct Msg {
	int dest;
	char txt[BODY];
};


int loop = 1;

int main(){
	pid_t pid;
	pthread_t thread1;
	int status;
	struct Msg *msg = malloc(sizeof(msg));
	struct Msg *workerMsg = malloc(sizeof(msg));
	char* input = (char*)malloc(sizeof(char)*BODY);
	int numOfComps;
	int id = 1;
	//Install handler for SIGUSR1
	signal(SIGUSR1, sigHandler);
 
	printf("Enter Number of Processes: ");
	numOfComps = atoi(fgets(input, MAX, stdin));
	printf("Number of comps %d\n", numOfComps);

	//Allocate memory for pipes.
	int* fdWrite;
	int* fdRead = (int*)malloc(sizeof(int)*2);
	
	//Creates first pipe in process. We want numOfComps+1 pipes
	if(pipe(fdRead) < 0){
		perror("Pipe issues.");
		exit(1);
	}

	//Secondary pointer to fdWrite for the last process to use.
	int* fdp = fdRead;

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
				fdRead = fdWrite;
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
	if(id != numOfComps){
		printf("ID:%d PAUSED, numofComps%d\n",id,numOfComps);
		pause();
	}
	if( id > 1){
		kill(getppid(), SIGUSR1);
		printf("Freed ID:%d\n", id-1);
	}
	sleep(1);
	//Creates thread in parent process that handles the input.
	if(id == 1){
		fdRead = fdp;
		if ((status = pthread_create(&thread1, NULL, handleInput, workerMsg)) != 0){
			fprintf(stderr, "Thread create error %d: %s\n", status, strerror(status));
			exit(1);
		}

	}




	sleep(5);
	if(id == 1)
		write(fdWrite[WRITE], (const void *) msg, (size_t) MAX);
	while(loop){				
		read(fdRead[READ], (void *) msg, (size_t) MAX);
		printf("(dest: %d)(msg: %s)", msg->dest, msg->txt);
		printf("(%d == %d)", id, msg->dest);
		if (id == msg->dest && id != 1){
			printf("Process%d received message:%s\n", id, msg->txt);	
			memset(msg->txt, 0, BODY);
			msg->dest = 0;
			write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
		}
		else if(id == 1 && msg->dest <= 0 && msg->txt[0] != '\0'){
			//If ID is one and destination is 0, has the chance to pass on a new message
			if(msg->dest != 1){
				if(workerMsg->dest != 0){
					write(fdWrite[WRITE], (const void *) workerMsg, (size_t) MAX);
					memset(workerMsg->txt, 0, BODY);
					workerMsg->dest = 0;
				}
				else{
					write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
					printf("Process%d received \"%s\" and is forwarding it.\n", id, msg->txt);

				}
			}//If destination is 1
			else{
				printf("Process%d received message:%s\n", id, msg->txt);	
				if(workerMsg->dest != 0){
					write(fdWrite[WRITE], (const void *) workerMsg, (size_t) MAX);
					memset(workerMsg->txt, 0, BODY);
					workerMsg->dest = 0;
					printf("Process%d is sending new message:%s\n", id, workerMsg->txt);				
				}
				else{
					write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
					printf("Process%d sending empty message.\n", id);
				}			
			}
		}
		else{
			printf("Process%d received \"%s\" and is forwarding it.\n", id, msg->txt);
			sleep(1);
			write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
		}
	}
	
	free(fdRead);
	free(fdWrite);
	free(input);
	free(msg);

	return 0;

}

//Worker thread is sent here in order to handle the input.
void* handleInput(void* arg){
	struct Msg *msg = (struct Msg*)arg;
	char* input = (char*)malloc(sizeof(char)*BODY);
	while(1){
		printf("Enter destination: ");
		msg->dest = atoi(fgets(input, HEADER-1, stdin));
		printf("Enter message: ");
		fgets(msg->txt, BODY-1, stdin);
		//Removes trailing newline
		strtok(msg->txt, "\n");
		printf("DestId is: \"%d\"\n", msg->dest);
		printf("msg->txt is: \"%s\"\n", msg->txt);
	}
	free(input);

}

void sigHandler(int sigNum){
	if (sigNum == SIGUSR1){
		return;
	}
	return;
}
