CFLAGS=-Wall -g -O1 -std=c11

all: mtvshell printargs

mtvshell.o: mtvshell.c
	gcc $(CFLAGS) -c  mtvshell.c

mtvshell: mtvshell.o
	gcc $(CLFAGS) -o mtvshell mtvshell.o

printargs.o: printargs.c
	gcc $(CFLAGS) -c printargs.c

printargs: printargs.o
	gcc $(CFLAGS) -o printargs printargs.o

.PHONY clean:
	rm mtvshell.o printargs.o mtvshell
