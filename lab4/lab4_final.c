#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX 30

/**********************************************************************
 * Program to simulate a blocking multi-threaded server. Dispatch
 * thread take user input for filename and spawns a child thread to 
 * obtain the file. The child sleeps for a certain amount of time to
 * simulate file access scenarios, and returns file found to parent.
 * Dispatch thread runs until Control-C interrupt is given.
 *
 * Author: Bryce Hutton
 * Author: Edric Lin
 * Version 9/26/18
 **********************************************************************/

/**************************************************
 * Function to handle signals. Used to terminate
 * program and print out final program statistics.
 *
 * Param: sigNum the signal to handle
 **************************************************/
void sigHandler(int sigNum);

/**************************************************
 * Function used by child thread to simulate file
 * access. Sleeps random times to similate access
 * times.
 *
 * Param: arg the filename to search for
 **************************************************/
void* findFile(void* arg);

/* number of requests received */
int received = 0;

/* number of requests serviced */
int serviced = 0;

/* counter for total access time */
double counter = 0;

/* mutex variable */
pthread_mutex_t lock;

/**************************************************
 * Main method to run the blocking multi-threaded
 * server file access simulation. 
 **************************************************/
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

/**************************************************
 * Function to handle signals. Used to terminate
 * program and print out final program statistics.
 *
 * Param: sigNum the signal to handle
 **************************************************/
void sigHandler(int sigNum) {
	if (sigNum == SIGINT) {

		printf(" received. Terminating program.\n");

		// calculate average time
            	double avg_time = (counter / serviced);

		// print out statistics
            	printf("Requests received: %d\n", received);
            	printf("Requests serviced: %d\n", serviced);
            	printf("Average time: %f seconds\n", avg_time);
            	pthread_mutex_destroy(&lock);
            	exit(0);
	}
}

/**************************************************
 * Function used by child thread to simulate file
 * access. Sleeps random times to similate access
 * times.
 *
 * Param: arg the filename to simulate search on
 **************************************************/
void* findFile(void* arg) {

	char* filename;
	filename = (char*) malloc(sizeof(char) * strlen((char*) arg));
	memcpy(filename, (char*) arg, (strlen((char*) arg)) - 1);

	// sleep time
	int n;

	// random number 0-4
	int r = rand() % 5;

	// 1 through 4, i.e. 80%
	if (r != 0)

		// set sleep time to 1 sec
        	n = 1;
	
	// 0, i.e. 20%
    	else

		// set sleep time to 7-10 seconds
             	n = rand() % 4 + 7;


    	sleep(n);

    	printf("\nfound: %s\n", filename);

    	serviced++;

    	pthread_mutex_lock(&lock);
    	counter += n;
    	pthread_mutex_unlock(&lock);


    	free(filename);
    	return NULL;
}
