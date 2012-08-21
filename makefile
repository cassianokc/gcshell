CC=gcc
CFLAGS=-ansi -Wall -o3 -march=native
LDFLAGS=
SOURCES=*.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gcshell

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

.c:
	$(CC) $(CFLAGS) $< -o $@
