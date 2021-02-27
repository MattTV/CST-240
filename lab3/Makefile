CFLAGS=-Wall -g -O1 -std=c11

all: list	

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

listtest.o: listtest.c list.h
	gcc $(CFLAGS) -c listtest.c

list: list.o listtest.o
	gcc $(CFLAGS) -o list list.o listtest.o
	
clean:
	rm list list.o listtest.o

# All should depend on list, and list depends on list.o and listtest.o,
#  and list.o depends on list.c and list.h, listtest.o depends on
#  listtest.c and list.h (It needs the header file only). Both of those
#  only compile, then we do the linking step on list to produce a final
#  list executable.
