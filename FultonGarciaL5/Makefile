all:    minishell

minishell: minishell.o
	gcc minishell.o -o minishell

minishell.o:   minishell.c
	gcc -c minishell.c -o minishell.o

clean:
	rm -f minishell minishell.o core *~
