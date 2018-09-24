#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>


#define MAX 256

void sigHandler(int sigNum);
void* findFile(void* arg);
int threadsHandled;

int main(){
	pthread_t thread1;
	signal(SIGINT, sigHandler);
	int status;
	char user_input[MAX];

	while(1){
	
	printf("Enter string:");
	fgets(user_input, MAX, stdin);
	//Create a worker thread that executes findFile
	if ((status = pthread_create (&thread1, NULL, findFile, &user_input)) != 0){
			fprintf (stderr, "Thread create error %d: %s\n", status, strerror(status));
			exit (1);

		}
	else
		threadsHandled++;

	int pthread_detach (pthread_t thread1);

	}
}

void sigHandler(int sigNum){
	//Print out statistics here
		printf("%d files found and %d files serviced!\n", threadsHandled, threadsHandled);	
		exit(0);
}

void* findFile(void* arg){
	char *fileName = (char*) arg;
	pid_t tid = syscall(__NR_gettid);
	//Creates a random number between 0-3	
	int r = rand() % 4;
	if(r != 0){
		sleep(1);
	}
	else{
		int n = (rand() % 3) + 7;
		sleep(n);
	}
		printf("\nthread#%d found :%s\n", tid,fileName);

		return NULL;
	
}
