all:    grades

grades: grades.o
    gcc grades.o -o grades

grades.o:   grades.c
    gcc -c grades.c -o grades.o

clean:
    rm -f grades grades.o core *~
