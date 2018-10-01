#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main(){
	//int status;
	pid_t pid;

	//Creates pipe

	//Details number of computers, this is a temporary number
	int numOfComps = 5;

	//fd = write, fd2 = read.
	//Creates two arrays for pipes
	int* fdWrite = (int*)malloc(sizeof(int) * 2);
	int* fdRead = (int*)malloc(sizeof(int) * 2);

	int id = 1;

	//Creates first pipe in the process, we only want this to be created once, and keep a pointer so that the last pipe can connect with it.
	if(pipe(fdWrite) < 0){
		perror("Pipe issues.");
		exit(1);
	}
	int* fdp = fdWrite;
	//Malloc memory for the list of processes, set proc[0] as parent.
	pid_t* listProc = (pid_t*)malloc(sizeof(pid_t) * numOfComps);
	listProc[0] = getpid();

	//Fill malloc array with list of processes. Process [0] is parent.
	//Each loop a fork is created, the parent leaves the loop, the child continues
	//Two pipes exist.
	for (int i = 1; i < numOfComps; i++){
		fdRead = (int*)malloc(sizeof(int)*2);
		//Create pipe so that it exists in both child and parent
		if(pipe(fdRead) < 0){
				perror("Pipe issues.");
				exit(1);
		}	
		if ((pid = fork()) < 0){
			printf("Error! Fork was not successful!");
			exit(2);
		}
		//If parent then add the PID to the list of processes, then exit loop
		else if(pid){ 
			printf("\n pid is: %d", pid);
			listProc[i] = pid;
			id = i;
			i = numOfComps+1;
			//Add parent process to write pipe
			printf("Child:%d created. Parent is:%d\n", id, getpid());
		}
		//if child stay in loop. If last step of loop, link pipe with original pipe.
		else{
			//if it's the last step of the loop (the last child), link with parent.
			if(i == (numOfComps - 1 )){

				 fdRead = fdp;
				 id = i+1;
				 printf("No more children, last process ID:%d\n", getpid());
			}
			//change fdWrite to fdRead, so that each process only has access to two pipes
			else{
				fdWrite = fdRead;
			}	
		}
	}
	int i = 1;
	printf("\nprocess is :%d\n", id);
	char* msg;	
	while(i){
			if(id == 1){
			msg = "*MSG Sent*";
			//printf("\n\nThis is parent%d#%d sending %s", 1, getpid(),msg);
			write(fdRead[WRITE], (const void *) "*MSG Sent*", (size_t) 11);
			i = 0;
		}
		else if (id == 2){
			printf("\nHere");
			fflush(stdout);
			sleep(1);
			char str[10];
			ssize_t numRead;
			numRead = read(fdWrite[READ], (void *) str, (size_t) 10);
			// Change both sixes here to the size of the string
			if ( numRead > 10){
				perror ("pipe read error\n");
				exit(1);
			}
			printf("abb");
			puts(str);
			printf("\n\nThis is child%d#%d receiving %s", 2, getpid(), str);
			fflush(stdout);
			i = 0;			
			
		}
		else{
			i = 0;
		}
	}
	
	printf("\nBye.");
	free(fdRead);
	free(fdWrite);	
	free(listProc);
	return 0;
}
