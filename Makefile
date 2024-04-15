allocate: main.o read.o process.o task1.o queue.o task2.o bitmap.o helperfunctions.o
	gcc -Wall -o allocate main.o read.o process.o task1.o queue.o task2.o bitmap.o helperfunctions.o -lm

main.o: main.c read.h process.h queue.h task1.h task2.h helperfunctions.h
	gcc -Wall -o main.o main.c -c

task2.o: task2.c read.h process.h queue.h memory.h bitmap.h
	gcc -Wall -o task2.o task2.c -c

task1.o: task1.c task1.h process.h queue.h
	gcc -Wall -o task1.o task1.c -c

helperfunctions.o: helperfunctions.c helperfunctions.h process.h
	gcc -Wall -o helperfunctions.o helperfunctions.c -c

queue.o: queue.c queue.h process.h 
	gcc -Wall -o queue.o queue.c -c

read.o: read.c read.h process.h
	gcc -Wall -o read.o read.c -c

process.o: process.c process.h memory.h
	gcc -Wall -o process.o process.c -c

bitmap.o: bitmap.c bitmap.h process.h memory.h
	gcc -Wall -o bitmap.o bitmap.c -c

clean:
	rm -f *.o allocate