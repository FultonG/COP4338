all:	countprimes 

countprimes:	countprimes.c
		/usr/lib64/openmpi/bin/mpicc -O -o countprimes countprimes.c

clean:
		rm -f countprimes core *~
