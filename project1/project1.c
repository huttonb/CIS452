#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(){
	//int status;
	pid_t* listProc;
	pid_t pid;

	//Creates pipe

	//Details number of computers, this is a temporary number
	int numOfComps = 5;

	//Creates two arrays for pipes
	int fd[2];
	int fd2[2];

	//Creates first pipe in the process, we only want this to be created once, and keep a pointer so that the last pipe can connect with it.
	if(pipe(fd) < 0){
		perror("Pipe issues.");
		exit(1);
	}
	int* fdp = fd;

	//Malloc memory for the list of processes, set proc[0] as parent.
	listProc = (pid_t*)malloc(sizeof(pid_t) * numOfComps);
	listProc[0] = getpid();
	//Fill malloc array with list of processes. Process [0] is parent.
	//Each loop a fork is created, the parent leaves the loop, the child continues
	//Two pipes exist.
	for (int i = 1; i < numOfComps; i++){
		int fd2[2];
		//Create pipe so that it exists in both child and parent
		if(pipe(fd2) < 0){
				perror("Pipe issues.");
				exit(1);
		}
		
		if ((pid = fork()) < 0){
			printf("Error! Fork was not successful!");
			exit(2);
		}//If parent then add the PID to the list of processes, then exit loop
		else if(pid){
			listProc[i] = pid;
			i = numOfComps;
			//Add parent process to write pipe
	//		dup2(fd2[READ], STDIN_FILENO);
			printf("Child:%d created. Parent is:%d\n", pid, getpid());
		
		}
		//if child stay in loop. If last step of loop, link pipe with original pipe.
		else{
			//if it's the last step of the loop (the last child), link with parent.
			if(i == (numOfComps - 1 )){
				 fd2 = fdp;
			}
			//change fd2 to fd, so that each process only has access to two pipes
			else
				 fd = fd2;

			
			//Dup pipe write to next pipes read
			dup2(fd[WRITE], fd2[READ]);
			
			
			
		}
		//If child maybe wait until parent has finished creating all of the parents, or just immediately go into a while loop waiting for information.


	}
	
	free(listProc);
	return 0;
}
