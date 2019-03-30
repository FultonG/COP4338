/*  Name: Fulton Garcia
*   PantherID: 5464347
*	This program is to show how to run child processes and how to make sure they are terminated
*	before ending the main parent process. We make an ls -al cmd and a grep minishell.c command run
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//these are my two commands that will be run
//ls will be the first child
char *ls[] = { "ls", "-al", 0 };
//grep will be the second child
char *grep[] = { "grep", "minishell.c", 0 };

void runSource(int pfd[]);
void runDest(int pfd[]);

int main(int argc, char **argv){
	int pid, status; 
	int fd[2];

	pipe(fd); //Create pipe

	//run the ls
	runSource(fd);
	//run the grep
	runDest(fd);

	close(fd[0]); close(fd[1]);

	while ((pid = wait(&status)) != -1)
	{
		fprintf(stderr, "process %d exits with %d\n",pid, status);
	}
	exit(0);
}


void runSource(int pfd[]){
	int pid;	

	switch (pid = fork()) {

	case 0:
		dup2(pfd[1], 1);
		close(pfd[0]);
		execvp("ls", ls);
		//if it fails
		perror(ls[0]);

	default:
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}

void runDest(int pfd[])
{
	int pid;

	switch (pid = fork()) {

	case 0:
		dup2(pfd[0], 0);
		close(pfd[1]);
		execvp("grep", grep);
		//if it fails
		perror(grep[0]);

	default:
		break;

	case -1:
		perror("fork");
		exit(1);
	}
}

