/*  Name: Fulton Garcia
*   PantherID: 5464347
*	I affirm that I wrote this program myself without any help form any other people or sources from the internet.
*	Program Description: This program extends the myshell.c program to add pipes and reading from the standard output
*	If I missed anything I'm sorry I just got back from UHack and haven't slept in 36 hours
*/

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 20
#define BUFSIZE 1024

int get_args(char* terminal, char* args[]);
int get_cmds(char* terminal, char* cmds[]);
void execute_cmd(char* cmd);
void execute(char* terminal);

int main(int argc, char* argv[]){
	char terminal[BUFSIZE];

	//infinite loop so that we can keep getting commands until we break out of it
	while(1){
		printf("COP4338$ ");
		if (fgets(terminal, BUFSIZE, stdin) == NULL) {
			perror("Fatal error using fgets");
			exit(1);
		}
		execute(terminal);
	}
	return 0;
}

void execute(char* terminal){
	int pid, status;

	char* cmds[MAX_ARGS];
	int ncmds = get_cmds(terminal, cmds);

	pid = fork();
	if (pid == 0){

		if (ncmds == 1){
			execute_cmd(cmds[0]);
		}
		else if (ncmds == 2){
			int fpid;
			int fd[2];

			pipe(fd);

			fpid = fork();
			if (fpid == 0){
				dup2(fd[1], 1);
				close(fd[0]);
				execute_cmd(cmds[0]);
			}
			else if (fpid > 0){
				dup2(fd[0], 0);
				close(fd[1]);
				execute_cmd(cmds[1]);
			}
			close(fd[0]);
			close(fd[1]);
		}
		else if (ncmds == 3){
			int fpid;

			int fd1[2];
			int fd2[2];

			pipe(fd1);

			fpid = fork();
			if (fpid == 0){
				dup2(fd1[1], 1);
				close(fd1[0]);
				close(fd1[1]);
				execute_cmd(cmds[0]);
			}

			pipe(fd2);

			fpid = fork();
			if (fpid == 0){
				dup2(fd1[0], 0);
				// output to pipe2
				dup2(fd2[1], 1);
				// close fds
				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);
				execute_cmd(cmds[1]);
			}

			close(fd1[0]);
			close(fd1[1]);

			fpid = fork();
			if (fpid == 0){
				dup2(fd2[0], 0);
				close(fd2[0]);
				close(fd2[1]);
				execute_cmd(cmds[2]);
			}

		}
	}
	else if (pid > 0){
		waitpid(pid, NULL, 0);
	}
	else {
		perror("fork failed");
		exit(1);
	}
}

//following the suggestions from the instructions
//here I am breaking up the commands into individual commands
//separated by the | 
int get_cmds(char* terminal, char* cmds[]){
	int i = 0;

	//When you have no arguments just return 1
	if ((cmds[0] = strtok(terminal, "|")) == NULL){
		return 1;
	}

	//one liner that will auto increase the position at cmds by 1 every time it checks
	while ((cmds[++i] = strtok(NULL, "|")) != NULL);

	/* the last one is always NULL */
	return i;
}

//Execute the command line
void execute_cmd(char* cmd){
	int pid, async;
	char* args[MAX_ARGS];

	int nargs = get_args(cmd, args);
	if (nargs <= 0) return;

	//you can quite the shell by typing quit or exit
  	if(!strcmp(args[0], "quit") || !strcmp(args[0], "exit")) {
	 printf("Exiting the shell\n");
	 exit(0);
	}


	//checking to see if the call is asynchronous. could be handled with Async await or promises had this been JS lol
	if (!strcmp(args[nargs - 1], "&")) { async = 1; args[--nargs] = 0; }
	else async = 0;

	//Set variables, flags and file pointers
	int index = 0, redirectOutputFlag = 0, redirectInputFlag = 0, appendOutputFlag = 0;
	int in, out;
	char* outputName;
	char* inputName;

	FILE *output, *input;

	while (index < nargs){
		//redirect output from standard output
		if (!strcmp(args[index], ">")){
			redirectOutputFlag = 1;
			outputName = args[index + 1];
			args[index] = 0;
			args[++index] = 0;
		}
		//append
		else if (!strcmp(args[index], ">>")){
			appendOutputFlag = 1;
			outputName = args[index + 1];
			args[index] = 0;
			args[++index] = 0;
		}
		//redirect input
		else if (!strcmp(args[index], "<")){
			redirectInputFlag = 1;
			inputName = args[index + 1];
			args[index] = 0;
			args[++index] = 0;
		}
		index++;
	}


	if (redirectOutputFlag){
		if ((output = fopen(outputName, "w")) == NULL)
			return;
		out = fileno(output);
		dup2(out, STDOUT_FILENO);
	}
	else if (appendOutputFlag){
		if ((output = fopen(outputName, "a")) == NULL)
			return;
		out = fileno(output);
		dup2(out, STDOUT_FILENO);
	}

	if (redirectInputFlag){
		if ((input = fopen(inputName, "r")) == NULL)
			return;
		in = fileno(input);
		dup2(in, STDIN_FILENO);
	}

	execvp(args[0], args);
	
	perror("Fatal Exec Falure");


	exit(-1);

}

//returns a count of how many arguments there are in the command.
int get_args(char* terminal, char* args[])
{
	int i = 0;

	/* if no args */
	if((args[0] = strtok(terminal, "\n\t ")) == NULL)
		return 0;

	while((args[++i] = strtok(NULL, "\n\t ")) != NULL) {
		if(i >= MAX_ARGS) {
			printf("Too many arguments!\n");
			exit(1);
		}
	}
	/* the last one is always NULL */
	return i;
}










