#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(){
	int temp;
	int status;
	pid_t pid;
	temp = open("temp", O_WRONLY|O_CREAT);
	
	dup2(out, 1);

	pid = fork();
	
	if (pid < 0){
		printf("Error! Fork was not successful.");
	}
	//parent
	else if(pid){
		wait(&status);
	}
	//child
	else{
		printf("Hello\n.");
	}
	return 0;



}
