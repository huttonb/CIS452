#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	//int status;
	pid_t* listProc;
	pid_t pid;
	//Details number of computers, this is a temporary number
	int numOfComps = 5;
	//Fill malloc array with list of processes. Process[0] will be parent
	listProc = (pid_t*)malloc(sizeof(pid_t) * numOfComps);
	listProc[0] = getpid();
	for (int i = 1; i < numOfComps; i++){
		if ((pid = fork()) < 0){
			printf("Error! FOrk was not successful!");
			exit(1);
		}
		else if(pid){
			listProc[i] = pid;
		}
		//if child exit loop
		else{
			//Wait until parent has tuned listProc into pid.
			i = numOfComps;
			printf("\nChild# %d has been created.", getpid());	
		}
		//If child maybe wait until parent has finished creating all of the parents, or just immediately go into a while loop waiting for information.


	}
	

	free(listProc);
	return 0;
}
