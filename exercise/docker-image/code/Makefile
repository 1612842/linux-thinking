all: server.o client.o

server.o: server.c
	 gcc -pthread server.c -o server.o
     
client.o: client.c
	 gcc -pthread client.c -o client.o

clean:
	 rm server.o client.o *.txt
