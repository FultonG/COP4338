all:    minicalc

minicalc: minicalc.o
	gcc minicalc.o -o minicalc

minicalc.o:   minicalc.c
	gcc -c minicalc.c -o minicalc.o

clean:
	rm -f minicalc minicalc.o core *~
