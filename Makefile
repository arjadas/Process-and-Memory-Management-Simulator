allocate: main.o read.o process.o 
	gcc -Wall -o allocate main.o read.o process.o 

main.o: main.c read.h process.h
	gcc -Wall -o main.o main.c -c

process.o: process.c process.h read.h
	gcc -Wall -o process.o process.c -c

read.o: read.c read.h
	gcc -Wall -o read.o read.c -c

clean:
	rm -f *.o allocate