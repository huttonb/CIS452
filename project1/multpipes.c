#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define io 2

int main(){
	//int status;
	pid_t* listProc;
	pid_t pid;

	//Creates pipe

	//Details number of computers, this is a temporary number
	int numOfComps = 5;

	int fd[io * numOfComps];
	//Creates pipe of fd[2]
	if (pipe(&fd[0]) < 0){
		perror("Pipe issue.");
		exit(1);
	}

	//Malloc memory for the list of processes, set proc[0] as parent.
	listProc = (pid_t*)malloc(sizeof(pid_t) * numOfComps);
	listProc[0] = getpid();
	//Fill malloc array with list of processes. Process [0] is parent.
	//Each loop a fork is created, the parent leaves the loop, the child continues
	//And a Pipe is created linking each parent and child.
	for (int i = 1; i < numOfComps; i++){
		//Create pipe so that it exists in both child and parent
		if(pipe(&fd[io*i]) < 0){
				perror("Pipe issue.");
				exit(1);
				}
		
		if ((pid = fork()) < 0){
			printf("Error! Fork was not successful!");
			exit(2);
		}//If parent then add the PID to the list of processes, then exit loop
		else if(pid){
			listProc[i] = pid;
			i = numOfComps;
			printf("Child:%d created. Parent is:%d\n", pid, getpid());
		
		}
		//if child stay in loop
		else{
			
		}
		//If child maybe wait until parent has finished creating all of the parents, or just immediately go into a while loop waiting for information.


	}
	

	free(listProc);
	return 0;
}
