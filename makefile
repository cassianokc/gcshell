CC=gcc
CFLAGS=-std=c99 -Wall -o3 -march=native -D_GNU_SOURCE

LDFLAGS=
SOURCES=*.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gcshell

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

.c:
	$(CC) $(CFLAGS) $< -o $@
