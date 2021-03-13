CFLAGS=-Wall -g -O1 -std=c11 -lpthread

all: client server

client.o: client.c
	gcc $(CFLAGS) -c  client.c

client: client.o
	gcc $(CLFAGS) -o client client.o common.o

server.o: server.c
	gcc $(CFLAGS) -c server.c

server: server.o
	gcc $(CFLAGS) -o server server.o common.o

common.o: common.c common.h
	gcc $(CFLAGS) -c common.c

.PHONY clean:
	rm mtvshell.o printargs.o mtvshell
