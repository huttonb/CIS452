/*
 * Author: Bryce Hutton
 * Date: 9/5/2018
 * Lab1 Program1
 */ 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>

int main(){

	
	char* username;
	char hostName[100];
	char term[30];
	struct passwd *passName;
	int userID;
	int groupID; 
	

	//Prints the username using system call getlogin
	username = getlogin();
	printf("User is: %s\n", username);
	
	//Prints the UID using the getuid call.
	userID = getuid();
	printf("UID is: %d\n", userID);
	
	//Prints the GUID using the getgid call.
	groupID = getgid();
       	printf("GID is: %d\n", groupID);	
	
	//Prints hostname using gethostname call.
	//Makes hostname null-terminated and makes
	//namelen 1 less so that host-name will
	//always be null-terminated, regardless of whether or not
	//the name is too long.
	hostName[99] = '\0';
	gethostname(hostName, 99);
       	printf("Host is: %s\n", hostName);	

	//Prints the first 5 entires of the password file.
	//Starts by using setpwent to rewind to beginning of database.
	//Then sets the struct passName using getpwent()
	//to get the next entry, and prints the username from those entires.
	//Ends by calling endpwent() to close the database.
	setpwent();
	passName = getpwent();
	printf("Entry 1: %s\n", passName->pw_name);
	passName = getpwent();
	printf("Entry 2: %s\n", passName->pw_name);
       	passName = getpwent();
	printf("Entry 3: %s\n", passName->pw_name);
	passName = getpwent();
	printf("Entry 4: %s\n", passName->pw_name);
	passName = getpwent();
	printf("Entry 5: %s\n", passName->pw_name);	
	endpwent();	

	printf("Environment variable desired?\n");
	term[29] = '\0'; 
	fgets(term, 29, stdin);
	char* token = strtok(term, "\n");
	char* envVar = getenv(token);
	printf("Value is: %s\n: ", envVar);



	return 0;
}
