CC=gcc
CFLAGS=-std=c99 -Wall -D_GNU_SOURCE

all: gcshell
	rm -f *~ *.o
	
gcshell: gcshell.o jobs.o builtin.o jobs.o
	$(CC) gcshell.o builtin.o jobs.o -o gcshell
	
gcshell.o: gcshell.c common.h
	$(CC) $(CFLAGS) -c gcshell.c
	
builtin.o: builtin.c common.h
	$(CC) $(CFLAGS) -c builtin.c

jobs.o: jobs.c common.h
	$(CC) $(CFLAGS) -c jobs.c
	

clean:
	rm -f *~ *.o gcshell
