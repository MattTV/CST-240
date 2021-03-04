OBJS = list.o \
       listtest.o \

CFLAGS = -Wall -g -O1 -std=c11
CC = gcc

all: test

clean:
	rm -f *.o
	rm -f listtest

%.o: %.c Makefile
	gcc $(CFLAGS) -c -o $@ $<

listtest: $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o listtest

test: listtest
	valgrind --leak-check=full ./listtest

