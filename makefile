all: server cliente

server: serverUDP.c utils.o
	gcc -o server serverUDP.c utils.o -lm

cliente: clientUDP.c utils.o
	gcc -o cliente clientUDP.c utils.o -lm

utils.o: utils.c utils.h
	gcc -c utils.c -lm

clean:
	rm -f *.o server cliente 