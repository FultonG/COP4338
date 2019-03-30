all:    mywords

mywords: mywords.o
	gcc mywords.o -o mywords

mywords.o:   mywords.c
	gcc -c mywords.c -o mywords.o

clean:
	rm -f mywords mywords.o core *~
