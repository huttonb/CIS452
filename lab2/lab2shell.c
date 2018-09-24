#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX 256

/**********************************************************************
 * Simple shell program that reads and executes user input. Prompts 
 * the user for input, parses it, spawns a child process to execute 
 * the command, and provides some usage statistics about each executed  
 * process.
 * 
 * Authors: Edric Lin, Bryce Hutton
 * Version: 9/12/18
 **********************************************************************/

/**************************************************
 * Main method to run the shell program. Continues
 * to run until user inputs "quit" command.
 **************************************************/
int main(int argc, char* argv[]) {

	// prompt used by shell
	char* shell_prompt = "[simple_shell]$ ";
	
	// input entered by user
	char user_input[MAX];

	// array to store tokenized user input
	char** arguments;

	// keeps track of argument array size (including null pointer)
	int num_args;

	// process id for fork
	pid_t pid;

	// wait status
	int status;

	// storage for usage stats
	struct rusage usage;

	// loop until user inputs "quit" 
	while (strcmp(user_input, "quit") != 0) {

		// read user input
		printf("%s", shell_prompt); 
		fgets(user_input, MAX, stdin);
		
		// if empty input (i.e. newline), do nothing
		if (strcmp(user_input, "\n") == 0) {
			continue;
		}

		// "remove" newline from fgets
		user_input[strlen(user_input) - 1] = '\0';

		// parse arguments
		arguments = (char**) malloc(MAX * sizeof(char*));
		char* argument = strtok(user_input, " ");
		num_args = 0;
		while(argument != NULL) {	
		
			// allocate space and add argument to list
			*(arguments + num_args) = (char*) malloc((strlen(argument) + 1) * sizeof(char));	
			strcpy(*(arguments + num_args), argument);	

			// get next argument (sets argument to null if none left)
			argument = strtok(NULL, " ");
			num_args++;
		}
		*(arguments + num_args) = NULL;

		// print out filename and arguments for testing
		printf("Filename: |%s|\n", *arguments);
		printf("Arguments: ");
		for (int i = 0; i < num_args; i++) {
			printf("|%s|", *(arguments + i));
		}
		printf("\n");	

		//If command is CD, then don't fork, change directory instead
		if(strcmp(*arguments, "cd") == 0){
			if(num_args <= 0){
				printf("No directory specified");
			} 
			else{
 				chdir(*(arguments+1));
			}

		}
		else{

		// fork process to execute command
		pid = fork();
		
		// fork failed
		if (pid < 0) {
			printf("Error! Fork was not successful.");
		}

		// process is parent, wait for child
		else if (pid) {
			wait(&status);

			// print usage info
			getrusage(RUSAGE_CHILDREN, &usage);
			printf("User CPU time used: %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);			
			printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
			
			//printf("parent finished waiting. status is %d\n", status);
		}

		// process is child, execute command
		else {


			//printf("child about to run\n");
			execvp(*arguments, arguments);
		}
		}
		// free each argument and arguments list
		for (int i = 0; i < num_args; i++) {
			free(*(arguments + i));
		}
		free(arguments);
	}
	
	return 0;	
}
