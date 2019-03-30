all:    tenthreads

tenthreads: tenthreads.o
	gcc tenthreads.o -o tenthreads

tenthreads.o:   tenthreads.c
	gcc -c tenthreads.c -o tenthreads.o

clean:
	rm -f tenthreads tenthreads.o core *~