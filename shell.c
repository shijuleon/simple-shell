#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>


//#define

char** parse_args(char input[]);

void execute(char **args);

int main(void){
	
	char **args, input[40];
	int should_run = 1, i=0;	

	while(should_run){ // loop till should_run = 0
		printf("sh>");
		fgets(input, 40, stdin);
		if (strncmp("exit", input, 4) == 0) exit(1);
		args = parse_args(input);
		execute(args);
	}
	return 0;
}


char** parse_args(char input[]){
	char *s, **args;
	int delimiter = 0;
	int passed = 1;
	s = strtok(input, " \t\n");

	while (s != NULL){
		//reallocate the pointer depending on the argument size
		args = realloc(args, (sizeof(char*) * ++delimiter)); 
		if (args == NULL) printf("Memory allocation failed");

		args[delimiter-1] = s;
		s = strtok(NULL, " \t\n");
		
	}

	//add NULL as the last element to indicate end 
	args = realloc(args, sizeof(char*) * delimiter+1);
	args[delimiter] = NULL;

	return args;
}

void execute(char **args){
	pid_t pid, child_to_kill;
	int status, error;
	if ((pid = fork()) < 0) {   
		printf("Error forking child process\n");
		exit(1);
     }
    else if (pid == 0) {          
		if (execvp(*args, args) < 0){
         	error = errno;
         	if (error == 2) printf("sh: command not found\n");
         	
         	child_to_kill = getpid();         	
         	kill(child_to_kill, SIGKILL); // kill child process in case execvp fails
         }
    } else {                                
        while (wait(&status) != pid)
           ;
    }
}