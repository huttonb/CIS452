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
	int fd2[2];
	int fd3[2];
	int fd4[2];
	char* str = "test";
	char str1[6];

	pipe(fd);
	pipe(fd2);
	pipe(fd3);
	pipe(fd4);

	dup2(fd[READ], fd2[READ]);
	dup2(fd2[READ], fd3[READ]);
	dup2(fd3[READ], fd4[READ]);
	write(fd[WRITE], (const void *) str, 6);
	write(fd[WRITE], (const void *) str, 6);
//	write(fd[WRITE], (const void *) str, 6);
	read(fd2[READ], (void*) str1, (size_t) 5);
	printf("\n1:%s", str1);
	read(fd3[READ], (void*) str1, (size_t) 5);
	printf("\n2:%s", str1);
	read(fd4[READ], (void*) str1, (size_t) 5);
	printf("\n3:%s", str1);
	return 0;
	


}
