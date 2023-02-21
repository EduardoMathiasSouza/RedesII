all: server cliente

server: serverUDP.c
	gcc -g  -o server serverUDP.c -lm -pthread

cliente: clientUDP.c
	gcc -g -o cliente clientUDP.c -lm -pthread

clean:
	rm -f *.o server cliente 