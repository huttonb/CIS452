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

pthread_mutex_t lock;

struct Msg {
	int dest;
	char txt[BODY];
	int flag;
};


int loop = 1;

int main(){
	pid_t pid;
	pthread_t thread1;
	int status;
	struct Msg *msg = malloc(sizeof(msg));
	struct Msg *workerMsg = malloc(sizeof(msg));
	char* input = (char*)malloc(sizeof(char)*BODY);
	char emptyTxt[BODY] = {0};
	int numOfComps;
	int id = 1;
	workerMsg->dest = 0;
	workerMsg->flag = 0;
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
		if(pthread_mutex_init(&lock, NULL) != 0)
			exit(1);
		fdRead = fdp;
		if ((status = pthread_create(&thread1, NULL, handleInput, workerMsg)) != 0){
			fprintf(stderr, "Thread create error %d: %s\n", status, strerror(status));
			exit(1);
		}
	}

	sleep(5);
	if(id == 1){
	//	if(workerMsg->flag == 1){
			write(fdWrite[WRITE], (const void *) msg, (size_t) MAX);
	//		memset(workerMsg->txt, 0, BODY);
	//		workerMsg->dest = 0;
	//		workerMsg->flag = 0;
	//	}
	}
	
	while(loop){				
		read(fdRead[READ], (void *) msg, (size_t) MAX);
		printf("(dest: %d)(msg: %s)", msg->dest, msg->txt);
		printf("(%d == %d)", id, msg->dest);
		pthread_mutex_lock(&lock);
		if (id == msg->dest){
			printf("Process%d received message:%s\n", id, msg->txt);	
			//memset(msg->txt, 0, BODY);
			msg->dest = 0;
			write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
		}
		//If it is the original process, and the destination is nonexistent or 1
		
		else if(id == 1 && msg->dest == 0){
			//Check to see if dest is 0 or 1
			//If dest is 0 then message needs to be reset to zero or there needs to a message passed
			//If there's a flag, pass a message, otherwise reset to zero and continue on.
			if(workerMsg->flag == 1){
				write(fdWrite[WRITE], (const void*) workerMsg, (size_t) MAX);
			//	strncpy(msg->txt, workerMsg->txt, BODY);
			//	msg->dest = workerMsg->dest;
				printf("Blankity Blank");
				workerMsg->flag = 0;
				pthread_mutex_unlock(&lock);
			}
			else{
				strncpy(msg->txt, emptyTxt, BODY);
				msg->txt[BODY-1] = '\0';
				printf("Not blank");
				write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
			}
			printf("\nPROCESS 1: MSG= %s, DEST=%d, flag:%d\n", msg->txt, workerMsg->dest, workerMsg->flag);
		
			
		
		}	
		else{
			printf("Process%d received \"%s\" and is forwarding it.\n", id, msg->txt);
			sleep(1);
			write(fdWrite[WRITE], (const void*) msg, (size_t) MAX);
		}
		pthread_mutex_unlock(&lock);
	}
	
	free(fdRead);
	free(fdWrite);
	free(input);
	free(msg);

	return 0;

}

//Worker thread is sent here in order to handle the input.
void* handleInput(void* arg){
	struct Msg *msg1 = (struct Msg*)arg;
	char* input1 = (char*)malloc(sizeof(char)*BODY);
	msg1->flag = 0;
	while(1){
		pthread_mutex_lock(&lock);
		if(msg1->flag == 0){
			printf("Enter destination: ");
			msg1->dest = atoi(fgets(input1, HEADER-1, stdin));
			printf("Enter message: ");
			fgets(msg1->txt, BODY-1, stdin);
			//Removes trailing newline
			strtok(msg1->txt, "\n");
			msg1->flag = 1;
			pthread_mutex_unlock(&lock);
			printf("DestId is: \"%d\"\n", msg1->dest);
			printf("msg->txt is: \"%s\"\n", msg1->txt);
		}
		pthread_mutex_unlock(&lock);
	}
	free(input1);

}

void sigHandler(int sigNum){
	if (sigNum == SIGUSR1){
		return;
	}
	return;
}
