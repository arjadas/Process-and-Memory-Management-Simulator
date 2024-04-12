allocate: main.o read.o process.o roundrobin.o queue.o task2.o bitmap.o
	gcc -Wall -o allocate main.o read.o process.o roundrobin.o queue.o task2.o bitmap.o -lm

main.o: main.c read.h process.h queue.h roundrobin.h task2.h
	gcc -Wall -o main.o main.c -c

task2.o: task2.c read.h process.h queue.h memory.h bitmap.h
	gcc -Wall -o task2.o task2.c -c

roundrobin.o: roundrobin.c roundrobin.h process.h queue.h
	gcc -Wall -o roundrobin.o roundrobin.c -c

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