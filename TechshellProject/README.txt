Group Members: Zachary Uson and Jonathan Wellmeyer

Inner Workings: Our techshell.c program displays the built-in functionalities of cd, pwd, and exit. These three functions are executed in the parent process while all other commands, which are external, are executed in child process.

| CD |

The cd function utilizes the C functions getcwd() and chdir() to change and grab directories. The function error handles itself by ensuring the directory it changes to exists.

| PWD |

This is a simple function that also utilizes the getcwd() function. After getting directory, the function displays the current directory to the user.

| EXIT |

This is another simple function that just utilizes the exit() function from C. This function is used in both the child and parent processes. The child process requires exiting since exit() only exits the process itself (not the parent process). When used in the parent process, the program terminates.

| SYSERR |

This function error checks any invalid input by user. It displays the error number provided by Linux and the message that goes along with the number. Also, the function formats the error message as displayed in the example runthrough on the project description.

| CHECKIO |

This is the input output redirection function. The function resets all redirection variables at the beginning. The output and append need to equal 1 after being called in order to function correctly later on in the program. The function is called at the beginning of the shell loop in order to repeatedly iterate through the command line.

| SETSHELLENVIRONMENT |

Function is explained in the name, it sets shell environment. The function is also required in order for error handling to work correctly.

| MAIN |

The main function splits into parent and child processes. The parent process controls the built-in commands (cd, pwd, exit) while the child process all other simple UNIX commands. The loop runs as long as user does not input "exit" or end of file redirected input has nothing. The main function also reads arguments and stores them in args array (this is what the command line arguments are stored in).  
