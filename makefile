CC=gcc
CFLAGS=-ansi -Wall -D_GNU_SOURCE

all: gcshell
	rm -f *~ *.o

gcshell: gcshell.o string_util.o builtin.o
	$(CC) gcshell.o string_util.o builtin.o -o gcshell
	
gcshell.o: gcshell.c common.h
	$(CC) $(CFLAGS) -c gcshell.c
	
builtin.o: builtin.c common.h
	$(CC) $(CFLAGS) -c builtin.c

string_util.o: string_util.c common.h
	$(CC) $(CFLAGS) -c string_util.c

clean:
	rm -f *~ *.o gcshell
