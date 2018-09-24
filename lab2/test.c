#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int main(){
	printf("Please give prompt");

	char term[30];
	term[29] = '\0';
	fgets(term, 29, stdin);
	int numOfTerms = 0;
	//When space is 0, it is currently a word
	int space = 0;
	for(int i = 0; i < strlen(term) - 1; i++){
		if(isspace(term[i])){
			space = 1;
			//numOfTerms++;
		}
		else{
			if(space == 1){
				numOfTerms++;
				space = 0;
			}
		}

	}
	char *token = strtok(term, " ");
	char *arrTerms[numOfTerms];
	char *firstTerm  = token;
	printf("%s\n", firstTerm);
	for(int i = 0; i < numOfTerms; i++){
		token = strtok(NULL, " ");
		arrTerms[i] = token;
		printf("%s\n", arrTerms[i]);
	}
	return 0;


}
