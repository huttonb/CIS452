#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX 1024
#define READ 0
#define WRITE 1

int main(){
	int fd[2];
	pid_t pid;
	char* str = "test";
	char str1[6];

	pipe(fd);

	if ((pid = fork()) != 0){
	write(fd[WRITE], (const void *) str, 6);
	}
	else{
	read(fd[READ], (void*) str1, (size_t) 5);
	printf("\n1:%s", str1);
	}
	return 0;
	


}
