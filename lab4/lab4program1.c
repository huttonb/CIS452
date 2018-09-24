#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX 30

void sigHandler(int sigNum);
void* findFile(void* arg);

int received = 0;
int serviced = 0;
double counter = 0;
pthread_mutex_t lock;

int main(int argc, char** argv) {
    pthread_t thread1;
    signal(SIGINT, sigHandler);
    int status;
    char user_input[MAX];

    if (pthread_mutex_init(&lock, NULL) != 0){
		    //printf(stderr, "Mutex create error %d: %s\m" status, strerr(status));
		    exit(1);
    }
    while(1) {
   	 printf("Enter string: ");
   	 fgets(user_input, MAX, stdin);   	 
  	      //Create a worker thread that executes findFile
   	 if ((status = pthread_create(&thread1, NULL, findFile, &user_input)) != 0){
  			 fprintf (stderr, "Thread create error %d: %s\n", status, strerror(status));
   			 exit (1);
   	 }

   	 received++;
	 pthread_detach (thread1);
} 

    return 0;
}

void sigHandler(int sigNum) {
    if (sigNum == SIGINT) {
	    	double avg_time = (counter / serviced);
		printf("\nRequests received: %d,", received);
		printf("Requests serviced: %d", serviced);
		printf("\nAverage time is: %f", avg_time);
		pthread_mutex_destroy(&lock);
		exit(0);

    }
}

void* findFile(void* arg) {
    
    char* filename;
    filename = (char*) malloc(sizeof(char) * (strlen((char*) arg) + 1));
    memcpy(filename, (char*) arg, strlen((char*) arg));

    int r = rand() % 4;
    int n;
    // 1 through 4, i.e. 80%
    if (r != 0) 
	    n = 1; 
    // 0, i.e. 20%
	else 
		 n = rand() % 3 + 7;
	sleep(n);
	printf("\nfound: %s\n", filename);
	
	serviced++;
	
	pthread_mutex_lock(&lock);
	counter += n;
	pthread_mutex_unlock(&lock);
	

	free(filename);
	return NULL;
}

