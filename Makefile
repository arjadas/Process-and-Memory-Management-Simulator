allocate: main.o read.o process.o task1.o queue.o task2.o bitmap.o helperfunctions.o page.o task3.o task4.o
	gcc -Wall -o allocate main.o read.o process.o task1.o queue.o task2.o bitmap.o helperfunctions.o page.o task3.o task4.o -lm

main.o: main.c read.h process.h queue.h task1.h task2.h task3.h task4.h page.h helperfunctions.h
	gcc -Wall -o main.o main.c -c

task4.o: task4.c task4.h read.h process.h queue.h page.h memory.h task3.h
	gcc -Wall -o task4.o task4.c -c

task3.o: task3.c task3.h read.h process.h queue.h page.h memory.h
	gcc -Wall -o task3.o task3.c -c

task2.o: task2.c read.h process.h queue.h memory.h bitmap.h
	gcc -Wall -o task2.o task2.c -c

task1.o: task1.c task1.h process.h queue.h
	gcc -Wall -o task1.o task1.c -c

helperfunctions.o: helperfunctions.c helperfunctions.h process.h
	gcc -Wall -o helperfunctions.o helperfunctions.c -c

queue.o: queue.c queue.h process.h 
	gcc -Wall -o queue.o queue.c -c

read.o: read.c read.h process.h page.h task4.h
	gcc -Wall -o read.o read.c -c

process.o: process.c process.h memory.h page.h
	gcc -Wall -o process.o process.c -c

bitmap.o: bitmap.c bitmap.h process.h memory.h
	gcc -Wall -o bitmap.o bitmap.c -c

page.o: page.c page.h process.h queue.h
	gcc -Wall -o page.o page.c -c

clean:
	rm -f *.o allocate