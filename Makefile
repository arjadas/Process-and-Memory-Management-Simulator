allocate: main.o read.o process.o roundrobin.o queue.o
	gcc -Wall -o allocate main.o read.o process.o roundrobin.o queue.o -lm

main.o: main.c read.h process.h queue.h roundrobin.h
	gcc -Wall -o main.o main.c -c

roundrobin.o: roundrobin.c roundrobin.h process.h queue.h
	gcc -Wall -o roundrobin.o roundrobin.c -c

queue.o: queue.c queue.h process.h 
	gcc -Wall -o queue.o queue.c -c

read.o: read.c read.h process.h
	gcc -Wall -o read.o read.c -c

process.o: process.c process.h 
	gcc -Wall -o process.o process.c -c

clean:
	rm -f *.o allocate