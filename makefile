$CC=gcc
$CFLAGS=-ansi -Wall -march=NATIVE -o3 -D_GNU_SOURCE

all: gcshell
	rm -f *~ *.o

gcshell: gcshell.o string_util.o
	$(CC) gcshell.o string_util.o -o gcshell

gcshell.o: gcshell.c common.h
	$(CC) $(CFLAGS) -c gcshell.c

string_util.o: string_util.c common.h
	$(CC) $(CFLAGS) -c string_util.c

clean:
	rm -f ~* *.o
