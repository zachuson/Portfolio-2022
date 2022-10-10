/*
 * Names: Zachary Uson and Jonathan Wellmeyer
 * Date: 2/26/2021
 * Class: CSC 222 001
 */

// libraries required to run program
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// max line buffer
#define MAX_BUFFER 1024
// max arguments
#define MAX_ARGS 64        
// token separators
#define SEPARATORS " \t\n" 

// initialize all functions used in program
void cd(char *path);
void pwd();
void exitProgram(int i);
void syserr();
void checkIO(char **args);
void setShellEnvironment();

// error number
extern int errno;   
// process ID
pid_t pid;         
// status for the forking process
int status;             
// variables for I/O redirection
int input, output, append;
// files for redirection
char *inputFile, *outputFile;         

//change directory function
void cd(char *path) {

	// initialize the current working directory string
  	char * name = "PWD";
	// holder for the current working directory
  	char cwd[256];
	// get current working directoy and store it in variable
  	char * newDirectory = getcwd(cwd, sizeof(cwd));

	// throw error if path does not exist
	if(chdir(path) != 0)
		 syserr();
	// if path is viable, set the new directory
	else
  		setenv(name, newDirectory, 1);

}

// displays current directory
void pwd() {

	// holder for current directory
	char cwd[256];
	// put current directory in variable
	char * directory = getcwd(cwd, sizeof(cwd));

	// displays current directory
	printf("%s\n", directory);

}

// exits program/loop; is used to exit child process as well
void exitProgram(int i) {

	exit(i);

}

//error printing
void syserr() {

	// prints error number along with the error message
  	fprintf(stderr, "Error %i (%s)\n", errno, strerror(errno));

}

// check the command for any I/O redirection
void checkIO(char **args){
  	// reset all variables for I/O redirection
  	input = 0;
  	output = 0;
  	append = 0;

  	int i = 0;

  	// loop through the command line input
  	while(args[i] != NULL){

		// check for input
    		if (!strcmp(args[i], "<")){

      			strcpy(args[i], "\0");
      			inputFile = args[i+1];
			break;

    		}

		// check for output
    		else if (!strcmp(args[i], ">")) {

      			outputFile = args[i+1];
      			args[i] = NULL;
      			output = 1;
      			break;

    		}

		// check for append
    		else if (!strcmp(args[i], ">>")){  

      			outputFile = args[i+1];
      			args[i] = NULL;
      			append = 1;
      			break;

    		}

    		i++;

  	}
}

// set the shell environment variable
// this is also required for error handling to function
void setShellEnvironment(){

  	char orig_path[1024];
  	
	getcwd(orig_path, 1024);
  	strcat(orig_path, "/techshell");
  	setenv("shell", orig_path, 1);

}

// the main function
int main(int argc, char ** argv) {

	// initilize arrays required for program to run
	char buffer[MAX_BUFFER];
	char cwd[256];
  	char * args[MAX_ARGS];
  	char ** arg;
	// displays the second section of the prompt (the $)
	char * prompt2 = "$ ";
  	int status;

  	setShellEnvironment(); // get the shell environment

	// input is read until the user inputs "exit" or end of file of redirected input
  	while(!feof(stdin)) {
		
		// displays the first section of the prompt (the directory)
		char * prompt1 = strdup(getcwd(cwd, sizeof(cwd)));

		// displays the full prompt to the user
		fputs(prompt1, stdout);
    		fputs(prompt2, stdout);

		// reads the line inputted
		if(fgets(buffer, MAX_BUFFER, stdin)) {
      		
			// initialize args array (this will hold the command line arguments
			arg = args;
			// tokenize input
      			*arg++ = strtok(buffer, SEPARATORS);
      
			// to continue input, we insert this while loop since the last input will be NULL
			while ((*arg++ = strtok(NULL,SEPARATORS)));

			// check I/O redirections
			checkIO(args);
      			
			// checks if there is an argument
			if (args[0]) {
			
				// these are the three built-in commands

				// change directory
        			if (!strcmp(args[0],"cd")) {        
	
					// call change directory function
					cd(args[1]);
					continue;
	
				}

				// display the current directory
				if (!strcmp(args[0], "pwd")) {

					// call the current directory function
					pwd();
					continue;

				}
        

				// exit from loop if "exit" is inputted
				if (!strcmp(args[0], "exit")) {
				
					exitProgram(0);

				}
	
        			// else runs the external commands
        			else {
         
					pid = getpid();
          
					// forks processes
					switch (pid = fork ()) { // in child process 
            
						// error handle
						case -1:
              						syserr();
							exitProgram(1);

            					// sets the environment of the parent and handles output and append redirection
						case 0:
              						setenv("parent", getenv("shell"), 1);
              						
              						if(output == 1)
                						freopen(outputFile, "w", stdout);
              						else if(append == 1)
                						freopen(outputFile, "a+", stdout); 
							// executes arguments given in command line
							execvp (args[0], args);
            
							// error handles if argument given is invalid
							syserr();

							// to ensure "exit" works properly, we must exit the child process after execution
							exitProgram(1);

						// waits for processes
						default:
							
              						waitpid(pid, &status, WUNTRACED);
        
					}
          
					continue;
        
				}      
			}
    
		} 
  
	}
  
	return 0;

}


