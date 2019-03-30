all:    bstsort

bstsort: bstsort.o
	gcc bstsort.o -o bstsort

bstsort.o:   bstsort.c
	gcc -c bstsort.c -o bstsort.o

clean:
	rm -f bstsort bstsort.o core *~
