all: server cliente

server: serverUDP.c
	gcc -g  -o server serverUDP.c

cliente: clientUDP.c
	gcc -g -o cliente clientUDP.c

clean:
	rm -f *.o server cliente 